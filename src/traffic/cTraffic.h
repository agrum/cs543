/*
 * cTraffic.h
 *
 *  Created on: Nov 15, 2012
 *      Author: agrum
 */

#ifndef CTRAFFIC_H_
#define CTRAFFIC_H_

#include <QPair>

class cTraffic {

public:
	cTraffic(int, int);

	QPair<int, int> genTraffic() const;

private:
	int m_stream;
	int m_chunk;
};

#endif /* CTRAFFIC_H_ */
