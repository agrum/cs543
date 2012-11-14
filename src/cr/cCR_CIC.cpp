/*
 * cCR_CIC.cpp
 *
 *  Created on: Nov 13, 2012
 *      Author: agrum
 */

#include "cCR_CIC.h"

cCR_CIC::cCR_CIC():
m_confLvl(0)
{

}

void cCR_CIC::run(){
	//Set N() and d
	configureOptimal();

	for(int i = 0; i < m_subNet.size(); i++)
		for(int j = 0; j < m_subNet[i]->m_pathListOptimal.size(); j++)
			if(!m_neighborhood.contains(m_subNet[i]->m_pathListOptimal[j]->opposite(m_subNet[i])))
				m_neighborhood.append(m_subNet[i]->m_pathListOptimal[j]->opposite(m_subNet[i]));
}

void cCR_CIC::configureOptimal(){
	m_pathListOptimal = m_pathList.mid(0, m_label-1);
	for(int i = 0; i < m_pathListOptimal.size(); i++){
		m_subNet.push_back(m_pathListOptimal[i]->opposite(this));
		m_neighborhood.push_back(m_pathListOptimal[i]->opposite(this));
		m_fractDistanceOptimal += m_pathListOptimal[i]->distance();
	}
}

void cCR_CIC::configurePhase(){
	QList<int> phaseList;
	QList<cCR_CIC*> crList;

	for(int i = 0; i < m_label; i++)
		phaseList.push_back(i);

	crList.append(this);
	for(int i = 0; i < m_pathListOptimal.size(); i++)
		crList.push_back((cCR_CIC*) m_pathListOptimal[i]->opposite(this));

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

void cCR_CIC::configurePhaseLeft(){
	if(m_phase != -1)
		return;

	QList<int> phaseList;

	for(int i = 0; i < m_label; i++)
		phaseList.push_back(i);

	for(int i = 0; i < m_pathList.size() && phaseList.size() > 1; i++){
		cCR_CIC* tmp = (cCR_CIC*)m_pathList[i]->opposite(this);
		phaseList.removeOne(tmp->m_phase);
	}

	m_phase = phaseList[0];
}

void cCR_CIC::configureFinal(){
	QList<int> phaseList;

	for(int i = 0; i < m_label; i++)
		phaseList.push_back(i);

	phaseList.removeOne(m_phase);
	for(int i = 0; i < m_pathList.size() && phaseList.size() > 0; i++){
		cCR_CIC* tmp = (cCR_CIC*)m_pathList[i]->opposite(this);
		if(phaseList.contains(tmp->m_phase)){
			m_pathListFinal.push_back(m_pathList[i]);
			phaseList.removeOne(tmp->m_phase);
		}
	}

	for(int i = 0; i < m_pathListFinal.size(); i++){
		m_fractDistanceFinal += m_pathListFinal[i]->distance();
	}

	if(m_state == OPTIMIZED)
		qDebug() << "Optimized" << m_fractDistanceFinal/m_fractDistanceOptimal;
	else
		qDebug() << "Saved" << m_fractDistanceFinal/m_fractDistanceOptimal;
}
