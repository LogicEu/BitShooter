#ifndef BIT_SHOOTER_H
#define BIT_SHOOTER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

/****************
   BIT SHOOTER
         @eulogic
 ****************/

#include <glee.h>
#include <photon.h>
#include <utopia.h>

typedef struct Player2D {
    vec2 pos;
    float rot;
} Player2D;

extern vec2 iResolution;
extern float iTime, iScale;

extern Player2D player2D;
extern array_t *map2D, *mapIndices;

void gameInit(int argc, char** argv);
void gameUpdate();
void gameExit();

void graphics2D_init();
void map2D_init(int width, int height);

void triDraw(Tri2D* tri);
void mapDraw();
void backgroundDraw(float T);

vec2 world_to_screen(vec2 p);
vec2 screen_to_world(vec2 p);
Tri2D player_to_triangle(Player2D p);

#ifdef __cplusplus
}
#endif
#endif