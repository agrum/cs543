/*
 * cCR_ECIC.h
 *
 *  Created on: Nov 13, 2012
 *      Author: agrum
 */

#ifndef CCR_ECIC_H_
#define CCR_ECIC_H_

#include "cCR.h"

class cCR_ECIC : public cCR {

public:
	static void setExtend(int);

	virtual void configureOptimal();
	virtual void configurePhase();
	virtual void configurePhaseLeft();
	virtual void configureFinal();

private:
	static int m_extend;
};

#endif /* CCR_ECIC_H_ */
