/*
 * cCR_CIC.cpp
 *
 *  Created on: Nov 15, 2012
 *      Author: agrum
 */

#include "cCR_CIC_SLAP.h"

QMutex cCR_CIC_SLAP::m_turn;

cCR_CIC_SLAP::cCR_CIC_SLAP():
m_state(NONE)
{

}

cCR_CIC_SLAP::cCR_CIC_SLAP(const cCR_CIC_SLAP& p_cr):
cCR(p_cr)
{
	m_state = p_cr.m_state;
	m_waitingRelease = p_cr.m_waitingRelease;
	m_waitingEnding = p_cr.m_waitingEnding;
}

cCR_CIC_SLAP& cCR_CIC_SLAP::operator=(const cCR_CIC_SLAP& p_cr){
	*(cCR*) this = p_cr;
	m_state = p_cr.m_state;
	m_waitingRelease = p_cr.m_waitingRelease;
	m_waitingEnding = p_cr.m_waitingEnding;

	return *this;
}

void cCR_CIC_SLAP::run() {
	for(int i = 0; i < m_pathList.size(); i++)
		m_waitingEnding.insert(m_pathList[i].opposite(this));
	m_mutex.unlock();
	/*for(int i = 0; i < m_pathListOptimal.size(); i++){
		cCR_CIC_SLAP* tmp = (cCR_CIC_SLAP*) m_pathListOptimal[i].opposite(this);
		tmp->receiveFractDist(this);
		for(int j = 0; j < tmp->m_pathListOptimal.size(); j++){
			((cCR_CIC_SLAP*) m_pathListOptimal[j].opposite(tmp))->receiveFractDist(this);
		}
	}*/
	for(int i = 0; i < m_pathListOptimal.size(); i++){
		cCR_CIC_SLAP* tmp = (cCR_CIC_SLAP*) m_pathListOptimal[i].opposite(this);
		tmp->receiveFractDist(this);
	}

	while(m_state != RELEASED){
		msleep(100);
		m_mutex.lock();
		if(m_waitingRelease.empty())
			m_state = RELEASED;
		m_mutex.unlock();
	}

	LAP();
	for(int i = 0; i < m_pathList.size(); i++)
		((cCR_CIC_SLAP*) m_pathList[i].opposite(this))->receiveRelease(this);

	while(!m_waitingEnding.empty()){
		msleep(10);
	}

	if(m_state == SAVED)
		LAP2();

	final();
	m_configured = true;
}

void cCR_CIC_SLAP::lock(){
	m_mutex.lock();
}

void cCR_CIC_SLAP::receiveFractDist(const cCR_CIC_SLAP* p_cr){
	if(p_cr->m_fractDistanceSocial < m_fractDistanceSocial){
		m_mutex.lock();
		m_waitingRelease.insert(p_cr);
		m_mutex.unlock();
	}
}

void cCR_CIC_SLAP::receiveRelease(cCR* p_cr){
	m_mutex.lock();
	m_waitingRelease.remove(p_cr);
	m_waitingEnding.remove(p_cr);
	m_mutex.unlock();
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

void cCR_CIC_SLAP::LAP2(){
	QList<int> phaseList;

	if(m_phase != -1)
		return;

	for(int i = 0; i < m_label; i++)
		phaseList.push_back(i);

	for(int i = 0; i < m_pathList.size() && phaseList.size() > 1; i++){
		cCR_CIC_SLAP* tmp = (cCR_CIC_SLAP*) m_pathList[i].opposite(this);
		phaseList.removeOne(tmp->m_phase);
	}

	m_phase = phaseList[0];
}

void cCR_CIC_SLAP::final(){
	QList<int> phaseList;

	for(int i = 0; i < m_label; i++)
		phaseList.push_back(i);

	phaseList.removeOne(m_phase);
	for(int i = 0; i < m_pathList.size() && phaseList.size() > 0; i++){
		cCR_CIC_SLAP* tmp = (cCR_CIC_SLAP*)m_pathList[i].opposite(this);
		if(phaseList.contains(tmp->m_phase)){
			m_pathListFinal.push_back(m_pathList[i]);
			m_fractDistanceFinal += m_pathListFinal.last().distance();
			phaseList.removeOne(tmp->m_phase);
		}
	}
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
