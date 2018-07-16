#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <CL/cl.h>


int main(int argc, char const *argv[])
{
    clock_t start, end;

    start = clock();
    cl_int err;
    cl_platform_id platform;
    err = clGetPlatformIDs( 1, &platform, NULL );
    cl_device_id device;
    err = clGetDeviceIDs( platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL );
    cl_context_properties props[3] = { CL_CONTEXT_PLATFORM, 0, 0 };
    cl_context ctx;
    props[1] = (cl_context_properties)platform;
    ctx = clCreateContext( props, 1, &device, NULL, NULL, &err );
    cl_command_queue queue = clCreateCommandQueue( ctx, device, 0, &err );
    const char *kernel_code =
    "__kernel void ddcalc("
    "    __global const float* data,"
    "    const int dataCount,"
    "    const int dataDim,"
    "    __global float* distances"
    ") {"
    "    int from = get_global_id(0);"
    "    int to = get_global_id(1);"
    "    float distance = 0;"
    "    for (int i = 0; i < dataDim; ++i) {"
    "        distance += pow(data[from * dataDim + i] - data[to * dataDim + i], 2);"
    "    }"
    "    distances[from * dataCount + to] = sqrt(distance);"
    "}";
    cl_program program = clCreateProgramWithSource(ctx, 1, (const char **) &kernel_code, NULL, &err);
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    cl_event event = NULL;
    cl_kernel ddcalc= clCreateKernel(program, "ddcalc", &err);
    end = clock();
    printf("queue and kernel time: %f\n", (end - start) / (double) CLOCKS_PER_SEC);

    int elements = 20000;
    int dimension = 5;

    start = clock();
    srand(time(NULL));
    float* data = (float*) malloc(sizeof(float) * elements * dimension);
    float* distances = (float*) malloc(sizeof(float) * elements * elements);
    for(int i = 0; i < elements * dimension; ++i) data[i] = rand() / (float) RAND_MAX;

    cl_mem data_gpu = clCreateBuffer(ctx, CL_MEM_READ_WRITE, sizeof(float) * elements * dimension, NULL, &err );
    err = clEnqueueWriteBuffer( queue, data_gpu, CL_TRUE, 0, sizeof(float) * elements * dimension, data, 0, NULL, NULL );
    cl_mem distances_gpu = clCreateBuffer(ctx, CL_MEM_READ_WRITE, sizeof(float) * elements * elements, NULL, &err );
    end = clock();
    printf("data build time: %f\n", (end - start) / (double) CLOCKS_PER_SEC);
    
    start = clock();
    clSetKernelArg(ddcalc, 0, sizeof(cl_mem), &data_gpu);
    clSetKernelArg(ddcalc, 1, sizeof(int), &elements);
    clSetKernelArg(ddcalc, 2, sizeof(int), &dimension);
    clSetKernelArg(ddcalc, 3, sizeof(cl_mem), &distances_gpu);
    size_t* kernelSize = malloc(sizeof(size_t) * 2);
    kernelSize[0] = kernelSize[1] = elements;
    clEnqueueNDRangeKernel(queue, ddcalc, 2, NULL, kernelSize, NULL, 0, NULL, NULL);
    err = clFinish(queue);
    end = clock();
    printf("ddcalc time: %f\n", (end - start) / (double) CLOCKS_PER_SEC);


    start = clock();
    err = clEnqueueReadBuffer(queue, distances_gpu, CL_TRUE, 0, sizeof(float) * elements * elements, distances, 0, NULL, NULL);
    err = clFinish(queue);
    end = clock();
    printf("copy back time: %f\n", (end - start) / (double) CLOCKS_PER_SEC);
}
