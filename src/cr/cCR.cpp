/*
 * cCR.cpp
 *
 *  Created on: Nov 12, 2012
 *      Author: agrum
 */

#include "cCR.h"

int cCR::m_label = 1;

cCR::cCR():
m_fractDistanceOptimal(0),
m_fractDistanceFinal(0),
m_phase(-1),
m_configured(false)
{

}

cCR::cCR(const cCR& p_cr):
m_pathList(p_cr.m_pathList),
m_pathListOptimal(p_cr.m_pathListOptimal),
m_pathListFinal(p_cr.m_pathListFinal),
m_fractDistanceOptimal(p_cr.m_fractDistanceOptimal),
m_fractDistanceFinal(p_cr.m_fractDistanceFinal),
m_phase(p_cr.m_phase),
m_configured(false)
{

}

cCR& cCR::operator=(const cCR& p_cr){
	m_pathList = p_cr.m_pathList;
	m_pathListOptimal = p_cr.m_pathListOptimal;
	m_pathListFinal = p_cr.m_pathListFinal;
	m_fractDistanceOptimal = p_cr.m_fractDistanceOptimal;
	m_fractDistanceFinal = p_cr.m_fractDistanceFinal;
	m_phase = p_cr.m_phase;
	m_configured = p_cr.m_configured;
}

void cCR::setLabel(int p_label){
	m_label = p_label;
}

void cCR::setPathList(const QList<cLink<cCR*> >& p_linkList){
	m_pathList = p_linkList;
	m_pathListOptimal = m_pathList.mid(0, m_label-1);
	for(int i = 0; i < m_pathListOptimal.size(); i++)
		m_fractDistanceOptimal += m_pathListOptimal[i].distance();
}
