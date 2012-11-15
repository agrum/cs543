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
#include "map/cMap.h"
#include "map/cLink.h"
#include "cr/cCR.h"

template <class T>
class cNetwork {

public:
	cNetwork(const cMap&);

	void configure();
	void result();

private:
	QList<T> m_crList;
};

template <class T>
cNetwork<T>::cNetwork(const cMap& p_map){
	QList<cCR*> crAddrList;

	for(int i = 0; i < p_map.size(); i++){
		m_crList.push_back(T ());
		crAddrList.push_back(&(m_crList.last()));
	}

	for(int i = 0; i < p_map.size(); i++){
		QList<cLink<cCR*> > linkList;
		QList<cPath> pathList = p_map.pathList(i);

		for(int j = 0; j < pathList.size(); j++)
			linkList.push_back(cLink<cCR*> (crAddrList, pathList[j]));

		m_crList[i].setPathList(linkList);
	}
}

template <class T>
void cNetwork<T>::configure(){
	for(int i = 0; i < m_crList.size(); i++)
		m_crList[i].start();
}

template <class T>
void cNetwork<T>::result(){
	float averageFract = 0;
	int optimized = 0;

	for(int i = 0; i < m_crList.size(); i++){
		float fractOptimalTmp = m_crList[i].fractDistanceOptimal();
		float fractFinalTmp = m_crList[i].fractDistanceFinal();
		averageFract += fractFinalTmp/fractOptimalTmp;
		if(fractFinalTmp == fractOptimalTmp)
			optimized++;
	}
	averageFract /= m_crList.size();

	qDebug() << "Average ratio :" << averageFract;
	qDebug() << "Optimized CR :" << optimized;
}

#endif /* CCRLIST_H_ */
