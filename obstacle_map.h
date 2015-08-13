/* File: obstacle_map.h
 * Author: Becky Williams and Harmony Li
 * Desc: Reads in the .map file to create the map for the levels.
 */

#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

enum o_type { ROCK, TREE, POLE, RAMP };

struct obstacle{
    int x;
    int y;
    o_type t;
};

// Obstacle deque
      
std::deque<obstacle*> readMapFile(std::string filename);

bool collide(int x, int y, int& performance, bool& stop, std::deque<obstacle*> obstacle_map);

#endif
