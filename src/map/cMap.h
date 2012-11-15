/*
 * cMap.h
 *
 *  Created on: Nov 14, 2012
 *      Author: agrum
 */

#ifndef CMAP_H_
#define CMAP_H_

#include "cPath.h"
#include <QVector3D>
#include <QMap>
#include <QFile>

class cMap {

public:
	cMap(unsigned int, unsigned int, unsigned int);
	cMap(const QString&);

	int size() const;
	QList<cPath> pathList(int) const;

private:
	QList<cPath> expand(const QList<cPath>&, const QList<cPath>&);
	void save(const QString&);

private:
	int m_nodes;
	QList<cPath> m_pathList;
};

#endif /* CMAP_H_ */
