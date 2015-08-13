/* File: pong.h
 * Author: Becky Williams and Harmony Li
 * Desc: Header file for SkiFall window.
 */

#ifndef SKIFALL_H
#define SKIFALL_H

#include <QtGui>

//Forward class declaration.
class SkiFallCanvas;

// Manages the window, a container for the SkiFallCanvas (where all the game logic lives).
class SkiFall : public QWidget
{
    Q_OBJECT

public:
    SkiFall();

private:
    SkiFallCanvas* canvas;
};

#endif // SKIFALL_H
