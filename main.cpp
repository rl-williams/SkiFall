/* File: main.cpp
 * Author: Becky Williams and Harmony Li
 * Desc: Main file to start off SkiFall
 */

#include <QtGui>
#include "skifall.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    SkiFall w;
    w.setFixedHeight(500);
    w.setFixedWidth(400);
    w.show();

    return app.exec();
}
