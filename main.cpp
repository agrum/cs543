/*
 * main.cpp
 *
 *  Created on: Nov 14, 2012
 *      Author: agrum
 */

#include <QtCore>
#include <QApplication>
#include "src/cNetwork.h"
#include "src/cr/cCR_TMP.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	cMap map(3, 20, 10);
	//cMap map("Map_3_20_10.conf");
	cNetwork<cCR_TMP> network(map);

	return a.exec();
}
