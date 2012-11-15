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

class cPath : public cLink<int> {

public:
	cPath(int, int, float);
	cPath(const QDomElement&);
	cPath(const cPath&);
	cPath& operator=(const cPath&);
	bool operator==(const cPath&) const;

	void save(QDomElement&);
	bool merge(cPath);

private:
	bool canMerge(cPath);
};

#endif /* CPATH_H_ */
