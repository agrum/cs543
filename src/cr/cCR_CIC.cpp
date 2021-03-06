/*
 * cCR_CIC.cpp
 *
 *  Created on: Nov 15, 2012
 *      Author: agrum
 */

#include "cCR_CIC.h"

cCR_CIC::cCR_CIC():
m_state(NONE)
{

}

cCR_CIC::cCR_CIC(const cCR_CIC& p_cr):
cCR(p_cr)
{
	m_state = p_cr.m_state;
	m_waitingRelease = p_cr.m_waitingRelease;
	m_waitingEnding = p_cr.m_waitingEnding;
}

cCR_CIC& cCR_CIC::operator=(const cCR_CIC& p_cr){
	*(cCR*) this = p_cr;
	m_state = p_cr.m_state;
	m_waitingRelease = p_cr.m_waitingRelease;
	m_waitingEnding = p_cr.m_waitingEnding;

	return *this;
}

void cCR_CIC::run() {
	for(int i = 0; i < m_pathList.size(); i++)
		m_waitingEnding.insert(m_pathList[i].opposite(this));

	m_crList += this;
	for(int i = 0; i < m_pathListOptimal.size(); i++)
		m_crList += (cCR_CIC*) m_pathList[i].opposite(this);

	m_mutex.unlock();
	sendFractDist();

	for(int i = 0; i < m_pathList.size(); i++){
		if(((cCR_CIC*) m_pathList[i].opposite(this))->m_state < FDSENT){
			i--;
			msleep(10);
		}
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
		((cCR_CIC*) m_pathList[i].opposite(this))->receiveRelease(this);

	bool wait = true;
	while(wait){
		m_mutex.lock();
		wait = !m_waitingEnding.empty();
		m_mutex.unlock();
		msleep(10);
	}

	LAP2();

	final();
	m_configured = true;
}

bool cCR_CIC::optimized(){
	return (m_state == OPTIMIZED);
}

void cCR_CIC::lock(){
	m_mutex.lock();
}

void cCR_CIC::unlock(){
	m_mutex.unlock();
}

void cCR_CIC::sendFractDist(){
	for(int i = 0; i < m_pathListOptimal.size(); i++){
		cCR_CIC* tmp = (cCR_CIC*) m_pathListOptimal[i].opposite(this);
		tmp->receiveFractDist(this);
		//receiveFractDist(tmp);
		for(int j = 0; j < tmp->m_pathListOptimal.size(); j++){
			((cCR_CIC*) m_pathListOptimal[j].opposite(tmp))->receiveFractDist(this);
			//receiveFractDist((cCR_CIC*) m_pathListOptimal[j].opposite(tmp));
		}
	}
	m_state = FDSENT;
}

void cCR_CIC::receiveFractDist(const cCR_CIC* p_cr){
	if(((cCR_CIC*) p_cr)->m_fractDistanceOptimal < m_fractDistanceOptimal
			||
			(((cCR_CIC*) p_cr)->m_fractDistanceOptimal == m_fractDistanceOptimal
					&& (int) p_cr < (int) this)){
		m_mutex.lock();
		m_waitingRelease.insert(p_cr);
		m_mutex.unlock();
	}
}

void cCR_CIC::receiveRelease(const cCR* p_cr){
	m_mutex.lock();
	m_waitingRelease.remove(p_cr);
	m_waitingEnding.remove(p_cr);
	m_mutex.unlock();
}

void cCR_CIC::LAP(){
	QList<int> phaseList;
	QList<cCR_CIC*> crList;

	for(int i = 0; i < m_label; i++)
		phaseList.push_back(i);

	crList.push_back(this);
	for(int i = 0; i < m_pathListOptimal.size(); i++)
		crList.push_back((cCR_CIC*) m_pathListOptimal[i].opposite(this));

	for(int i = 0; i < crList.size(); i++){
		if(crList[i]->m_phase != -1 && phaseList.contains(crList[i]->m_phase))
			phaseList.removeOne(crList.takeAt(i--)->m_phase);
		else if(crList[i]->m_phase != -1){
			m_state = PAPED;
			return;
		}
	}

	usleep(400); //

	for(int i = 0; i < crList.size(); i++)
		crList[i]->m_phase = phaseList.takeFirst();
	m_state = PAPED;
	m_state = OPTIMIZED;
}

void cCR_CIC::LAP2(){
	QList<int> phaseList;

	if(m_phase != -1)
		return;

	for(int i = 0; i < m_label; i++)
		phaseList.push_back(i);

	for(int i = 0; i < m_pathList.size() && phaseList.size() > 1; i++){
		cCR_CIC* tmp = (cCR_CIC*) m_pathList[i].opposite(this);
		phaseList.removeOne(tmp->m_phase);
	}

	m_phase = phaseList[0];
}

void cCR_CIC::final(){
	QList<int> phaseList;

	for(int i = 0; i < m_label; i++)
		phaseList.push_back(i);

	phaseList.removeOne(m_phase);
	int i = 0;
	for(; i < m_pathList.size() && phaseList.size() > 0; i++){
		cCR_CIC* tmp = (cCR_CIC*)m_pathList[i].opposite(this);
		if(phaseList.contains(tmp->m_phase)){
			m_pathListFinal.push_back(m_pathList[i]);
			m_fractDistanceFinal += m_pathListFinal.last().distance();
			phaseList.removeOne(tmp->m_phase);
		}
	}
	/*if(i == m_label)
		m_state = OPTIMIZED;
	else if(i != m_label)
		m_state = SAVED;*/
}
