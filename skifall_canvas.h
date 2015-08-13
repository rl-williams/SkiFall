/* File: skifall_canvas.h
 * Author: Becky Williams and Harmony Li
 * Desc: Header file for SkiFall Canvas. Game logic goes here.
 */

#ifndef SKIFALL_CANVAS_H
#define SKIFALL_CANVAS_H

#include <deque>
#include <QtGui>
#include "obstacle_map.h"

class SkiFallCanvas : public QWidget
{
    Q_OBJECT

public:
    SkiFallCanvas(QWidget* parent);

protected:
    void paintEvent(QPaintEvent* event);
    void keyPressEvent(QKeyEvent* event);

private slots:
    void timerTicked();

private:
    //Reset playing field
    void reset();

    enum State { READY, DOWN, D_LEFT, D_RIGHT, F_LEFT, F_RIGHT, CRASHED, JUMPING, FINISHED, GAMEOVER } state;

    QTimer* timer;

    QPixmap bgPix;
    QPixmap spritesPix;
    QPixmap oPix;

    //Game trackers
    int level;
    int distance; //per level distance tracker
    int performance; // NOTE: potential score check, this is optional
    int lives; // normal SkiFree does not have this

    std::deque<obstacle*> obstacles;

    //Coordinates of the left side of player
    int playerX;
    int playerY;

    //Whether background objects should be shifting upwards or not
    bool bgMoving;
    int bgY;

    bool collision;

    static const int WIDTH = 400;
    static const int HEIGHT = 500;
    static const int MAX_LIVES = 10;
    static const int PLAYER_DIM = 20; //dimensions of the player
};


#endif // SKIFALL_CANVAS_H
