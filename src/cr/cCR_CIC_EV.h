/*
 * cCR_CIC.h
 *
 *  Created on: Nov 15, 2012
 *      Author: agrum
 */

#ifndef CCR_CIC_EV_H_
#define CCR_CIC_EV_H_

#include "cCR.h"
#include <QSet>
#include <QMutex>

class cCR_CIC_EV : public cCR {

public:
	enum state{
		NONE,
		FRACTSENT,
		RELEASED,
		SAVED,
		OPTIMIZED
	};

	cCR_CIC_EV();
	cCR_CIC_EV(const cCR_CIC_EV&);
	cCR_CIC_EV& operator=(const cCR_CIC_EV&);

	virtual void run();

	void lock();
	void receiveFractDist(const cCR_CIC_EV*);
	void receiveRelease(cCR*);

private:
	void LAP();
	void LAP2();
	void final();
	QList<int> need();

protected:
	int m_state;
	QMutex m_mutex;
	QSet<const cCR*> m_waitingRelease;
	QSet<const cCR*> m_waitingEnding;
};

#endif /* CCR_CIC_EV_H_ */
