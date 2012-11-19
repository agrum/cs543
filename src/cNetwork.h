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
class cNetwork : public QThread {

public:
	cNetwork(const cMap&);

	void run();

private:
	void configure();
	void result();
	void genTraffic();

private:
	QList<T> m_crList;
};

template <class T>
cNetwork<T>::cNetwork(const cMap& p_map)
{
	QList<cCR*> crAddrList;

	//Init CR List
	for(int i = 0; i < p_map.size(); i++){
		m_crList.push_back(T ());
		crAddrList.push_back(&(m_crList.last()));
	}

	//Init pathes from map
	for(int i = 0; i < p_map.size(); i++){
		QList<cLink<cCR*> > linkList;
		QList<cPath> pathList = p_map.pathList(i);

		for(int j = 0; j < pathList.size(); j++)
			linkList.push_back(cLink<cCR*> (crAddrList, pathList[j]));

		m_crList[i].setPathList(linkList);
	}
}


template <class T>
void cNetwork<T>::run(){
	configure();
	result();
}

template <class T>
void cNetwork<T>::configure(){
	for(int i = 0; i < m_crList.size(); i++)
		m_crList[i].lock();
	for(int i = 0; i < m_crList.size(); i++)
		m_crList[i].start();
}

template <class T>
void cNetwork<T>::result(){
	float worstFract = 0;
	float worstFractRatio = 0;
	float averageFract = 0;
	float averageFractRatio = 0;
	float optimalFract = 0;
	int optimized = 0;

	for(int i = 0; i < m_crList.size(); i++){
		while(!m_crList[i].configured()){
			//qDebug() << "Configuration in progress";
			sleep(1);
		}

		float fractOptimalTmp = m_crList[i].fractDistanceOptimal();
		float fractFinalTmp = m_crList[i].fractDistanceFinal();
		float fractRatio = fractFinalTmp/fractOptimalTmp;

		optimalFract += fractOptimalTmp;
		averageFract += fractFinalTmp;
		averageFractRatio += fractRatio;
		if(fractFinalTmp == fractOptimalTmp)
			optimized++;
		if(fractFinalTmp > worstFract)
			worstFract = fractFinalTmp;
		if(fractRatio > worstFractRatio)
			worstFractRatio = fractRatio;
	}
	optimalFract /= m_crList.size();
	averageFract /= m_crList.size();
	averageFractRatio /= m_crList.size();

	qDebug() << averageFractRatio;
	//qDebug() << "Optimized CR  :" << optimized;
	//qDebug() << "Optimial fract  :" << optimalFract;
	//qDebug() << "Average ratio :" << averageFractRatio;
	//qDebug() << "Average fract :" << averageFract;
	//qDebug() << "Worst ratio   :" << worstFractRatio;
	//qDebug() << "Worst fract   :" << worstFract;
}

#endif /* CCRLIST_H_ */
