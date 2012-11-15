/*
 * cCRList.h
 *
 *  Created on: Nov 13, 2012
 *      Author: agrum
 */

#ifndef CCRLIST_H_
#define CCRLIST_H_

#include <QList>
#include <QVector3D>
#include "map/cMap.h"
#include "map/cLink.h"

template <class T>
class cNetwork {

public:
	cNetwork(const cMap&);
	~cNetwork();

private:
	QList<T*> m_crList;
};

template <class T>
cNetwork<T>::cNetwork(const cMap& p_map){
	for(int i = 0; i < p_map.size(); i++)
		m_crList.push_back(new T);

	for(int i = 0; i < p_map.size(); i++){
		QList<cLink<T*> > linkList;
		QList<cPath> pathList = p_map.pathList(i);

		for(int j = 0; j < pathList.size(); j++){
			cLink<T*> tmp(
					m_crList[i],
					m_crList[pathList[j].opposite(i)],
					pathList[j].distance());
			linkList.push_back(tmp);
		}

		m_crList[i]->setPathList(linkList);
	}
}

template <class T>
cNetwork<T>::~cNetwork (){
	while(!this->empty())
		 delete this->takeFirst();
}

#endif /* CCRLIST_H_ */
