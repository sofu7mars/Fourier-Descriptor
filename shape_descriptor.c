#include<stdio.h>
#include<stdlib.h>
#include<complex.h>
#include<math.h>
#include<SDL2/SDL.h>

//gcc -o imdesc shape_descriptor.c $(sdl2-config --cflags --libs) -lm <--- for compilling 

#define SURFACE_WIDTH 900
#define SURFACE_HEIGHT 600
#define NUMBER_OF_DESCRIPTORS 150
#define POINT_SHAPE 5
#define OBJECT_POINTS 200
#define COORDINATE_X_OFFSET SURFACE_WIDTH/2
#define COORDINATE_Y_OFFSET SURFACE_HEIGHT/2
#define COLOR_GRAY 0xf1f1f1f1
#define COLOR_WHITE 0xffffffff
#define COLOR_BLACK 0x00000000

struct Object {
  int x;
  int y;
};

struct Object rect[OBJECT_POINTS];
struct Object output_rect[OBJECT_POINTS];

void draw_point(SDL_Surface* surface, double x, double y, uint32_t color) {
  SDL_Rect pixel = (SDL_Rect) {x, y, POINT_SHAPE, POINT_SHAPE};
  SDL_FillRect(surface, &pixel, color);
}

void init_shape() {
  int points_per_side = OBJECT_POINTS / 4;
  int side_lenght = OBJECT_POINTS;
  for (int i = 0; i < points_per_side; i++) {
    int step_size = OBJECT_POINTS / points_per_side;
    rect[i] = (struct Object) {-side_lenght/2 + i * step_size, -side_lenght/2};
    rect[i+points_per_side] = (struct Object) {side_lenght/2, -side_lenght/2 + i * step_size};
    rect[i+2*points_per_side] = (struct Object) {side_lenght/2 - i * step_size, side_lenght/2};
    rect[i+3*points_per_side] = (struct Object) {-side_lenght/2, side_lenght/2 - i * step_size};
  }
}

void points2compfunc(double complex *input) {
  for (int i = 0; i < OBJECT_POINTS; i++) {
    input[i] = rect[i].x + I * rect[i].y;
  }
}
void compfunc2points(double complex *output) {
  for (int i = 0; i < OBJECT_POINTS; i++) {
    output_rect[i] = (struct Object) {creal(output[i]), cimag(output[i])};
  }
}

void draw_shape(SDL_Surface* surface, struct Object points[], uint32_t color) {
  for (int i = 0; i < OBJECT_POINTS; i++) {
    int point_x = points[i].x + COORDINATE_X_OFFSET;
    int point_y = points[i].y + COORDINATE_Y_OFFSET;
    draw_point(surface, point_x, point_y, color);
  }
}

void descriptor(double complex *input, double complex *descriptors, int N, int U){
  double angle = 0;
  for(int u = 0; u < U; u++) {
    descriptors[u] = 0.00 + 0.00 * I;
    angle = 0;
    for(int k = 0; k < N; k++) {
      angle = (-1 * 2.0 * M_PI * k * u) / N;
      descriptors[u] += input[k] *  cexp(I * angle);
    }
    descriptors[u] /= N;
    //printf("u: %d, -- angle: %.2f, descriptor: %.2f + j%.2f\n",u, angle, creal(descriptors[u]), cimag(descriptors[u]));
  }
}
void desc2points(double complex *descriptors, double complex *output, int N, int U) {
  double angle = 0;
  for (int k = 0; k < N; k++) {
    output[k] = 0.0 + 0.0 * I;
    angle = 0;
    for (int u = 0; u < U; u++) {
      angle = (2.0 * M_PI * k * u) / U;
      output[k] += descriptors[u] * cexp(angle * I);
    }
    //points[k] /= U;
  }
}


void print(double complex *input, double complex *descriptors, double complex *output){
  for (int i = 0; i < OBJECT_POINTS; i++) {
    printf("POINTS x: %d, y: %d::::COMPLEX FUNCTION: %.2f + j%.2f |||||| OUTPUT COMPLEX FUNCTION: %.2f + j%.2f\n", rect[i].x, rect[i].y, creal(input[i]), cimag(input[i]), creal(output[i]), cimag(output[i]));
  }
  for (int i = 0; i < NUMBER_OF_DESCRIPTORS; i++) {
    printf("DESCRIPTOR %d: %.2f + j%.2f\n", i, creal(descriptors[i]), cimag(descriptors[i]));
  }
}


int main(int argc, char *argv[]) {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window* window = SDL_CreateWindow("Shape Descriptor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 900, 600, 0);
  SDL_Surface* surface = SDL_GetWindowSurface(window);
  double complex input[OBJECT_POINTS];
  double complex descriptors[NUMBER_OF_DESCRIPTORS];
  double complex output[OBJECT_POINTS];
  init_shape();
  points2compfunc(input);
  descriptor(input, descriptors, OBJECT_POINTS, NUMBER_OF_DESCRIPTORS);
  desc2points(descriptors, output, OBJECT_POINTS, NUMBER_OF_DESCRIPTORS);
  print(input, descriptors, output);
  compfunc2points(output);
  
  SDL_Event event;
  int running = 1;
  while (running) {
    while(SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT){
	running = 0;
      }
    }
    SDL_FillRect(surface, NULL, COLOR_BLACK);
    draw_shape(surface, output_rect, COLOR_GRAY);
    SDL_UpdateWindowSurface(window);
  }
  
  return 0;

}
