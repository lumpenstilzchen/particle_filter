#define _USE_MATH_DEFINES 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "particle.h"

double gaussian(double mean, double std) {
    double u1 = (double)rand() / RAND_MAX;
    double u2 = (double)rand() / RAND_MAX;
    double z  = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
    return mean + std * z;
}

void init_particles(Particle* particles, int N) {
    for (int i = 0; i < N; i++) {
        particles[i].position = gaussian(0.0, 2.0);
        particles[i].weights = 1.0 / N;
    }
}

void predict(Particle* particles, int N, double Q) {
    for (int i = 0; i < N; i++)
        particles[i].position += gaussian(0.0, sqrt(Q));
}

void update_weights(Particle* particles, int N, double z, double R) {
    for (int i = 0; i < N; i++)
        particles[i].weights *= exp(-pow(z - particles[i].position, 2) / (2.0 * R));
}

void normalize_weights(Particle* particles, int N) {
    double sum = 0;
    for (int i = 0; i < N; i++)
        sum += particles[i].weights;
    for (int i = 0; i < N; i++)
        particles[i].weights /= sum;
}

void resample(Particle* particles, int N) {
    Particle temp[N]; 
    
    double cumsum[N];
    cumsum[0] = particles[0].weights;
    for (int i = 1; i < N; i++)
        cumsum[i] = cumsum[i-1] + particles[i].weights;
    
    double step = 1.0 / N;
    double start = ((double)rand() / RAND_MAX) * step;
    
    int j = 0;
    for (int i = 0; i < N; i++) {
        double pos = start + i * step;
        while (pos > cumsum[j]) j++;
        temp[i] = particles[j];
        temp[i].weights = 1.0 / N;
    }
    
    for (int i = 0; i < N; i++)
        particles[i] = temp[i];
}

double estimate_position(Particle* particles, int N) {
    double estimate = 0;
    for (int i = 0; i < N; i++)
        estimate += particles[i].position * particles[i].weights;
    return estimate; 
}
