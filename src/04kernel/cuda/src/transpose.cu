﻿#include "kernel/cuda/transpose.cuh"
#include "macro.cuh"
#include <cstdint>

namespace refactor::kernel::cuda {

    __global__ static void transposeKernel(
        unsigned long long n,
        uint8_t const *data, transpose::DimStride const *strides, uint8_t *output,
        unsigned int rank,
        unsigned int eleSize) {
        extern __shared__ transpose::DimStride shared[];
        for (auto i = threadIdx.x; i < rank; i += blockDim.x) {
            shared[i] = strides[i];
        }
        __syncthreads();
        for (auto tid = blockIdx.x * blockDim.x + threadIdx.x,
                  step = blockDim.x * gridDim.x;
             tid < n;
             tid += step) {
            auto j = 0u, rem = tid;
            for (auto k = 0u; k < rank; ++k) {
                auto d = shared[k];
                j += rem / d.o * d.i;
                rem %= d.o;
            }

            optimizedMemcpy(output + tid * eleSize, data + j * eleSize, eleSize);
        }
    }

    void launchTranspose(
        KernelLaunchParameters const &params,
        void const *data, transpose::DimStride const *strides, void *output,
        unsigned int rank,
        unsigned int eleSize) {
        transposeKernel<<<
            params.gridSize,
            params.blockSize,
            rank * sizeof(transpose::DimStride),
            reinterpret_cast<cudaStream_t>(params.stream)>>>(
            params.n,
            reinterpret_cast<uint8_t const *>(data),
            strides,
            reinterpret_cast<uint8_t *>(output),
            rank,
            eleSize);
    }

}// namespace refactor::kernel::cuda
