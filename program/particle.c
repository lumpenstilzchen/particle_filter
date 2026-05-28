#define _USE_MATH_DEFINES 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "particle.h"


// генерация случайного значения из нормального распределения (распределения Гаусса)
// (вспомогательная функция)
double gaussian(double mean, double std) {
    if (std < 0) {
        fprintf(stderr, "Ошибка: отрицательное стандартное отклонение\n");
        return mean;
    }

    double u1 = (double)rand() / RAND_MAX;
    double u2 = (double)rand() / RAND_MAX;

    // защита от нулевого логарифма
    if (u1 < 1e-10)
        u1 = 1e-10;

    double z  = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
    return mean + std * z;
}

// инициализация частиц из гауссовского распределения
// инициализация с одинаковым весом
int init_particles(Particle* particles, int N) {
    if (particles == NULL || N <= 0)
        return -1;
    
    for (int i = 0; i < N; i++) {
        particles[i].position = gaussian(0.0, 2.0);
        particles[i].weights = 1.0 / N;
    }

    return 0;
}

// сдвиг каждой частицы на случайную величину
int predict(Particle* particles, int N, double Q) {
    if (particles == NULL || N <= 0 || Q < 0)
        return -1;

    for (int i = 0; i < N; i++)
        particles[i].position += gaussian(0.0, sqrt(Q));

    return 0;
}

// обновление веса частиц
// чем ближе частица к измерению z, тем больший вес она получит
int update_weights(Particle* particles, int N, double z, double R) {
    if (particles == NULL || N <= 0)
        return -1; 
    
    if (R <= 0.0)
        return -1;

    for (int i = 0; i < N; i++)
        particles[i].weights *= exp(-pow(z - particles[i].position, 2) / (2.0 * R));

    return 0;
}

// подсчет общей суммы весов
// затем деление веса каждой частицы на эту сумму
int normalize_weights(Particle* particles, int N) {
    if (particles == NULL || N <= 0)
        return -1;
   
    double sum = 0;
    for (int i = 0; i < N; i++)
        sum += particles[i].weights;

    if (sum < 1e-10) { 
        return -1; 
    }
    for (int i = 0; i < N; i++)
        particles[i].weights /= sum;
    
    return 0;
}

// заменяет набор частиц новым, где частицы с большим весом копируются чаще, с малым — исчезают
// после замены веса всех частиц снова равны
int resample(Particle* particles, int N) {
    if (particles == NULL || N <= 0)
        return -1;

    Particle* temp = (Particle*)malloc(N * sizeof(Particle));
    double* cumsum = (double*)malloc(N * sizeof(double));

    // проверка нехватки памяти
    if (temp == NULL || cumsum == NULL) {
        free(temp);  
        free(cumsum);
        return -1; 
    }
    
    // кумулятивная сумма весов
    cumsum[0] = particles[0].weights;
    for (int i = 1; i < N; i++)
        cumsum[i] = cumsum[i-1] + particles[i].weights;
    
    // вычисляем полезность частиц пропорционально весу
    double step = 1.0 / N;
    double start = ((double)rand() / RAND_MAX) * step;
    
    int j = 0;
    for (int i = 0; i < N; i++) {
        double pos = start + i * step;
        while (pos > cumsum[j] && j < N - 1) 
            j++;
        temp[i] = particles[j];
        temp[i].weights = 1.0 / N;
    }
    
    for (int i = 0; i < N; i++)
        particles[i] = temp[i];

    free(temp);
    free(cumsum);
    return 0;
}

// вычисление возможного положения объекта как средневзвешенного значения набора частиц
double estimate_position(Particle* particles, int N) {
    if (particles == NULL || N <= 0)
        return 0.0;

    double estimate = 0;
    for (int i = 0; i < N; i++)
        estimate += particles[i].position * particles[i].weights;
    return estimate; 
}
