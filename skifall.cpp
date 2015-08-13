/* File: skifall.cpp
 * Author: Becky Williams and Harmony Li
 * Desc: Defines the SkiFall game
 */

#include <QtGui>
#include "skifall.h"
#include "skifall_canvas.h"

SkiFall::SkiFall()
{
    setWindowTitle("SkiFall!");
    canvas = new SkiFallCanvas(this);
}
