#include<stdio.h>
#include<stdlib.h>
#include<complex.h>
#include<math.h>


#define NUMBER_OF_POINTS 30
#define NUMBER_OF_DESCRIPTORS 2


void descriptor(double complex *input, double complex *output, int N, int U){
  double angle = 0;
  for(int u = 0; u < U; u++) {
    output[u] = 0.00 + 0.00 * I;
    angle = 0;
    for(int k = 0; k < N; k++) {
      angle = (-1 * 2.0 * M_PI * k * u) / N;
      output[u] += input[k] *  cexp(I * angle);
    }
    output[u] /= N;
   }
}
void desc2points(double complex *descriptors, double complex *points, int N, int U) {
  double angle = 0;
  for (int k = 0; k < N; k++) {
    points[k] = 0.0 + 0.0 * I;
    angle = 0;
    for (int u = 0; u < U; u++) {
      angle = (2.0 * M_PI * k * u) / U;
      points[k] += descriptors[u] * cexp(angle * I);
    }
    //points[k] /= U;
  }
}


void print(double complex *output, double complex *points, double complex *input, int U, int N) {

  for (int i = 0; i < U; i++) {
    printf("descriptor %d: %f + j%f\n", i, creal(output[i]), cimag(output[i]));
  }
  for (int j = 0; j < N; j++) {
    printf("input point %d: %.3f + j%.3f  ::::::  otput point %d: %.3f + j%.3f\n", j, creal(input[j]), cimag(input[j]), j, creal(points[j]), cimag(points[j]));
  }
}

int main(int argc, char *argv[]) {

  double complex input[NUMBER_OF_POINTS];
  double complex output[NUMBER_OF_DESCRIPTORS];
  double complex points[NUMBER_OF_POINTS];

  for(int i = 0; i < NUMBER_OF_POINTS; i++) {
    input[i] = (i / (double)NUMBER_OF_POINTS) + ((-1.0 * i) / (double)NUMBER_OF_POINTS) * I;
    printf("input %d: %.2f + j%.2f\n", i, creal(input[i]), cimag(input[i]));
								
  }
  descriptor(input, output, NUMBER_OF_POINTS, NUMBER_OF_DESCRIPTORS);
  desc2points(output, points, NUMBER_OF_POINTS, NUMBER_OF_DESCRIPTORS);
  print(output, points, input, NUMBER_OF_DESCRIPTORS, NUMBER_OF_POINTS);

  return 0;

}
