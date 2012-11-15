/*
 * cMap.cpp
 *
 *  Created on: Nov 14, 2012
 *      Author: agrum
 */

#include "cMap.h"

cMap::cMap(unsigned int p_seed, unsigned int p_nodes, unsigned int p_expand):
m_nodes(p_nodes)
{
	QList<QVector3D> nodeList;
	QMap<float, int> linkMap;
	QList<int> linkListSorted;
	QList<cPath> unitPathList;
	QList<cPath> longPathList;

	//Node configuration
	qsrand(p_seed);
	for(unsigned int i = 0; i < p_nodes; i++)
		nodeList.push_back(QVector3D (qrand()%1000, qrand()%1000, qrand()%1000));
	//Unitary pathes
	for(unsigned int i = 0; i < p_nodes-1; i++){
		int toAdd = 1+qrand()%3;
		linkMap.clear();
		for(unsigned int j = i+1; j < p_nodes; j++)
			linkMap.insertMulti((nodeList[i] - nodeList[j]).lengthSquared(), j);
		linkListSorted = linkMap.values();
		for(int j = 0; j < toAdd && j < linkListSorted.size(); j++){
			cPath tmp(i, linkListSorted[j], 100 + qrand()%400);
			m_pathList.push_back(tmp);
		}
	}
	//Expansion
	unitPathList = longPathList = m_pathList;
	for(unsigned int i = 0; i < p_expand; i++){
		longPathList = expand(unitPathList, longPathList);
		m_pathList.append(longPathList);
	}
	//Order
	for(int i = 0; i < m_pathList.size()-1; i++)
		for(int j = i+1; j < m_pathList.size(); j++)
			if(m_pathList[j].distance() < m_pathList[i].distance())
				m_pathList.swap(i, j);
	//Result
	//show();
	//Write conf file
	QString name = QString("Map_%1_%2_%3")
			.arg(p_seed)
			.arg(p_nodes)
			.arg(p_expand);
	save(name);
}

cMap::cMap(const QString& p_name){
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
				m_nodes = e.text().toInt();
			else if(e.tagName() == "Path"){
				cPath tmp(e);
				m_pathList.append(tmp);
			}
		}
		n = n.nextSibling();
	}

	//Result
	//show();
}

int cMap::size() const {
	return m_nodes;
}

QList<cPath> cMap::pathList(int p_node) const {
	QList<cPath> rtn;

	if(p_node < 0)
		return m_pathList;

	for(int i = 0; i < m_pathList.size(); i++)
		if(m_pathList[i].opposite(p_node) != p_node)
			rtn.push_back(m_pathList[i]);

	return rtn;
}

QList<cPath> cMap::expand(const QList<cPath>& p_unit, const QList<cPath>& p_long){
	QList<cPath> rtn;
	QList<cPath> toRemoveOld;
	QList<cPath> toRemoveNew;

	//List of all new path
	for(int i = 0; i < p_unit.size(); i++){
		for(int j = 0; j < p_long.size(); j++){
			cPath tmp(p_long[j]);
			if(tmp.merge(p_unit[i]))
				rtn.append(tmp);
		}
	}

	//List of replica
	for(int i = 0; i < rtn.size()-1; i++){
		for(int j = i+1; j < rtn.size(); j++){
			if(rtn[i].sameHeads(rtn[j])){
				if(rtn[i].distance() < rtn[j].distance())
					toRemoveNew.append(rtn[j]);
				else
					toRemoveNew.append(rtn[i]);
			}
		}
	}
	while(!toRemoveNew.empty())
		rtn.removeOne(toRemoveNew.takeFirst());

	for(int i = 0; i < m_pathList.size(); i++){
		for(int j = 0; j < rtn.size(); j++){
			if(m_pathList[i].sameHeads(rtn[j])){
				if(m_pathList[i].distance() < rtn[j].distance())
					toRemoveNew.append(rtn[j]);
				else
					toRemoveOld.append(m_pathList[i]);
			}
		}
	}

	//Remove replica
	while(!toRemoveOld.empty())
		m_pathList.removeOne(toRemoveOld.takeFirst());
	while(!toRemoveNew.empty())
		rtn.removeOne(toRemoveNew.takeFirst());

	return rtn;
}

void cMap::save(const QString& p_name){
	QDomDocument doc(p_name);
	QDomElement root = doc.createElement("Map");
	QDomElement tag;
	QDomElement sub;

	QFile file(p_name + ".conf");
	if (!file.open(QIODevice::WriteOnly))
		return;

	doc.appendChild(root);
	tag = doc.createElement("Node");
	root.appendChild(tag);
	tag.appendChild(doc.createTextNode(QString("%1").arg(m_nodes)));

	for(int i = 0; i < m_pathList.size(); i++)
		m_pathList[i].save(root);

	QString xml = doc.toString();
	file.write(xml.toAscii());
}

void cMap::show() const {
	for(int i = 0; i < m_pathList.size(); i++)
		qDebug()
		<< i
		<< m_pathList[i].distance()
		<< m_pathList[i].nodeList().first()
		<< m_pathList[i].nodeList().last();
	qDebug() << m_pathList.size();
}
