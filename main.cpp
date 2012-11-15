/*
 * main.cpp
 *
 *  Created on: Nov 14, 2012
 *      Author: agrum
 */

#include <QtCore>
#include <QApplication>
#include "src/cNetwork.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	cMap map(0, 100, 10);

	return a.exec();
}
