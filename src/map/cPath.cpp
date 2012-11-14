/*
 * cPath.cpp
 *
 *  Created on: Nov 12, 2012
 *      Author: agrum
 */

#include "cPath.h"

QList<QList<cPath*> > cPath::m_pathList;

cPath::cPath(float p_distance, QList<void*> p_nodeList):
QList<void*>(p_nodeList),
m_distance(p_distance),
m_lvl(p_nodeList.size()-1)
{

}

cPath::cPath(const cLink* p_link):
m_distance(p_link->distance()),
m_lvl(1)
{
	push_back(p_link->opposite(NULL));
	push_back(p_link->opposite(at(0)));
}

cPath::cPath(const cPath* p_path0, const cPath* p_path1):
m_distance(p_path0->m_distance + p_path1->m_distance),
m_lvl(p_path0->m_lvl + p_path1->m_lvl)
{
	QList<void*> tmp;

	if(p_path0->last() == p_path1->first() || p_path0->last() == p_path1->last()){
		tmp = *p_path1;
		if(p_path0->last() == p_path1->last())
			for(int i = 0; i < tmp.size()/2; i++)
				tmp.swap(i, tmp.size()-1-i);
		tmp.removeFirst();
		append(*p_path0);
		append(tmp);
	}
	else if(p_path0->first() == p_path1->last() || p_path0->first() == p_path1->first()){
		tmp = *p_path1;
		if(p_path0->first() == p_path1->first())
			for(int i = 0; i < tmp.size()/2; i++)
				tmp.swap(i, tmp.size()-1-i);
		tmp.removeLast();
		append(tmp);
		append(*p_path0);
	}
}

float cPath::distance() const {
	return m_distance;
}

float cPath::level() const {
	return m_lvl;
}

void* cPath::opposite(void* p_01) const {
	return (first() == p_01) ? last() : first();
}

void cPath::init(unsigned int p_expand, const QList<cLink*>& p_list){
	QList<cPath*> lvlOne;

	m_pathList.clear();
	m_pathList.push_back(lvlOne);

	for(int i = 0; i < p_list.size(); i++)
		m_pathList[0].push_back(new cPath(p_list[i]));

	for(unsigned int e = 0; e < p_expand; e++)
		expand();

	while(m_pathList.size() > 1)
		m_pathList[0].append(m_pathList.takeAt(1));

	order();

	/*for(int i = 0; i < m_pathList[0].size(); i++)
		qDebug() << m_pathList[0][i]->m_lvl
		<< m_pathList[0][i]->m_distance;

	qDebug() << m_pathList[0].size();*/
}

void cPath::init(const QList<QPair<float, QList<void*> > >& p_pathList){
	QList<cPath*> lvlOne;
	m_pathList.push_back(lvlOne);

	for(int i = 0; i < p_pathList.size(); i++)
		m_pathList[0].push_back(new cPath(p_pathList[i].first, p_pathList[i].second));

	/*for(int i = 0; i < m_pathList[0].size(); i++)
			qDebug() << m_pathList[0][i]->m_lvl
			<< m_pathList[0][i]->m_distance;

	qDebug() << m_pathList[0].size();*/
}

QList<cPath*> cPath::getList(void* p_node) {
	QList<cPath*> rtn;

	if(p_node == NULL)
		return m_pathList[0];

	for(int i = 0; i < m_pathList[0].size(); i++)
		if(m_pathList[0][i]->first() == p_node || m_pathList[0][i]->last() == p_node)
			rtn.push_back(m_pathList[0][i]);

	return rtn;
}

void cPath::clear(){
	m_pathList.clear();
}

bool cPath::canJoin(const cPath* p_path){
	if(this == p_path)
		return false;

	QList<void*> tmp;
	if(last() == p_path->first() || last() == p_path->last()){
		tmp = *this;
		tmp.removeLast();
	}
	else if(first() == p_path->last() || first() == p_path->first()){
		tmp = *this;
		tmp.removeFirst();
	}
	else
		return false;

	for(int i = 0; i < tmp.size(); i++)
		if(p_path->contains(tmp[i]))
			return false;

	return true;
}

bool cPath::operator==(const cPath& p_path) const{
	return ((first() == p_path.first() && last() == p_path.last())
			|| (first() == p_path.last() && last() == p_path.first()));
}

void cPath::expand(){
	QList<QPair<int, void*> > toRemove;
	QList<cPath*> newLvl;
	int lastLvl = m_pathList.size() - 1;

	m_pathList.push_back(newLvl);
	for(int i = 0; i < m_pathList[0].size(); i++){
		for(int j = 0; j < m_pathList[lastLvl].size(); j++){
			if(m_pathList[0][i]->canJoin(m_pathList[lastLvl][j]))
				m_pathList[lastLvl+1].push_back(new cPath(m_pathList[0][i], m_pathList[lastLvl][j]));
		}
	}

	for(int i = 0; i < m_pathList.size(); i++){
		for(int j = 0; j < m_pathList[i].size(); j++){
			for(int k = (i == lastLvl+1) ? j+1 : 0; k < m_pathList[lastLvl+1].size(); k++){
				if((m_pathList[i][j] != m_pathList[lastLvl+1][k])
						&& (*(m_pathList[i][j]) == *(m_pathList[lastLvl+1][k]))){
					QPair<int, void*> tmp;
					if(m_pathList[i][j]->distance() < m_pathList[lastLvl+1][k]->distance())
						tmp = QPair<int, void*> (lastLvl+1, m_pathList[lastLvl+1][k]);
					else
						tmp = QPair<int, void*> (i, m_pathList[i][j]);
					if(!toRemove.contains(tmp))
						toRemove.push_back(tmp);
				}
			}
		}
	}

	while(!toRemove.empty()){
		QPair<int, void*> tmp = toRemove.takeFirst();
		m_pathList[tmp.first].removeOne((cPath*) tmp.second);
	}

}

void cPath::order(){
	int pathListSize = m_pathList[0].size();
	for(int i = 0; i < pathListSize; i++)
		for(int j = i; j < pathListSize; j++)
			if(m_pathList[0][j]->distance() < m_pathList[0][i]->distance())
				m_pathList[0].swap(i, j);
}
