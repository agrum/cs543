/*
 * cLink.cpp
 *
 *  Created on: Nov 12, 2012
 *      Author: agrum
 */

#include "cLink.h"

cLink::cLink(void* p_0, void* p_1, float p_distance) :
m_0(p_0),
m_1(p_1),
m_distance(p_distance)
{

}

void* cLink::opposite(void* p_01) const {
	return (p_01 == m_0) ? m_1 : m_0;
}

float cLink::distance() const{
	return m_distance;
}
