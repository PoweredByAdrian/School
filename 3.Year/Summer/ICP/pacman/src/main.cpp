/**
* @brief The entry point for the application.
*
* @author Matus Vrablik(xvrabl05)
*
*/

#include "mainwindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	mainwindow w;
	w.show();
	return a.exec();
}
