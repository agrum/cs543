/*
 * cCR_CIC.h
 *
 *  Created on: Nov 15, 2012
 *      Author: agrum
 */

#ifndef CCR_CIC_SLAP_H_
#define CCR_CIC_SLAP_H_

#include "cCR_CIC.h"
#include <QTime>

class cCR_CIC_SLAP : public cCR_CIC {

public:
	cCR_CIC_SLAP();
	cCR_CIC_SLAP(const cCR_CIC_SLAP&);
	cCR_CIC_SLAP& operator=(const cCR_CIC_SLAP&);

	void setPathList(const QList<cLink<cCR*> >&);
	void sendFractDist();
	void receiveFractDist(const cCR_CIC*);

private:
	void LAP();
	QList<int> need();

protected:
	float m_fractDistanceSocial;
	QList<cCR_CIC_SLAP*> m_usingMe;
	static int m_meh;
};

#endif /* CCR_CIC_SLAP_H_ */
