#define _USE_MATH_DEFINES 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Particle{
    double position;
} Particle;

double gaussian(double mean, double std) {
    double u1 = (double)rand() / RAND_MAX;
    double u2 = (double)rand() / RAND_MAX;
    double z  = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
    return mean + std * z;
}

void init_particles(Particle* particles, int N) {
    for (int i = 0; i < N; i++)
        particles[i].position = gaussian(0.0, 2.0);
}
