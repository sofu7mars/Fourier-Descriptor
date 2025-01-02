#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

int arr[26][2] ={
  {1, -3},  {5, -4},  {4, -3},  {9, 1},
  {7, 2},  {8, 5},  {5, 4},  {5, 5},
  {3, 4},  {4, 9},  {2, 7},  {0, 10},
  {-2, 7},  {-4, 8},  {-3, 3},  {-5, 6},
  {-5, 4},  {-8, 5},  {-7, 2},  {-9, 1},
  {-4, -3},  {-5, -4},  {0, -3},  {2, -7},
  {2, -6},  {1, -3}
};

int arr1[52][2];

#define NUMBER_OF_POINTS (sizeof(arr1) / sizeof(arr1[0]))
#define NUMBER_OF_DESCRIPTORS 26

struct Points {
  int x;
  int y;
};  

struct Points points[NUMBER_OF_POINTS];
struct Points output_points[NUMBER_OF_POINTS];

void points_array(){
  int j = 0;
  for (int i = 0; i < 26; i++) {
    arr1[j][0] = arr[i][0];
    arr1[j][1] = arr[i][1];

    j++;

    arr1[j][0] = round((arr[i][0] + arr[i+1][0]) / 2);
    arr1[j][1] = round((arr[i][1] + arr[i+1][1]) / 2);

    j++;
  }

  arr1[51][0] = arr[25][0];
  arr1[51][1] = arr[25][1];
  
}

void points2struct() {
  for (int i = 0; i < NUMBER_OF_POINTS; i++) {
    points[i] = (struct Points) {arr1[i][0], arr1[i][1]};
  }
}




void points2compfunc(double complex *input) {
  for (int i = 0; i < NUMBER_OF_POINTS; i++) {
    input[i] = points[i].x + I * points[i].y;
  }
}
void compfunc2points(double complex *output) {
  for (int i = 0; i < NUMBER_OF_POINTS; i++) {
    output_points[i].x = round(creal(output[i]));
    output_points[i].y = round(cimag(output[i]));
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
    printf("Nr: %d, POINTS x: %d, y: %d::::COMPLEX FUNCTION: %.2f + j%.2f |||||| OUTPUT COMPLEX FUNCTION: %.2f + j%.2f :::: OUTPUT POINTS x: %d, y: %d \n",i, points[i].x, points[i].y, creal(input[i]), cimag(input[i]), creal(output[i]), cimag(output[i]), output_points[i].x, output_points[i].y);
  }
  for (int i = 0; i < NUMBER_OF_DESCRIPTORS; i++) {
    printf("DESCRIPTOR %d: %.2f + j%.2f\n", i, creal(descriptors[i]), cimag(descriptors[i]));
  }
}




int main(int argc, char *argv[]) {

  double complex input[NUMBER_OF_POINTS];
  double complex descriptors[NUMBER_OF_DESCRIPTORS];
  double complex output[NUMBER_OF_POINTS];
  points_array();
  points2struct();
  points2compfunc(input);
  descriptor(input, descriptors, NUMBER_OF_POINTS, NUMBER_OF_DESCRIPTORS);
  desc2points(descriptors, output, NUMBER_OF_POINTS, NUMBER_OF_DESCRIPTORS);
  
  compfunc2points(output);
  print(input, descriptors, output);
  
  FILE *gp;
  gp = popen("gnuplot -persistent", "w");
  if (!gp) {
    printf("Error opening pipe to gnuplot. \n");
    return 1;
  }
  
  fprintf(gp, "set xrange [-10:10]\n");
  fprintf(gp, "set yrange [-10:15]\n");
  
  fprintf(gp, "set grid\n");

  fprintf(gp, "set title 'Red Foliage Leaf'\n");
  fprintf(gp, "set xlabel 'X'\n");
  fprintf(gp, "set ylabel 'Y'\n");
  fprintf(gp, "plot '-' with linespoints pointtype 7 pointsize 0.5 lc rgb 'blue' title 'Input shape', '-' with linespoints pointtype 7 pointsize 0.5 lc rgb 'red' title 'Output shape' \n");
  
  for (int i = 0; i < NUMBER_OF_POINTS; i++) {
    fprintf(gp, "%d %d\n", points[i].x, points[i].y);
  }
  fprintf(gp, "e\n");
  for (int i = 0; i < NUMBER_OF_POINTS; i++) {
    fprintf(gp, "%d %d\n", output_points[i].x, output_points[i].y);
  }
  fclose(gp); 
  return 0;
}
