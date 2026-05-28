#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "particle.h"

// N - количество частиц
// S - количество шагов
// Q - дисперсия шума движения
// R - дисперсия шума датчика

int run_test(int N, int S, double Q, double R, const char* filename) {
    if (filename == NULL) {
        fprintf(stderr, "Error: Empty file name\n");
        return -1;
    }

    if (N <= 0) {
        fprintf(stderr, "Error: Incorrect N\n");
        return -1;
    }

    if (S <= 0) {
        fprintf(stderr, "Error: Incorrect S\n");
        return -1;
    }

    if (Q < 0) {
        fprintf(stderr, "Error: Incorrect Q\n");
        return -1;
    }

    if (R <= 0) {
        fprintf(stderr, "Error: Incorrect R\n");
        return -1;
    }

    FILE* output = fopen(filename, "w");

    if (output == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return -1;
    }

    Particle* particles = (Particle*)malloc(N * sizeof(Particle));

    if (particles == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        fclose(output);
        return -1;
    }

    if (init_particles(particles, N) != 0) {
        fprintf(stderr, "Error: init_particles\n");
        free(particles);
        fclose(output);
        return -1;
    }

    fprintf(output, "step,real_pos,measurement,estimate\n");

    double real_pos = 0.0;

    for (int k = 1; k < S; k++) { 
        real_pos += gaussian(0.0, sqrt(Q));

        double z = real_pos + gaussian(0.0, sqrt(R));

        if (predict(particles, N, Q) != 0) {
            fprintf(stderr, "Error on step %d: predict\n", k);
            free(particles);
            fclose(output);
            return -1;
        }

        if (update_weights(particles, N, z, R) != 0) {
            fprintf(stderr, "Error on step %d: update_weight\n", k);
            free(particles);
            fclose(output);
            return -1;
        }

        if (normalize_weights(particles, N) != 0) {
            fprintf(stderr, "Error on step %d: normalize_weight\n", k);
            free(particles);
            fclose(output);
            return -1;
        }

        if (resample(particles, N) != 0) {
            fprintf(stderr, "Error on step %d: resample\n", k);
            free(particles);
            fclose(output);
            return -1;
        }

        double res = estimate_position(particles, N);

        printf("Step %d: position = %.4lf, z = %.4lf, result = %.4lf\n", k, real_pos, z, res);

        fprintf(output, "%d,%lf,%lf,%lf\n", k, real_pos, z, res);
    }

    free(particles);
    fclose(output);
    return 0;
}


int main() {

    srand(time(NULL));

    printf("1. Default\n");
    if (run_test(100, 50, 1.0, 4.0, "output_default.csv") != 0)
        fprintf(stderr, "Error: Default test\n");

    printf("2. Low Q (Q=0.1)\n");
    if (run_test(100, 50, 0.1, 4.0, "output_low_Q.csv") != 0)
        fprintf(stderr, "Error: Low Q test\n");

    printf("3. High Q (Q=5.0)\n");
    if (run_test(100, 50, 5.0, 4.0, "output_high_Q.csv") != 0)
        fprintf(stderr, "Error: High Q test\n");

    printf("4. Low R (R=0.5)\n");
    if (run_test(100, 50, 1.0, 0.5, "output_low_R.csv") != 0)
        fprintf(stderr, "Error: Low R test\n");

    printf("5. High R (R=10.0)\n");
    if (run_test(100, 50, 1.0, 10.0, "output_high_R.csv") != 0)
        fprintf(stderr, "Error: High R test\n");

    printf("6. Few particles (N=10)\n");
    if (run_test(10, 50, 1.0, 4.0, "output_few_particles.csv") != 0)
        fprintf(stderr, "Error: Few particles test\n");

    printf("7. Many particles (N=500)\n");
    if (run_test(500, 50, 1.0, 4.0, "output_many_particles.csv") != 0)
        fprintf(stderr, "Error: Many particles test\n");

    return 0;
}
