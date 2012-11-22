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
#include <QMutex>

class cCR : public QThread {

public:

	cCR();
	cCR(const cCR&);
	cCR& operator=(const cCR&);

	static void setLabel(int);
	static int label() { return m_label; }

	virtual void setPathList(const QList<cLink<cCR*> >&);

	float fractDistanceOptimal() const { return m_fractDistanceOptimal; }
	float fractDistanceFinal() const { return m_fractDistanceFinal; }
	bool configured() const { return m_configured; }

	virtual void run() = 0;

protected:
	static int m_label;

	QList<cLink<cCR*> > m_pathList;
	QList<cLink<cCR*> > m_pathListOptimal;
	QList<cLink<cCR*> > m_pathListFinal;
	float m_fractDistanceOptimal;
	float m_fractDistanceFinal;
	int m_phase;
	bool m_configured;
};

#endif /* CCR_H_ */
