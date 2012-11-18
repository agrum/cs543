/*
 * cRequest.h
 *
 *  Created on: Nov 15, 2012
 *      Author: agrum
 */

#ifndef CREQUEST_H_
#define CREQUEST_H_

#include "../cr/cCR.h"
#include <QThread>
#include <QPair>

class cRequest : public QThread {

public:
	cRequest(cCR*, QPair<int, int>);

	bool inside() const;

	virtual void run();

private:
	cCR* m_input;
	QPair<int, int> m_chunk;
	bool m_in;
};

#endif /* CREQUEST_H_ */
