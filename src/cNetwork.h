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
	float worstFract = 0;
	float worstFractRatio = 0;
	float averageFract = 0;
	float averageFractRatio = 0;
	int optimized = 0;

	for(int i = 0; i < m_crList.size(); i++){
		while(!m_crList[i].configured()){
			qDebug() << "Configuration in progress";
			sleep(1);
		}

		float fractOptimalTmp = m_crList[i].fractDistanceOptimal();
		float fractFinalTmp = m_crList[i].fractDistanceFinal();
		float fractRatio = fractFinalTmp/fractOptimalTmp;

		averageFract += fractFinalTmp;
		averageFractRatio += fractRatio;
		if(fractFinalTmp == fractOptimalTmp)
			optimized++;
		if(fractFinalTmp > worstFract)
			worstFract = fractFinalTmp;
		if(fractRatio > worstFractRatio)
			worstFractRatio = fractRatio;
	}
	averageFract /= m_crList.size();
	averageFractRatio /= m_crList.size();

	qDebug() << "Optimized CR  :" << optimized;
	qDebug() << "Average ratio :" << averageFract;
	qDebug() << "Average fract :" << averageFractRatio;
	qDebug() << "Worst ratio   :" << worstFractRatio;
	qDebug() << "Worst fract   :" << worstFract;
}

#endif /* CCRLIST_H_ */
