#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "particle.h"

int main() {
    FILE *output = fopen("output.csv","w");
    
    if (output == NULL) {
    fprintf(stderr, "Ошибка открытия файла\n");
    return 1;
}

    int S = 50;
    int N = 100;
    double Q = 1.0;
    double R = 4.0;

    Particle particles[N];
    init_particles(particles, N);

    double real_pos = 0.0;

    for (int k = 1; k < S; k++) { 
        real_pos += gaussian(0.0, sqrt(Q));

        double z = real_pos + gaussian(0.0, sqrt(R));

        predict(particles, N, Q);
        update_weights(particles, N, z, R);
        normalize_weights(particles, N);
        resample(particles, N);

        double res = estimate_position(particles, N);

        printf("Step %d: position = %.4lf, z = %.4lf, result = %.4lf\n", k, real_pos, z, res);

        fprintf(output, "%d,%lf,%lf,%lf\n", k, real_pos, z, res);
    }

    fclose(output);
    return 0;
}
