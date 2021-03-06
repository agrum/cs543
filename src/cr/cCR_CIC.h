/*
 * cCR_CIC.h
 *
 *  Created on: Nov 15, 2012
 *      Author: agrum
 */

#ifndef CCR_CIC_H_
#define CCR_CIC_H_

#include "cCR.h"
#include <QList>
#include <QMutex>

class cCR_CIC : public cCR {

public:
	enum state{
		NONE,
		FDSENT,
		RELEASED,
		PAPED,
		SAVED,
		OPTIMIZED
	};

	cCR_CIC();
	cCR_CIC(const cCR_CIC&);
	cCR_CIC& operator=(const cCR_CIC&);

	virtual void run();

	bool optimized();
	void lock();
	void unlock();
	virtual void sendFractDist();
	virtual void receiveFractDist(const cCR_CIC*);
	void receiveRelease(const cCR*);

private:
	virtual void LAP();
	void LAP2();
	void final();

protected:
	int m_state;
	QMutex m_mutex;
	QList<cCR_CIC*> m_crList;
	QSet<const cCR*> m_waitingRelease;
	QSet<const cCR*> m_waitingEnding;
};

#endif /* CCR_CIC_H_ */
