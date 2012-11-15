/*
 * cPath.cpp
 *
 *  Created on: Nov 12, 2012
 *      Author: agrum
 */

#include "cPath.h"

cPath::cPath(int p_0, int p_1, float p_distance):
cLink<int>(p_0, p_1, p_distance)
{

}

cPath::cPath(const QDomNode& p_node){
	QDomNode nn = p_node.firstChild();
	while(!nn.isNull()) {
		QDomElement ee = nn.toElement();
		if(!ee.isNull()) {
			if(ee.tagName() == "Node")
				m_nodeList.append(ee.text().toInt());
			else if(ee.tagName() == "Length")
				m_distance = ee.text().toFloat();
		}
		nn = nn.nextSibling();
	}
}

cPath::cPath(const cPath& p_path):
cLink<int>(p_path)
{

}

cPath& cPath::operator=(const cPath& p_path){
	m_nodeList = p_path.m_nodeList;
	m_distance = p_path.m_distance;

	return *this;
}

bool cPath::operator==(const cPath& p_path) const{
	return m_nodeList == p_path.m_nodeList;
}

bool cPath::sameHeads(const cPath& p_path) const{
	return ((m_nodeList.first() == p_path.m_nodeList.first()
			&& m_nodeList.last() == p_path.m_nodeList.last())
			||
			(m_nodeList.first() == p_path.m_nodeList.last()
			&& m_nodeList.last() == p_path.m_nodeList.first()));
}

void cPath::save(QDomNode& p_node){
	QDomDocument doc = p_node.toDocument();
	QDomElement sub;
	QDomElement tag = doc.createElement("Path");
	p_node.appendChild(tag);

	sub = doc.createElement("Length");
	tag.appendChild(sub);
	sub.appendChild(doc.createTextNode(QString("%1")
			.arg(m_distance)));

	for(int i = 0; i < m_nodeList.size(); i++){
		sub = doc.createElement("Node");
		p_node.appendChild(sub);
		sub.appendChild(doc.createTextNode(QString("%1")
				.arg(m_nodeList[i])));
		tag.appendChild(sub);
	}
}

bool cPath::merge(cPath p_path){
	if(!canMerge(p_path))
		return false;

	m_distance += p_path.distance();
	if(m_nodeList.last() == p_path.m_nodeList.first()){
		p_path.m_nodeList.removeFirst();
		m_nodeList = m_nodeList + p_path.m_nodeList;
	}
	else if(m_nodeList.first() == p_path.m_nodeList.last()){
		p_path.m_nodeList.removeLast();
		m_nodeList = p_path.m_nodeList + m_nodeList;
	}
	else if(m_nodeList.last() == p_path.m_nodeList.last()){
		for(int i = 0; i < p_path.m_nodeList.size()/2; i++)
			p_path.m_nodeList.swap(i, p_path.m_nodeList.size()-1-i);
		p_path.m_nodeList.removeFirst();
		m_nodeList = m_nodeList + p_path.m_nodeList;
	}
	else if(m_nodeList.first() == p_path.m_nodeList.first()){
		for(int i = 0; i < p_path.m_nodeList.size()/2; i++)
					p_path.m_nodeList.swap(i, p_path.m_nodeList.size()-1-i);
		p_path.m_nodeList.removeLast();
		m_nodeList = p_path.m_nodeList + m_nodeList;
	}

	return true;
}

bool cPath::canMerge(cPath p_path){
	if(m_nodeList.first() == p_path.m_nodeList.last()
			|| m_nodeList.last() == p_path.m_nodeList.last())
		p_path.m_nodeList.removeLast();
	else if(m_nodeList.first() == p_path.m_nodeList.first()
			|| m_nodeList.last() == p_path.m_nodeList.first())
		p_path.m_nodeList.removeFirst();
	else
		return false;

	for(int i = 0; i < p_path.m_nodeList.size(); i++)
		if(m_nodeList.contains(p_path.m_nodeList[i]))
			return false;

	return true;
}
