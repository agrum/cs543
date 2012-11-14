/*
 * cCR_ECIC.cpp
 *
 *  Created on: Nov 13, 2012
 *      Author: agrum
 */

#include "cCR_ECIC.h"

int cCR_ECIC::m_extend = 0;

void cCR_ECIC::setExtend(int p_extend){
	m_extend = p_extend;
}

void cCR_ECIC::configureOptimal(){
	m_pathList = cPath::getList(this);
	m_pathListOptimal = m_pathList.mid(0, m_label-1+m_extend);
	for(int i = 0; i < m_label-1; i++)
		m_fractDistanceOptimal += m_pathListOptimal[i]->distance();
}

void cCR_ECIC::configurePhase(){
	QList<int> phaseList;
	QList<cCR_ECIC*> crList;
	QList<cCR_ECIC*> crListTmp;
	QList<QPair<int, float> > crUsed;
	int replica = m_pathListOptimal.size() - m_label + 1;

	for(int i = 0; i < m_label; i++)
		phaseList.push_back(i);

	crList.append(this);
	crUsed.push_back(QPair<int, float>(0, 0));
	for(int i = 0; i < m_pathListOptimal.size(); i++){
		crList.push_back((cCR_ECIC*) m_pathListOptimal[i]->opposite(this));
		crUsed.push_back(QPair<int, float>(i+1, 0));
	}

	if(m_extend != 0){
		for(int i = 0; i < crList.size(); i++){
			for(int j = 0; j < m_pathList.size(); j++){
				//cCR_ECIC* tmp = (cCR_ECIC*) m_pathList[j]->opposite(crList[i]);
				cCR_ECIC* tmp = (cCR_ECIC*) m_pathList[j]->opposite(this);
				for(int k = 0; k < tmp->m_pathListOptimal.size(); k++){
					if(tmp->m_pathListOptimal[k]->opposite(tmp) == crList[i]){
						crUsed[i].second += 1;
					}
				}
			}
		}

		crListTmp = crList;
		crList.clear();
		while(!crUsed.empty()){
			int toRemove;
			int index;
			int used;
			for(int i = 0; i < crUsed.size(); i++){
				toRemove = 0;
				index = 0;
				used = 0;
				if(crUsed[i].second > used){
					toRemove = i;
					index = crUsed[i].first;
					used = crUsed[i].second;
				}
			}
			crUsed.removeAt(toRemove);
			crList.push_front(crListTmp[index]);
		}
	}

	for(int i = 0; i < crList.size(); i++){
		if(crList[i]->m_phase != -1 && phaseList.contains(crList[i]->m_phase))
			phaseList.removeOne(crList.takeAt(i--)->m_phase);
		else if(crList[i]->m_phase != -1 && replica-- == 0){
			m_state = SAVED;
			return;
		}
	}

	for(int i = 0; i < crList.size() && !phaseList.empty(); i++)
		crList[i]->m_phase = phaseList.takeFirst();
	m_state = OPTIMIZED;
}

void cCR_ECIC::configurePhaseLeft(){
	if(m_phase != -1)
		return;

	QList<int> phaseList;

	for(int i = 0; i < m_label; i++)
		phaseList.push_back(i);

	for(int i = 0; i < m_pathList.size() && phaseList.size() > 1; i++){
		cCR_ECIC* tmp = (cCR_ECIC*)m_pathList[i]->opposite(this);
		phaseList.removeOne(tmp->m_phase);
	}

	m_phase = phaseList[0];
}

void cCR_ECIC::configureFinal(){
	QList<int> phaseList;

	for(int i = 0; i < m_label; i++)
		phaseList.push_back(i);

	phaseList.removeOne(m_phase);
	for(int i = 0; i < m_pathList.size() && phaseList.size() > 0; i++){
		cCR_ECIC* tmp = (cCR_ECIC*)m_pathList[i]->opposite(this);
		if(phaseList.contains(tmp->m_phase)){
			m_pathListFinal.push_back(m_pathList[i]);
			phaseList.removeOne(tmp->m_phase);
		}
	}

	for(int i = 0; i < m_pathListFinal.size(); i++){
		m_fractDistanceFinal += m_pathListFinal[i]->distance();
	}

	/*if(m_state == OPTIMIZED)
		qDebug() << "Optimized" << m_fractDistanceFinal/m_fractDistanceOptimal;
	else
		qDebug() << "Saved" << m_fractDistanceFinal/m_fractDistanceOptimal;*/
}
