/*
 * cCRList.h
 *
 *  Created on: Nov 13, 2012
 *      Author: agrum
 */

#ifndef CCRLIST_H_
#define CCRLIST_H_

#include <QList>
#include <QVector3D>
#include "map/cPath.h"

#define MAX_EXTRA_NODE 3

template <class T>
class cNetwork : public QList<T*> {

public:
	cNetwork(unsigned int, unsigned int, unsigned int);
	cNetwork(const QString&);
	~cNetwork();
};

template <class T>
cNetwork<T>::cNetwork (unsigned int p_seed, unsigned int p_nodes, unsigned int p_expand) {
	//CR gen
	for(unsigned int i = 0; i < p_nodes; i++)
		push_back(new T());

	//Map gen
	QList<QVector3D> nodeList;
	QList<cLink*> linkList;
	QMap<float, int> linkMap;
	QList<int> linkListSorted;

	qsrand(p_seed);
	for(unsigned int i = 0; i < p_nodes; i++)
		nodeList.push_back(QVector3D (qrand()%1000, qrand()%1000, qrand()%1000));

	for(unsigned int i = 0; i < p_nodes-1; i++){
		int toAdd = 1+qrand()%MAX_EXTRA_NODE;
		linkMap.clear();
		for(unsigned int j = i+1; j < p_nodes; j++)
			linkMap.insertMulti((nodeList[i] - nodeList[j]).lengthSquared(), j);
		linkListSorted = linkMap.values();
		for(int j = 0; j < toAdd && j < linkListSorted.size(); j++)
			linkList.push_back(new cLink(this->at(i), this->at(linkListSorted[j]), 100+qrand()%400));
	}

	cPath::init(p_expand, linkList);
	for(int i = 0; i < this->size(); i++)
		this->at(i)->setPathList();
	cPath::clear();

	//Save
	QList<cPath*> pathList;
	QString name = QString("Map_%1_%2_%3")
								.arg(p_seed)
								.arg(p_nodes)
								.arg(p_expand);
	QDomDocument doc(name);
	QDomElement root = doc.createElement("Map");
	QDomElement tag;
	QDomElement sub;
	doc.appendChild(root);

	QFile file(name + ".conf");
	if (!file.open(QIODevice::WriteOnly))
		return;

	tag = doc.createElement("Node");
	root.appendChild(tag);
	tag.appendChild(doc.createTextNode(QString("%1").arg(p_nodes)));

	pathList = cPath::getList(NULL);
	for(int i = 0; i < pathList.size(); i++){
		tag = doc.createElement("Path");
		root.appendChild(tag);

		sub = doc.createElement("Length");
		tag.appendChild(sub);
		sub.appendChild(doc.createTextNode(QString("%1")
				.arg(pathList[i]->distance())));

		for(int j = 0; j < pathList[i]->size(); j++){
			sub = doc.createElement("Node");
			tag.appendChild(sub);
			sub.appendChild(doc.createTextNode(QString("%1")
					.arg(indexOf((T*) pathList[i]->at(j)))));
		}
	}

	QString xml = doc.toString();
	file.write(xml.toAscii());
}

template <class T>
cNetwork<T>::cNetwork (const QString& p_name) {
	QList<QPair<float, QList<void*> > > pathList;
	QDomDocument doc("Map");
	QFile file(p_name);
	if (!file.open(QIODevice::ReadOnly))
		return;
	if (!doc.setContent(&file)) {
		file.close();
		return;
	}
	file.close();

	QDomElement docElem = doc.documentElement();

	QDomNode n = docElem.firstChild();
	while(!n.isNull()) {
		QDomElement e = n.toElement();
		if(!e.isNull()) {
			if(e.tagName() == "Node")
				for(int i = 0; i < e.text().toInt(); i++)
					push_back(new T());
			else if(e.tagName() == "Path"){
				QPair<float, QList<void*> > path;
				QDomNode nn = e.firstChild();
				while(!nn.isNull()) {
					QDomElement ee = nn.toElement();
					if(!ee.isNull()) {
						if(ee.tagName() == "Length")
							path.first = ee.text().toFloat();
						else if(ee.tagName() == "Node")
							path.second.push_back(this->at(ee.text().toInt()));
					}
					nn = nn.nextSibling();
				}
				pathList.push_back(path);
			}
		}
		n = n.nextSibling();
	}

	cPath::init(pathList);
	for(int i = 0; i < this->size(); i++)
		this->at(i)->setPathList();
	cPath::clear();
}

template <class T>
cNetwork<T>::~cNetwork (){
	while(!this->empty())
		 delete this->takeFirst();
}

#endif /* CCRLIST_H_ */
