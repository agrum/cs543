/*
 * cPath.h
 *
 *  Created on: Nov 12, 2012
 *      Author: agrum
 */

#ifndef CPATH_H_
#define CPATH_H_

#include "cLink.h"
#include <QPair>
#include <QDomDocument>

class cPath : public QList<void*> {

public:
	float distance() const;
	float level() const;
	void* opposite(void*) const;

	static void init(unsigned int, const QList<cLink*>&);
	static void init(const QList<QPair<float, QList<void*> > >&);
	static QList<cPath*> getList(void*);
	static void clear();

private:
	cPath(float, QList<void*>);
	cPath(const cLink*);
	cPath(const cPath*, const cPath*);

	bool canJoin(const cPath*);
	bool operator==(const cPath&) const;
	static void expand();
	static void order();

private:
	static QList<QList<cPath*> > m_pathList;

	float m_distance;
	int m_lvl;
};

#endif /* CPATH_H_ */
