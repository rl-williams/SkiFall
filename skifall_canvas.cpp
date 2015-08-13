/* File: skifall_canvas.cpp
 * Author: Becky Williams and Harmony Li
 * Desc: Defines the behavior of the SkiFall game
 */

#include <stdlib.h>
#include <iostream>
#include <QtGui>
#include <iostream>
#include <deque>
#include <sstream>

#include "obstacle_map.h"
#include "skifall_canvas.h"

using namespace std;

int jumping_distance;

/****************************************************************************/


SkiFallCanvas::SkiFallCanvas(QWidget* parent) : QWidget(parent)
{
    timer = new QTimer(this);
    timer->setInterval(5);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerTicked()));

    setFixedSize(WIDTH, HEIGHT);

    // Set automatic focus to the window
    setFocusPolicy(Qt::StrongFocus);

    // Init game trackers
    level = 0;
    distance = 0;
    lives = MAX_LIVES;
    performance = 0;

    //Image stuff:
    bgY = 0;
    bgPix.load("bg.png");
    spritesPix.load("sprites.png");
    oPix.load("obstacles.png");

    reset();
}


/****************************************************************************/


void SkiFallCanvas::keyPressEvent(QKeyEvent *event)
{
    switch(state)
    {

    //If we're in the READY state and Key_Down is pressed, the timer starts,
    //the background knows to move, and we switch to state DOWN.
    case READY:
        if (event->key() == Qt::Key_Down) {
            timer->start();
            bgMoving = true;
            state = DOWN;
        } break;

    //If we're in the DOWN state, the timer starts. If Key_Left or Key_Right
    //are pressed, we switch to the appropriate state.
    case DOWN:
        if (event->key() == Qt::Key_Left) {
            state = D_LEFT;
        } else if (event->key() == Qt::Key_Right) {
            state = D_RIGHT;
        } break;

    //In the D_LEFT state:
    case D_LEFT:
        if (event->key() == Qt::Key_Down) {
            state = DOWN;
        } else if (event->key() == Qt::Key_Left) {
            bgMoving = false;
            state = F_LEFT;
        } else if (event->key() == Qt::Key_Right) {
            state = D_RIGHT;
        } break;

    //In the D_RIGHT state:
    case D_RIGHT:
        if (event->key() == Qt::Key_Down) {
            state = DOWN;
        } else if (event->key() == Qt::Key_Right) {
            bgMoving = false;
            state = F_RIGHT;
        } else if (event->key() == Qt::Key_Left) {
            state = D_LEFT;
        } break;

    //In the F_LEFT state. Shift to the left.
    case F_LEFT:
        playerX -= 4;
        if (event->key() == Qt::Key_Down) {
            bgMoving = true;
            state = DOWN;
        } else if (event->key() == Qt::Key_Right) {
            state = F_RIGHT;
        } break;

    //In the F_RIGHT state. Shift to the right.
    case F_RIGHT:
        playerX += 4;
        if (event->key() == Qt::Key_Down) {
            bgMoving = true;
            state = DOWN;
        } else if (event->key() == Qt::Key_Left) {
            state = F_LEFT;
        } break;

    //In the CRASHED state. Resume by pressing Key_Down.
    case CRASHED:
        if (event->key() == Qt::Key_Down) {
            bgMoving = true;
            collision = false;
            state = DOWN;
        } else if (event->key() == Qt::Key_Right){
            playerX += 20;
        } else if (event->key() == Qt::Key_Left){
            playerX -= 20;
        } break;

    //In the JUMPING state. Key presses do nothing.
    //Have to wait til it's finished.
    case JUMPING:

    //In the FINISHED state. Go back to READY state by pressing any key.
    case FINISHED:
        level++;
        reset();
        break;

    //In the GAMEOVER state. Reset everything return to beginning by pressing any key.
    case GAMEOVER:
        level = 0;
        reset();
        break;

    }

    //Do we need this?
    setFocus(Qt::OtherFocusReason);
}


/****************************************************************************/


void SkiFallCanvas::reset()
{
    playerX = WIDTH / 2 - (PLAYER_DIM / 2);
    playerY = HEIGHT / 2 - (PLAYER_DIM / 2);

    distance = 0;
    lives = 10;
    bgMoving = false;
    collision = false;
    bgY = 0;

    char filename[1024];
    sprintf(filename, "level%d.map", level);
    string level_file = string(filename);
    obstacles = readMapFile(level_file); 

    state = READY;

    update();
}


/****************************************************************************/


