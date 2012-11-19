/*
 * cCR_CIC.h
 *
 *  Created on: Nov 15, 2012
 *      Author: agrum
 */

#ifndef CCR_CIC_LUP_H_
#define CCR_CIC_LUP_H_

#include "cCR.h"
#include <QSet>
#include <QMutex>

class cCR_CIC_LUP : public cCR {

public:
	enum state{
		NONE,
		FRACTSENT,
		RELEASED,
		SAVED,
		OPTIMIZED
	};

	cCR_CIC_LUP();
	cCR_CIC_LUP(const cCR_CIC_LUP&);
	cCR_CIC_LUP& operator=(const cCR_CIC_LUP&);

	virtual void run();

	void receiveFractDist(const cCR_CIC_LUP*);
	void receiveRelease(cCR*);

private:
	void LAP();
	void LAP2();
	void final();
	const cLink<cCR*>& pathTo(const cCR*);

protected:
	int m_state;
	QMutex m_mutex;
	QSet<const cCR*> m_waitingRelease;
	QSet<const cCR*> m_waitingEnding;
	QList<float> m_phaseWeight;
};

#endif /* CCR_CIC_LUP_H_ */
