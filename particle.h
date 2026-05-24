#ifndef PARTICLE
#define PARTICLE

typedef struct Particle{
    double position;
    double weights;
} Particle;

void init_particles(Particle* particles, int N);

void predict(Particle* particles, int N, double Q);

void update_weights(Particle* particles, int N, double z, double R);

void normalize_weights(Particle* particles, int N);

void resample(Particle* particles, int N);

double estimate_position(Particle* particles, int N);

#endif
