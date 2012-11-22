/*
 * main.cpp
 *
 *  Created on: Nov 14, 2012
 *      Author: agrum
 */

#include <QtCore>
#include <QApplication>
#include "src/cNetwork.h"
#include "src/cr/cCR_CIC.h"
#include "src/cr/cCR_CIC_SLAP.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	//cMap map(1, 100, 10);
	cMap map("Map_1_100_10.conf");

	for(int i = 3; i <= 20; i++){
		cCR::setLabel(i);
		qDebug() << "Label" << i;
		for(int j = 0; j < 20; j++){
			cNetwork<cCR_CIC> network(map);
			network.start();
			while(!network.isFinished())
				sleep(1);
		}
	}

	return a.exec();
}
