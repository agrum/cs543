/*
 * cCR.h
 *
 *  Created on: Nov 12, 2012
 *      Author: agrum
 */

#ifndef CCR_H_
#define CCR_H_

#include "../map/cPath.h"
#include <QThread>

class cCR : public QThread {

public:
	cCR();

	static void setLabel(int);
	void setPathList(const QList<cLink<cCR*> >&);

	float fractDistanceOptimal() { return m_fractDistanceOptimal; }
	float fractDistanceFinal() { return m_fractDistanceFinal; }

	virtual void run() = 0;

protected:
	static int m_label;

	QList<cLink<cCR*> > m_pathList;
	QList<cLink<cCR*> > m_pathListOptimal;
	QList<cLink<cCR*> > m_pathListFinal;
	float m_fractDistanceOptimal;
	float m_fractDistanceFinal;
	int m_phase;
};

#endif /* CCR_H_ */
