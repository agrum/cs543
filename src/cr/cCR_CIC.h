/*
 * cCR_CIC.h
 *
 *  Created on: Nov 13, 2012
 *      Author: agrum
 */

#ifndef CCR_CIC_H_
#define CCR_CIC_H_

#include "cCR.h"

class cCR_CIC : public cCR {

public:
	enum state{
		NONE,
		SAVED,
		OPTIMIZED
	};

	cCR_CIC();

	virtual void run();

	void configureOptimal();
	void configurePhase();
	void configurePhaseLeft();
	void configureFinal();

private:
	QList<cCR_CIC*> m_subNet;
	QList<cCR_CIC*> m_neighborhood;
	int m_state;
};

#endif /* CCR_CIC_H_ */
