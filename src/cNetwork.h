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
#include "traffic/cTraffic.h"
#include "traffic/cRequest.h"

template <class T>
class cNetwork : public QThread {

public:
	cNetwork(const cMap&, const cTraffic&, int, int);

	void run();

private:
	void configure();
	void result();
	void genTraffic();

private:
	QList<T> m_crList;
	QList<cCR*> m_crInput;

	const cTraffic& m_traffic;
};

template <class T>
cNetwork<T>::cNetwork(const cMap& p_map, const cTraffic& p_traffic, int p_in, int p_out):
m_traffic(p_traffic)
{
	QList<cCR*> crAddrList;
	QList<cCR*> crOutput;

	//Init CR List
	for(int i = 0; i < p_map.size(); i++){
		m_crList.push_back(T ());
		crAddrList.push_back(&(m_crList.last()));
	}

	//Init input/output CRs
	qsrand(0);
	for(int i = 0; i < p_in; i++){
		int index = qrand()%m_crList.size();
		while(m_crInput.contains(&m_crList[index]))
			index = qrand()%m_crList.size();
		m_crList[index].setType(cCR::INPUT);
		m_crInput.push_back(&m_crList[index]);
	}
	for(int i = 0; i < p_out; i++){
		int index = qrand()%m_crList.size();
		while(crOutput.contains(&m_crList[index]) || m_crInput.contains(&m_crList[index]))
			index = qrand()%m_crList.size();
		m_crList[index].setType(cCR::OUTPUT);
		crOutput.push_back(&m_crList[index]);
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
	//genTraffic();
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

template <class T>
void cNetwork<T>::genTraffic(){
	QList<QList<cRequest*> > requestList;
	float sampleMaxSize = 20;
	float sampleSize = 0;
	float stayedIn = 0;

	qsrand(0);
	while(true){
		int index = qrand()%m_crInput.size();
		QPair<int, int> stream = m_traffic.genTraffic();
		QList<cRequest*> requestFlow;

		//New bunch of requests
		for(int i = 0; i < stream.second; i++){
			cRequest* request = new cRequest(
					m_crInput[index],
					QPair<int, int> (stream.first, i));
			requestFlow.push_back(request);
		}
		if(requestFlow.size() > 0){
			requestFlow[0]->start();
			requestList.push_back(requestFlow);
		}

		//Manage old requests
		for(int i = 0; i < requestList.size(); i++){
			QList<cRequest*>& requestFlowTmp = requestList[i];
			if(requestFlowTmp[0]->isFinished()){
				sampleSize++;
				if(requestFlowTmp[0]->inside())
					stayedIn++;
				if(sampleSize == sampleMaxSize){
					qDebug() << (int) (100.0*stayedIn/sampleSize);
					sampleSize = 0;
					stayedIn = 0;
				}
				delete requestFlowTmp.takeFirst();
				if(requestFlowTmp.size() > 0)
					requestFlowTmp[0]->start();
				else
					requestList.removeAt(i--);
			}
		}

		msleep(100);
	}
}

#endif /* CCRLIST_H_ */