void SkiFallCanvas::paintEvent(QPaintEvent*)
{
    //Values used to specify starting point in pixmap
    int guyPixX = 0;
    int oPixX = 0;
    int oPixWidth = 0;
    int oPixHeight = 0;

    //Create painting context.
    QPainter p(this);

    //Draw the field.
    if (bgMoving) bgY++;
    p.drawPixmap(0, 30, bgPix, 0, bgY, 0, 0);


    //Write game trackers:
    std::stringstream s1;
    s1 << "Level: " << level;
    p.drawText(QRect(25, 8, WIDTH, 30), Qt::AlignLeft, s1.str().c_str());

    std::stringstream s2;
    s2 << "Distance: " << distance/10;
    p.drawText(QRect(155, 8, WIDTH, 30), Qt::AlignLeft, s2.str().c_str());

    std::stringstream s3;
    s3 << "Lives: " << lives;
    p.drawText(QRect(310, 8, WIDTH, 30), Qt::AlignLeft, s3.str().c_str());

    //Potentially add "Performance" score as well.


    //Screen messages and change Pixmap guyPixX.
    switch (state) {
    case READY:
        guyPixX = 160;
        p.drawText(QRect(0, 300, WIDTH, HEIGHT), Qt::AlignHCenter, "Can you survive 1000 meters down our Slip 'n' Slide?!\nPress DOWN to Start!");
        break;
    case DOWN:
        guyPixX = 200;
        break;
    case D_LEFT:
        guyPixX = 0;
        break;
    case D_RIGHT:
        guyPixX = 40;
        break;
    case F_LEFT:
        guyPixX = 80;
        break;
    case F_RIGHT:
        guyPixX = 120;
        break;
    case CRASHED:
        guyPixX = 160;
        p.drawText(QRect(0, 440, WIDTH, HEIGHT), Qt::AlignHCenter, "Press DOWN to Continue!");
        break;
    case JUMPING:
        guyPixX = 200;
        break;
    case FINISHED:
        guyPixX = 160;
        p.drawText(QRect(0, 420, WIDTH, HEIGHT), Qt::AlignHCenter, "YOU WON!\n press any key to continue!");
        break;
    case GAMEOVER:
        guyPixX = 160;
        p.drawText(QRect(0, 420, WIDTH, HEIGHT), Qt::AlignHCenter, "YOU LOST.\n press any key to start over!");
        break;
    }


   // Draw obstacles
   std::deque<obstacle*>::iterator it = obstacles.begin();
   while(it != obstacles.end()) {
       if((*it)->y >= 2) {
           if((*it)->y > HEIGHT) break;
           switch((*it)->t) {
           case ROCK:
               oPixX = 0;
               oPixWidth = 40;
               oPixHeight = 40;
               break;
           case TREE:
               oPixX = 40;
               oPixWidth = 40;
               oPixHeight = 80;
               break;
           case POLE:
               oPixX = 40; //Right now it's same as Tree
               oPixWidth = 40;
               oPixHeight = 80;
               break;
           case RAMP:
               oPixX = 80;
               oPixWidth = 80;
               oPixHeight = 40;
               break;
           }
           p.drawPixmap(((*it)->x * 20 + 20), ((*it)->y * 20), oPix, oPixX, 0, oPixWidth, oPixHeight);
       }
       ++it;
   }


    //If jumping...
    if (state == JUMPING) {
        collision = false;    //Is this all we need to do to make sure collision won't happen while jumping?
        if (jumping_distance <= 0) {
            playerY = HEIGHT / 2 - (PLAYER_DIM / 2);
            state = DOWN;
            return;
        } else {
            jumping_distance -= 20;
        }
        playerY = log(playerY);
    }

    //Draw the character:
    p.drawPixmap(playerX-8, playerY-8, spritesPix, guyPixX, 0, 40, 0);
}


/****************************************************************************/


//Every 50 milliseconds...
void SkiFallCanvas::timerTicked()
{
    //Distance tracker:
    if (bgMoving) distance++;
    if (distance >= 1000) {
        bgMoving = false;
        state = FINISHED;
    }


 // Move all objects upwards
    if(bgMoving) {
      std::deque<obstacle*>::iterator it = obstacles.begin();
      while(it != obstacles.end()){
        (*it)->y -= 1;
        ++it;
      }
   }

    //Collision checker:
    if (state != CRASHED) {
      bool stop;
      collision = collide((playerX / PLAYER_DIM), (playerY / PLAYER_DIM), performance, stop, obstacles);
      if (collision) {
          if (stop) {
            lives--;
            state = CRASHED;
            bgMoving = false;
          } else {
            jumping_distance = 140;
            state = JUMPING;
          }
      }
    }

    //Lives tracker:
    if (lives <= 0) {
        lives = 0;
        bgMoving = false;
        state = GAMEOVER;
    }


    //Shift the player left or right if moving diagonally.
    if (state == D_LEFT) {
        playerX--;
    } else if (state == D_RIGHT) {
        playerX++;
    }

    //If the player hits a wall, continue straight down by the wall, like in SkiFree.
    if (playerX <= 15) {
        playerX = 15;
        state = CRASHED;
        bgMoving = false;
    } else if (playerX + PLAYER_DIM - 15 >= WIDTH - 15) {
        playerX = WIDTH - PLAYER_DIM - 15;
        state = CRASHED;
        bgMoving = false;
    }

    update();
}
