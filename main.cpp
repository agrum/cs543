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

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	cCR::setLabel(5);
	cCR::setCapacity(60);
	//cMap map(0, 100, 10);
	cMap map("Map_0_100_10.conf");
	cTraffic traffic(6, 100);
	cNetwork<cCR_CIC> network(map, traffic, 7, 4);

	network.start();

	return a.exec();
}
