/*
 * cCR_CIC.cpp
 *
 *  Created on: Nov 15, 2012
 *      Author: agrum
 */

#include "cCR_CIC_LUP.h"

cCR_CIC_LUP::cCR_CIC_LUP():
m_state(NONE)
{
	for(int i = 0; i < m_label; i++)
		m_phaseWeight.push_back(0);
}

cCR_CIC_LUP::cCR_CIC_LUP(const cCR_CIC_LUP& p_cr):
cCR(p_cr)
{
	m_state = p_cr.m_state;
	m_waitingRelease = p_cr.m_waitingRelease;
	m_waitingEnding = p_cr.m_waitingEnding;
	m_phaseWeight = p_cr.m_phaseWeight;
}

cCR_CIC_LUP& cCR_CIC_LUP::operator=(const cCR_CIC_LUP& p_cr){
	*(cCR*) this = p_cr;
	m_state = p_cr.m_state;
	m_waitingRelease = p_cr.m_waitingRelease;
	m_waitingEnding = p_cr.m_waitingEnding;
	m_phaseWeight = p_cr.m_phaseWeight;

	return *this;
}

void cCR_CIC_LUP::run() {
	for(int i = 0; i < m_pathList.size(); i++)
		m_waitingEnding.insert(m_pathList[i].opposite(this));
	for(int i = 0; i < m_pathListOptimal.size(); i++)
		((cCR_CIC_LUP*) m_pathListOptimal[i].opposite(this))->receiveFractDist(this);
	m_state = FRACTSENT;

	while(m_state != RELEASED){
		msleep(100);
		m_mutex.lock();
		if(m_waitingRelease.empty())
			m_state = RELEASED;
		m_mutex.unlock();
	}

	LAP();
	for(int i = 0; i < m_pathList.size(); i++)
		((cCR_CIC_LUP*) m_pathList[i].opposite(this))->receiveRelease(this);

	while(!m_waitingEnding.empty()){
		msleep(100);
	}

	if(m_state == SAVED)
		LAP2();

	final();
	m_configured = true;
}

void cCR_CIC_LUP::receiveFractDist(const cCR_CIC_LUP* p_cr){
	if(p_cr->m_fractDistanceOptimal < m_fractDistanceOptimal)
		m_waitingRelease.insert(p_cr);
}

void cCR_CIC_LUP::receiveRelease(cCR* p_cr){
	m_mutex.lock();
	m_waitingRelease.remove(p_cr);
	m_waitingEnding.remove(p_cr);
	if(((cCR_CIC_LUP*) p_cr)->m_phase != -1)
		m_phaseWeight[((cCR_CIC_LUP*) p_cr)->m_phase] -= pathTo(p_cr).distance();
	for(int i = 0; i < m_label-1; i++){
		cCR* tmp = ((cCR_CIC_LUP*) p_cr)->m_pathListOptimal[i].opposite(p_cr);
		if(((cCR_CIC_LUP*) tmp)->m_phase != -1)
			m_phaseWeight[((cCR_CIC_LUP*) tmp)->m_phase] -= pathTo(tmp).distance();
	}
	m_mutex.unlock();
}

void cCR_CIC_LUP::LAP(){
	QMap<float, int> phaseMap;
	QList<int> phaseList;
	QList<cCR_CIC_LUP*> crList;

	for(int i = 0; i < m_label; i++)
		phaseMap.insertMulti(m_phaseWeight[i], i);
	phaseList = phaseMap.values();

	crList.push_back(this);
	for(int i = 0; i < m_pathListOptimal.size(); i++)
		crList.push_back((cCR_CIC_LUP*) m_pathListOptimal[i].opposite(this));

	for(int i = 0; i < crList.size(); i++){
		if(crList[i]->m_phase != -1 && phaseList.contains(crList[i]->m_phase))
			phaseList.removeOne(crList.takeAt(i--)->m_phase);
		else if(crList[i]->m_phase != -1){
			m_state = SAVED;
			return;
		}
	}

	for(int i = 0; i < crList.size(); i++)
		crList[i]->m_phase = phaseList.takeFirst();
	m_state = OPTIMIZED;
}

void cCR_CIC_LUP::LAP2(){
	QList<int> phaseList;

	if(m_phase != -1)
		return;

	for(int i = 0; i < m_label; i++)
		phaseList.push_back(i);

	for(int i = 0; i < m_pathList.size() && phaseList.size() > 1; i++){
		cCR_CIC_LUP* tmp = (cCR_CIC_LUP*) m_pathList[i].opposite(this);
		phaseList.removeOne(tmp->m_phase);
	}

	m_phase = phaseList[0];
}

void cCR_CIC_LUP::final(){
	QList<int> phaseList;

	for(int i = 0; i < m_label; i++)
		phaseList.push_back(i);

	phaseList.removeOne(m_phase);
	for(int i = 0; i < m_pathList.size() && phaseList.size() > 0; i++){
		cCR_CIC_LUP* tmp = (cCR_CIC_LUP*)m_pathList[i].opposite(this);
		if(phaseList.contains(tmp->m_phase)){
			m_pathListFinal.push_back(m_pathList[i]);
			m_fractDistanceFinal += m_pathListFinal.last().distance();
			phaseList.removeOne(tmp->m_phase);
		}
	}
}

const cLink<cCR*>& cCR_CIC_LUP::pathTo(const cCR* p_cr) {
	for(int i = 0; i < m_pathList.size(); i++)
		if(m_pathList[i].opposite(this) == p_cr)
			return m_pathList[i];
	return m_pathList[0];
}
