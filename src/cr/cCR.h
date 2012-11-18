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
	enum type {
		NONE,
		INPUT,
		OUTPUT
	};

	cCR();
	cCR(const cCR&);
	cCR& operator=(const cCR&);

	static void setLabel(int);
	static void setCapacity(int);

	void setType(int);
	void setPathList(const QList<cLink<cCR*> >&);

	virtual bool request(const cCR*, const QPair<int, int>&, bool);
	virtual void response(const cCR*, const QPair<int, int>&);

	float fractDistanceOptimal() const { return m_fractDistanceOptimal; }
	float fractDistanceFinal() const { return m_fractDistanceFinal; }
	int type() const { return m_type; }
	bool configured() const { return m_configured; }

	virtual void run() = 0;

protected:
	cCR* crFor(const cCR*);
	cCR* crForPhase(int);

protected:
	static int m_label;
	static int m_capacity;

	QList<cLink<cCR*> > m_pathList;
	QList<cLink<cCR*> > m_pathListOptimal;
	QList<cLink<cCR*> > m_pathListFinal;
	cCR* m_exit;
	QList<QPair<int, int> > m_storage;
	QMutex m_mutex;
	float m_fractDistanceOptimal;
	float m_fractDistanceFinal;
	int m_phase;
	int m_type;
	bool m_configured;
};

#endif /* CCR_H_ */
