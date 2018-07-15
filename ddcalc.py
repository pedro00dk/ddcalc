import numpy as np
import pyopencl as cl
import time


context = cl.create_some_context()

start = time.time()
queue = cl.CommandQueue(context)
program = cl.Program(
    context,
    '''
    __kernel void ddcalc(
        __global const float* data,
        const int dataCount,
        const int dataDim,
        __global float* distances
    ) {
        int from = get_global_id(0);
        int to = get_global_id(1);

        float distance = 0;
        for (int i = 0; i < dataDim; ++i) {
            distance += pow(data[from * dataDim + i] - data[to * dataDim + i], 2);
        }
        distances[from * dataCount + to] = sqrt(distance);
    }
    '''
).build()
end = time.time()
print(f'queue and kernel time: {end - start}')

elements = 20_000
dimension = 5

start = time.time()
data = np.random.rand(elements, dimension).astype(np.float32)
data_gpu = cl.Buffer(context, cl.mem_flags.READ_ONLY | cl.mem_flags.COPY_HOST_PTR, hostbuf=data)
distances = np.empty(elements ** 2, np.float32).reshape((elements, elements))
distances_gpu = cl.Buffer(context, cl.mem_flags.WRITE_ONLY, distances.nbytes)
end = time.time()
print(f'data build time: {end - start}')

start = time.time()
program.ddcalc(queue, (elements, elements), None, data_gpu, np.int32(elements), np.int32(dimension), distances_gpu)
end = time.time()
print(f'ddcalc time: {end - start}')

start = time.time()
cl.enqueue_copy(queue, distances, distances_gpu)
end = time.time()
print(f'copy back time: {end - start}')
