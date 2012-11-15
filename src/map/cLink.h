/*
 * cLink.h
 *
 *  Created on: Nov 12, 2012
 *      Author: agrum
 */

#ifndef CLINK_H_
#define CLINK_H_

#include <QDebug>

template <class T>
class cLink {

public:
	cLink();
	cLink(T, T, float);
	cLink(const QList<T>&, const cLink<int>&);
	cLink(const cLink&);
	cLink& operator=(const cLink&);
	bool operator==(const cLink&) const;

	const QList<T>& nodeList() const;
	const T& opposite(const T&) const;
	float distance() const;

protected:
	QList<T> m_nodeList;
	float m_distance;
};

template <class T>
cLink<T>::cLink(){
	m_distance = 0;
}

template <class T>
cLink<T>::cLink(T p_0, T p_1, float p_distance)
{
	m_nodeList.append(p_0);
	m_nodeList.append(p_1);
	m_distance = p_distance;
}

template <class T>
cLink<T>::cLink(const QList<T>& p_nodeList, const cLink<int>& p_link)
{
	for(int i = 0; i < p_link.nodeList().size(); i++)
		m_nodeList.append(p_nodeList[p_link.nodeList()[i]]);
	m_distance = p_link.distance();
}

template <class T>
cLink<T>::cLink(const cLink& p_link) :
m_nodeList(p_link.m_nodeList),
m_distance(p_link.m_distance)
{

}

template <class T>
cLink<T>& cLink<T>::operator=(const cLink& p_link){
	m_nodeList = p_link.m_nodeList;
	m_distance = p_link.m_distance;

	return *this;
}

template <class T>
bool cLink<T>::operator==(const cLink& p_path) const{
	return ((m_nodeList.first() == p_path.m_nodeList.first()
			&& m_nodeList.last() == p_path.m_nodeList.last())
			||
			(m_nodeList.first() == p_path.m_nodeList.last()
			&& m_nodeList.last() == p_path.m_nodeList.first()));
}

template <class T>
const QList<T>& cLink<T>::nodeList() const{
	return m_nodeList;
}

template <class T>
const T& cLink<T>::opposite(const T& p_01) const {
	if(p_01 == m_nodeList.first())
		return m_nodeList.last();
	else if(p_01 == m_nodeList.last())
		return m_nodeList.first();
	return p_01;
}

template <class T>
float cLink<T>::distance() const{
	return m_distance;
}

#endif /* CLINK_H_ */
