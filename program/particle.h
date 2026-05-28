#ifndef PARTICLE
#define PARTICLE


typedef struct Particle{
    double position;
    double weights;
} Particle;

double gaussian(double mean, double std);

int init_particles(Particle* particles, int N);

int predict(Particle* particles, int N, double Q);

int update_weights(Particle* particles, int N, double z, double R);

int normalize_weights(Particle* particles, int N);

int resample(Particle* particles, int N);

double estimate_position(Particle* particles, int N);


#endif
