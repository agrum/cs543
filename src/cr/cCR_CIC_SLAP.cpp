/*
 * cCR_CIC.cpp
 *
 *  Created on: Nov 15, 2012
 *      Author: agrum
 */

#include "cCR_CIC_SLAP.h"

cCR_CIC_SLAP::cCR_CIC_SLAP():
cCR_CIC()
{

}

cCR_CIC_SLAP::cCR_CIC_SLAP(const cCR_CIC_SLAP& p_cr):
cCR_CIC(p_cr)
{

}

cCR_CIC_SLAP& cCR_CIC_SLAP::operator=(const cCR_CIC_SLAP& p_cr){
	*(cCR_CIC*) this = p_cr;

	return *this;
}

void cCR_CIC_SLAP::receiveFractDist(const cCR_CIC* p_cr){
	if(((const cCR_CIC_SLAP*)p_cr)->m_fractDistanceSocial < m_fractDistanceSocial){
		m_mutex.lock();
		m_waitingRelease.insert(p_cr);
		m_mutex.unlock();
	}
}

void cCR_CIC_SLAP::LAP(){
	QList<int> phaseList;
	QList<cCR_CIC_SLAP*> crList;

	for(int i = 0; i < m_label; i++)
		phaseList.push_back(i);

	crList.push_back(this);
	for(int i = 0; i < m_pathListOptimal.size(); i++)
		crList.push_back((cCR_CIC_SLAP*) m_pathListOptimal[i].opposite(this));

	for(int i = 0; i < crList.size(); i++){
		if(crList[i]->m_phase != -1 && phaseList.contains(crList[i]->m_phase))
			phaseList.removeOne(crList.takeAt(i--)->m_phase);
		else if(crList[i]->m_phase != -1){
			m_state = SAVED;
			return;
		}
	}

	QMap<int, int> crMap;
	for(int i = 0; i < crList.size(); i++){
		QList<int> phaseNeeded = crList[i]->need();
		int available = 0;
		for(int j = 0; j < phaseNeeded.size(); j++)
			if(phaseList.contains(phaseNeeded[j]))
				available++;
		if(available == 0)
			available = m_label+1;
		crMap.insertMulti(available, i);
	}

	QList<int> crIndex = crMap.values();
	for(int a = 0; a < crIndex.size(); a++){
		int i = crIndex[a];
		QList<int> phaseNeeded = crList[i]->need();
		for(int j = 0; j < phaseNeeded.size() && crList[i]->m_phase == -1; j++){
			if(phaseList.contains(phaseNeeded[j])){
				crList[i]->m_phase = phaseNeeded[j];
				phaseList.removeOne(phaseNeeded[j]);
			}
		}
		if(crList[i]->m_phase == -1)
			crList[i]->m_phase = phaseList.takeFirst();
	}
	m_state = OPTIMIZED;
}

QList<int> cCR_CIC_SLAP::need(){
	QList<int> phaseList;
	QList<cCR_CIC_SLAP*> crList;

	for(int i = 0; i < m_label; i++)
		phaseList.push_back(i);

	crList.push_back(this);
	for(int i = 0; i < m_pathListOptimal.size(); i++)
		crList.push_back((cCR_CIC_SLAP*) m_pathListOptimal[i].opposite(this));

	for(int i = 0; i < crList.size(); i++){
		if(crList[i]->m_phase != -1 && phaseList.contains(crList[i]->m_phase))
			phaseList.removeOne(crList.takeAt(i--)->m_phase);
	}

	return phaseList;
}

void cCR_CIC_SLAP::setPathList(const QList<cLink<cCR*> >& p_linkList){
	m_pathList = p_linkList;
	m_pathListOptimal = m_pathList.mid(0, m_label-1);
	for(int i = 0; i < m_pathListOptimal.size(); i++)
		m_fractDistanceOptimal += m_pathListOptimal[i].distance();
	m_fractDistanceSocial =
			m_fractDistanceOptimal
			- m_pathList[m_pathListOptimal.size()].distance();
}
