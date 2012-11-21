/*
 * cCR_CIC.h
 *
 *  Created on: Nov 15, 2012
 *      Author: agrum
 */

#ifndef CCR_CIC_SLAP_H_
#define CCR_CIC_SLAP_H_

#include "cCR.h"
#include <QSet>
#include <QMutex>

class cCR_CIC_SLAP : public cCR {

public:
	enum state{
		NONE,
		RELEASED,
		SAVED,
		OPTIMIZED
	};

	cCR_CIC_SLAP();
	cCR_CIC_SLAP(const cCR_CIC_SLAP&);
	cCR_CIC_SLAP& operator=(const cCR_CIC_SLAP&);

	void setPathList(const QList<cLink<cCR*> >&);

	virtual void run();

	void lock();
	void receiveFractDist(const cCR_CIC_SLAP*);
	void receiveRelease(cCR*);

private:
	void LAP();
	void LAP2();
	void final();
	QList<int> need();

protected:
	float m_fractDistanceSocial;
	int m_state;
	QMutex m_mutex;
	static QMutex m_turn;
	QSet<const cCR*> m_waitingRelease;
	QSet<const cCR*> m_waitingEnding;
};

#endif /* CCR_CIC_SLAP_H_ */
