/*
 * cLink.h
 *
 *  Created on: Nov 12, 2012
 *      Author: agrum
 */

#ifndef CLINK_H_
#define CLINK_H_

#include <QDebug>

class cLink {

public:
	cLink(void*, void*, float);

	void* opposite(void*) const;
	float distance() const;

private:
	void* m_0;
	void* m_1;
	float m_distance;
};

#endif /* CLINK_H_ */
