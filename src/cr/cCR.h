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
	enum state{
		NONE,
		SAVED,
		OPTIMIZED
	};

	cCR();

	static void setLabel(int);

	void zero();
	void setPathList();

	float fractDistanceOptimal() { return m_fractDistanceOptimal; }
	float fractDistanceFinal() { return m_fractDistanceFinal; }

	virtual void run() = 0;

protected:
	static int m_label;

	QList<cPath* > m_pathList;
	QList<cPath* > m_pathListOptimal;
	QList<cPath* > m_pathListFinal;
	float m_fractDistanceOptimal;
	float m_fractDistanceFinal;
	int m_phase;
	int m_state;
};

#endif /* CCR_H_ */
