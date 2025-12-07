#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

#define WINDOW_HEIGHT 1000
#define WINDOW_WIDTH 1000
#define COLOR_WHITE 0xFFFFFF
#define COLOR_GRAY  0x151716 
#define COLOR_MAGENTA 0xff00ff

#define VELOCITY_X 200
#define VELOCITY_Y 0

#define DAMPNESS 1 // no energy lost on colliding
                       
typedef struct {
  double cx;
  double cy;
  double vx;
  double vy;
} Circle;

const double g = 980;
const int radius = 50;

void drawCircle(Circle *circle,SDL_Surface *surface);

int main(void) {
  if(SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("%s ",SDL_GetError());
  }
  SDL_Window *window = SDL_CreateWindow("Ball with Gravity",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WINDOW_WIDTH,WINDOW_HEIGHT,SDL_WINDOW_SHOWN);
  if(window == NULL) { 
    printf("%s",SDL_GetError());
  }
  SDL_Surface *surface = SDL_GetWindowSurface(window);
  if(surface == NULL) {
    printf("%s",SDL_GetError());
  }
  bool isRunning = true;
  SDL_Event event;
  Circle a = { 0 };

  a.cx = 500;
  a.cy = 0;
  a.vx = VELOCITY_X;
  a.vy = VELOCITY_Y;

  Uint32 lastTime = SDL_GetTicks();

  while(isRunning) {
    while(SDL_PollEvent(&event)) {
      if(event.type == SDL_QUIT) isRunning = false;
    }
    Uint32 currentTime = SDL_GetTicks();
    double deltaTime = (currentTime - lastTime) / 1000.00;
    lastTime = currentTime;
    SDL_FillRect(surface,NULL,COLOR_GRAY);
    a.vx += 10;
    a.vy += g * deltaTime;
    a.cx += a.vx * deltaTime;
    a.cy += a.vy * deltaTime;
    drawCircle(&a,surface);

    if(a.cy + radius >= WINDOW_HEIGHT) {
      a.cy = WINDOW_HEIGHT - radius ;
      a.vy = -a.vy * DAMPNESS;
    }

    if(a.cy - radius <= 0) {
      a.cy = radius;
      a.vy = -a.vy * DAMPNESS;
    }

    if(a.cx + radius >= WINDOW_WIDTH) {
      a.cx = WINDOW_WIDTH - radius; 
      a.vx = -a.vx * DAMPNESS;
    }
    if(a.cx - radius <= 0) {
      a.cx = radius;
      a.vx = -a.vx * DAMPNESS;
    }
    SDL_UpdateWindowSurface(window);
  }
  return 0;
}

void drawCircle(Circle *circle, SDL_Surface *surface) {
  SDL_Rect pixel = {
    .h = 1,
    .w = 1,
    .x = 0,
    .y = 0,
  };
  for(int dx = -radius ; dx <= radius ; dx++) {
    for(int dy = -radius ; dy <= radius ; dy++) {
      if( (dx*dx) + (dy*dy) <= radius*radius) {
        pixel.x =(int) circle->cx + dx;
        pixel.y =(int) circle->cy + dy;
        SDL_FillRect(surface, &pixel, COLOR_MAGENTA);
      }
    }
  }
}
