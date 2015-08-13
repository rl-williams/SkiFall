/* File: obstacle_map.cpp
 * Author: Becky Williams and Harmony Li
 * Desc: Reads in the .map file to create the map for the levels.
 */

#include "obstacle_map.h"

using namespace std;

std::deque<obstacle*> readMapFile(string filename){
    std::deque<obstacle*> obstacle_map = std::deque<obstacle*>();
    char* f_name = (char*)filename.c_str();
    ifstream map_file;
    map_file.open(f_name);
 
    char c;
    int line, c_idx;
    line  = c_idx = 0;
    
    // Read stream
    if(map_file.is_open()){
        while(map_file.good() && !map_file.eof()){
             map_file >> c;
             if(c == '\n' || c == '\r'){
                 c_idx = 0;
             }else if(c == '*' || c == '+' || c == '=' || c == '^'){
                 obstacle* o = (obstacle*)malloc(sizeof(obstacle));
                 switch(c){
                     case '*':
                          o->t = ROCK;
                          break;
                     case '+':
                          o->t = TREE; 
                          break;
                     case '^':
                          o->t = POLE;
                          break;
                     case '=':
                          o->t = RAMP;
                          break;
                 }
                 o->x = c_idx % 20;
                 o->y = c_idx / 20; 
                 obstacle_map.push_back(o);
                 ++c_idx;
             }else{
               ++c_idx;
             }
        }
    }
    
    // Close file
    map_file.close();  
    return obstacle_map;
}

bool collide(int x, int y, int& performance, bool& stop, std::deque<obstacle*> obstacle_map){
    std::deque<obstacle*>::iterator it = obstacle_map.begin();
    while(it != obstacle_map.end()){
         switch((*it)->t){
             case ROCK:
                 if(x == (*it)->x && y == (*it)->y){
                     performance -= 10;
                     stop = true;
                     return true;
                 }
                 break;
             case TREE:
                 if(x == (*it)->x && y >= (*it)->y && y <= (*it)->y + 1){
                     performance -= 20;
                     stop = true;
                     return true;
                 } 
                 break;
             case POLE:
                 if(x == (*it)->x && y >= (*it)->y && y <= (*it)->y + 3){
                     performance -= 40;
                     stop = true;
                     return true;
                 }
                 break;
             case RAMP:
                 if(x >= (*it)->x && x <= (*it)->x + 1 && y == (*it)->y){
                     performance += 50;
                     stop = false;
                     return true;
                 }
                 break;
         }
         ++it;
    }
    return false;
}
