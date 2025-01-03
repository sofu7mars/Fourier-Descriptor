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


#define INPUT_ARRAY_POINTS (sizeof(arr) / sizeof(arr[0]))
#define NUMBER_OF_POINTS 200

#define NUMBER_OF_DESCRIPTORS 200



struct Points {
  double x;
  double y;
};  

struct Points points[NUMBER_OF_POINTS];
struct Points output_points[NUMBER_OF_POINTS];

void interpolation() {
  int interp_points = NUMBER_OF_POINTS, num_points = INPUT_ARRAY_POINTS;
  
  int seg_points = interp_points / (num_points - 1); 
    int index = 0;

    for (int i = 0; i < num_points - 1; i++) {
        double x1 = arr[i][0], y1 = arr[i][1];
        double x2 = arr[i + 1][0], y2 = arr[i + 1][1];

        for (int j = 0; j < seg_points; j++) {
            double t = (double)j / seg_points; 
            points[index].x = x1 + t * (x2 - x1);
            points[index].y= y1 + t * (y2 - y1); 
            index++;
        }
    }

    
    points[index].x = arr[num_points - 1][0];
    points[index].y = arr[num_points - 1][1];

}




void points2compfunc(double complex *input) {
  for (int i = 0; i < NUMBER_OF_POINTS; i++) {
    input[i] = points[i].x + I * points[i].y;
  }
}
void compfunc2points(double complex *output) {
  for (int i = 0; i < NUMBER_OF_POINTS; i++) {
    output_points[i].x = creal(output[i]);
    output_points[i].y = cimag(output[i]);
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
    printf("Nr: %d, POINTS x: %f, y: %f::::COMPLEX FUNCTION: %.2f + j%.2f |||||| OUTPUT COMPLEX FUNCTION: %.2f + j%.2f :::: OUTPUT POINTS x: %f, y: %f \n",i, points[i].x, points[i].y, creal(input[i]), cimag(input[i]), creal(output[i]), cimag(output[i]), output_points[i].x, output_points[i].y);
  }
  for (int i = 0; i < NUMBER_OF_DESCRIPTORS; i++) {
    printf("DESCRIPTOR %d: %.2f + j%.2f\n", i, creal(descriptors[i]), cimag(descriptors[i]));
  }
}




int main(int argc, char *argv[]) {

  double complex input[NUMBER_OF_POINTS];
  double complex descriptors[NUMBER_OF_DESCRIPTORS];
  double complex output[NUMBER_OF_POINTS];
  interpolation();
  
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
    fprintf(gp, "%f %f\n", points[i].x, points[i].y);
  }
  fprintf(gp, "e\n");
  for (int i = 0; i < NUMBER_OF_POINTS; i++) {
    fprintf(gp, "%f %f\n", output_points[i].x, output_points[i].y);
  }
  fclose(gp); 
  return 0;
}
