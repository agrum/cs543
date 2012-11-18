/*
 * cRequest.cpp
 *
 *  Created on: Nov 15, 2012
 *      Author: agrum
 */

#include "cRequest.h"

cRequest::cRequest(cCR* p_cr, QPair<int, int> p_chunk):
m_input(p_cr),
m_chunk(p_chunk),
m_in(true)
{

}

bool cRequest::inside() const {
	return m_in;
}

void cRequest::run(){
	//qDebug() << "Request ->" << m_chunk;
	m_in = m_input->request(m_input, m_chunk, true);
	//qDebug() << "Request <-" << m_chunk << m_in;
	finished();
}
