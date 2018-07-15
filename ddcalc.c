#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

void ddcalc(float* data, int dataCount, int dataDim, float* distances) {
    for(int from = 0; from < dataCount; ++from) {
        for(int to = from + 1; to < dataCount; to++) {
            float distance = 0;
            for (int i = 0; i < dataDim; ++i) {
                distance += pow(data[from * dataDim + i] - data[to * dataDim + i], 2);
            }
            distance = sqrt(distance);
            distances[from * dataDim + to] = distance;
        }
    }
}

int main(int argc, char const *argv[])
{
    clock_t start, end;

    int elements = 20000;
    int dimension = 5;



    start = clock();
    srand(time(NULL));
    float* data = (float*) malloc(sizeof(float) * elements * dimension);
    float* distances = (float*) malloc(sizeof(float) * elements * elements);
    for(int i = 0; i < elements * dimension; ++i) data[i] = rand() / (float) RAND_MAX;
    end = clock();
    printf("data build time: %f\n", (end - start) / (double) CLOCKS_PER_SEC);

    start = clock();
    ddcalc(data, elements, dimension, distances);
    end = clock();
    printf("ddcalc time: %f\n", (end - start) / (double) CLOCKS_PER_SEC);

    return 0;
}
