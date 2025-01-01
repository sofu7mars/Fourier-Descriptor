#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#define RECT_SIZE 200
#define NUMBER_OF_POINTS 100
#define NUMBER_OF_DESCRIPTORS 10

struct Rect {
  int x;
  int y;
};  

struct Rect rect[NUMBER_OF_POINTS];
struct Rect output_rect[NUMBER_OF_POINTS];

void init_shape() {
  int points_per_side = NUMBER_OF_POINTS / 4;
  int side_lenght = 200;
  for (int i = 0; i < points_per_side; i++) {
    int step_size = side_lenght / points_per_side;
    rect[i] = (struct Rect) {-side_lenght/2 + i * step_size, -side_lenght/2};
    rect[i + points_per_side] = (struct Rect) {side_lenght/2, -side_lenght/2 + i * step_size};
    rect[i + 2 * points_per_side] = (struct Rect) {side_lenght/2 - i * step_size, side_lenght/2};
    rect[i + 3 * points_per_side] = (struct Rect) {-side_lenght/2, side_lenght/2 - i * step_size};
  }
}

void points2compfunc(double complex *input) {
  for (int i = 0; i < NUMBER_OF_POINTS; i++) {
    input[i] = rect[i].x + I * rect[i].y;
  }
}
void compfunc2points(double complex *output) {
  for (int i = 0; i < NUMBER_OF_POINTS; i++) {
    output_rect[i] = (struct Rect) {creal(output[i]), cimag(output[i])};
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
   
  }
}


void print(double complex *input, double complex *descriptors, double complex *output){
  for (int i = 0; i < NUMBER_OF_POINTS; i++) {
    printf("POINTS x: %d, y: %d::::COMPLEX FUNCTION: %.2f + j%.2f |||||| OUTPUT COMPLEX FUNCTION: %.2f + j%.2f\n", rect[i].x, rect[i].y, creal(input[i]), cimag(input[i]), creal(output[i]), cimag(output[i]));
  }
  for (int i = 0; i < NUMBER_OF_DESCRIPTORS; i++) {
    printf("DESCRIPTOR %d: %.2f + j%.2f\n", i, creal(descriptors[i]), cimag(descriptors[i]));
  }
}




int main(int argc, char *argv[]) {

  double complex input[NUMBER_OF_POINTS];
  double complex descriptors[NUMBER_OF_DESCRIPTORS];
  double complex output[NUMBER_OF_POINTS];
  init_shape();
  points2compfunc(input);
  descriptor(input, descriptors, NUMBER_OF_POINTS, NUMBER_OF_DESCRIPTORS);
  desc2points(descriptors, output, NUMBER_OF_POINTS, NUMBER_OF_DESCRIPTORS);
  print(input, descriptors, output);
  compfunc2points(output);

  
  FILE *gp;
  gp = popen("gnuplot -persistent", "w");
  if (!gp) {
    printf("Error opening pipe to gnuplot. \n");
    return 1;
  }
  
  fprintf(gp, "set xrange [-250:250]\n");
  fprintf(gp, "set yrange [-150:150]\n");

  fprintf(gp, "set title 'Rectangle with 9 points'\n");
  fprintf(gp, "set xlabel 'X'\n");
  fprintf(gp, "set ylabel 'Y'\n");
  fprintf(gp, "plot '-' with linespoints pointtype 7 pointsize 0.5 lc rgb 'blue' title 'Input shape', '-' with linespoints pointtype 7 pointsize 0.5 lc rgb 'red' title 'Output shape' \n");
  
  for (int i = 0; i < NUMBER_OF_POINTS; i++) {
    fprintf(gp, "%d %d\n", rect[i].x, rect[i].y);
  }
  fprintf(gp, "e\n");
  for (int i = 0; i < NUMBER_OF_POINTS; i++) {
    fprintf(gp, "%d %d\n", output_rect[i].x, output_rect[i].y);
  }
  fclose(gp); 
  return 0;
}
