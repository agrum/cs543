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
m_state(0)
{

}

void cCR::setLabel(int p_label){
	m_label = p_label;
}

void cCR::zero(){
	m_pathList.clear();
	m_pathListOptimal.clear();
	m_pathListFinal.clear();
	m_fractDistanceOptimal = 0;
	m_fractDistanceFinal = 0;
	m_phase = -1;
	m_state = NONE;
}

void cCR::setPathList(){
	m_pathList = cPath::getList(this);
}
