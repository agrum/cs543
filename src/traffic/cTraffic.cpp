/*
 * cTraffic.cpp
 *
 *  Created on: Nov 15, 2012
 *      Author: agrum
 */

#include "cTraffic.h"

cTraffic::cTraffic(int p_stream, int p_chunk):
m_stream(p_stream),
m_chunk(p_chunk)
{

}

QPair<int, int> cTraffic::genTraffic() const {
	return QPair<int, int> ((QTime::currentTime().second()/4)%5, qrand()%m_chunk);
}
