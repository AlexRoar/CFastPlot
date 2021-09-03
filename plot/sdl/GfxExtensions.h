#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL2_rotozoom.h>

extern const Uint32 rmask;
extern const Uint32 gmask;
extern const Uint32 bmask;
extern const Uint32 amask;

int thickEllipseRGBA(SDL_Renderer *renderer,Sint16 xc,Sint16 yc,Sint16 xr,Sint16 yr,Uint8 r,Uint8 g,Uint8 b,Uint8 a,Uint8 thick);
int thickCircleRGBA(SDL_Renderer *renderer,Sint16 x,Sint16 y,Sint16 rad,Uint8 r,Uint8 g,Uint8 b,Uint8 a,Uint8 thick);
int thickEllipseColor(SDL_Renderer *renderer,Sint16 x,Sint16 y,Sint16 rx,Sint16 ry,Uint32 color,Uint8 thick);
int thickCircleColor(SDL_Renderer *renderer,Sint16 x,Sint16 y,Sint16 rad,Uint32 color,Uint8 thick);
int aaFilledEllipseRGBA(SDL_Renderer *renderer,float cx,float cy,float rx,float ry,Uint8 r,Uint8 g,Uint8 b,Uint8 a);
int aaFilledEllipseColor(SDL_Renderer *renderer,float cx,float cy,float rx,float ry,Uint32 color);
Uint32 ColorToUint(int r,int g,int b,int a);
Uint32 ColorToUint(SDL_Color color);
SDL_Color UintToColor(Uint32 color);
