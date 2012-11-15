/*
 * cCR.cpp
 *
 *  Created on: Nov 12, 2012
 *      Author: agrum
 */

#include "cCR.h"

int cCR::m_label = 1;
int cCR::m_capacity = 0;

cCR::cCR():
m_exit(NULL),
m_fractDistanceOptimal(0),
m_fractDistanceFinal(0),
m_phase(-1),
m_type(NONE),
m_configured(false)
{

}

cCR::cCR(const cCR& p_cr):
QThread(),
m_pathList(p_cr.m_pathList),
m_pathListOptimal(p_cr.m_pathListOptimal),
m_pathListFinal(p_cr.m_pathListFinal),
m_exit(p_cr.m_exit),
m_fractDistanceOptimal(p_cr.m_fractDistanceOptimal),
m_fractDistanceFinal(p_cr.m_fractDistanceFinal),
m_phase(p_cr.m_phase),
m_type(NONE),
m_configured(false)
{

}

cCR& cCR::operator=(const cCR& p_cr){
	m_pathList = p_cr.m_pathList;
	m_pathListOptimal = p_cr.m_pathListOptimal;
	m_pathListFinal = p_cr.m_pathListFinal;
	m_exit = p_cr.m_exit;
	m_fractDistanceOptimal = p_cr.m_fractDistanceOptimal;
	m_fractDistanceFinal = p_cr.m_fractDistanceFinal;
	m_phase = p_cr.m_phase;
	m_type = p_cr.m_type;
	m_configured = p_cr.m_configured;

	return *this;
}

void cCR::setLabel(int p_label){
	m_label = p_label;
}

void cCR::setCapacity(int p_capacity){
	m_capacity = p_capacity;
}

void cCR::setType(int p_type){
	m_type = p_type;
}

void cCR::setPathList(const QList<cLink<cCR*> >& p_linkList){
	bool exitFound = (m_type == OUTPUT);

	m_pathList = p_linkList;
	m_pathListOptimal = m_pathList.mid(0, m_label-1);
	for(int i = 0; i < m_pathListOptimal.size(); i++)
		m_fractDistanceOptimal += m_pathListOptimal[i].distance();

	for(int i = 0; i < m_pathList.size() && !exitFound; i++){
		if(m_pathList[i].opposite(this)->type() == OUTPUT){
			QList<cCR*> pathToExit = m_pathList[i].nodeList();
			if(pathToExit[0] == this)
				m_exit = pathToExit[1];
			else
				m_exit = pathToExit[pathToExit.size()-2];
		}
	}
}

bool cCR::request(const cCR* p_input, const QPair<int, int>& p_chunk, bool p_in){
	if(p_in){ //Search in the network
		if(p_chunk.second%m_label == m_phase) //Good phase, look for chunk
			if(m_storage.contains(p_chunk)){
				m_storage.removeOne(p_chunk);
				m_storage.push_back(p_chunk);
				crFor(p_input)->response(p_input, p_chunk);
				return true;
			}
			else{
				return m_exit->request(p_input, p_chunk, false);
			}
		else //Find good phase in network
			return crForPhase(p_chunk.second%m_label)->request(p_input, p_chunk, p_in);
	}
	else{ //Not found in network, go find outside
		if(!m_exit) //It's the exit, so return the response
			crFor(p_input)->response(p_input, p_chunk);
		else //Continue to the exit
			m_exit->request(p_input, p_chunk, p_in);
		return false;
	}
}

void cCR::response(const cCR* p_input, const QPair<int, int>& p_chunk){
	if(p_chunk.second%m_label == m_phase){
		m_storage.removeOne(p_chunk);
		m_storage.push_back(p_chunk);
	}
	if(this != p_input)
		crFor(p_input)->response(p_input, p_chunk);
}

cCR* cCR::crFor(const cCR* p_target) {
	for(int i = 0; i < m_pathList.size(); i++){
		if(m_pathList[i].opposite(this) == p_target){
			QList<cCR*> pathToTarget = m_pathList[i].nodeList();
			if(pathToTarget[0] == this)
				return pathToTarget[1];
			else
				return pathToTarget[pathToTarget.size()-2];
		}
	}
	return NULL;
}

cCR* cCR::crForPhase(int p_phase) {
	for(int i = 0; i < m_pathListFinal.size(); i++){
		if(m_pathListFinal[i].opposite(this)->m_phase == p_phase){
			QList<cCR*> pathToTarget = m_pathListFinal[i].nodeList();
			if(pathToTarget[0] == this)
				return pathToTarget[1];
			else
				return pathToTarget[pathToTarget.size()-2];
		}
	}
	return NULL;
}
