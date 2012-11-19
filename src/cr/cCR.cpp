/*
 * cCR.cpp
 *
 *  Created on: Nov 12, 2012
 *      Author: agrum
 */

#include "cCR.h"

int cCR::m_label = 1;

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
			exitFound = true;
		}
	}
}
