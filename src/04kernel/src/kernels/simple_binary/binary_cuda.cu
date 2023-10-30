﻿#include "binary_cuda.hh"
#include <cuda.h>

namespace refactor::kernel {
    using K = BinaryCuda;
    using Op = SimpleBinaryType;
    using DT = DataType;

#define KERNEL(NAME, OP)                                                        \
    template<typename T>                                                        \
    __global__ void _kernel_##NAME(T const *a, T const *b, T *c, size_t size) { \
        size_t tid = threadIdx.x + blockIdx.x * blockDim.x;                     \
        if (tid < size) {                                                       \
            c[tid] = a[tid] OP b[tid];                                          \
        }                                                                       \
    }

#define KERNEL_CONST(NAME, OP)                                                        \
    template<typename T>                                                              \
    __global__ void _kernel_const_##NAME(T const *a, T const *b, T *c, size_t size) { \
        size_t tid = threadIdx.x + blockIdx.x * blockDim.x;                           \
        if (tid < size) {                                                             \
            c[tid] = a[tid] OP b[0];                                                  \
        }                                                                             \
    }

    KERNEL(Add, +)
    KERNEL(Sub, -)
    KERNEL(Mul, *)
    KERNEL(Div, /)
    KERNEL_CONST(Add, +)
    KERNEL_CONST(Sub, -)
    KERNEL_CONST(Mul, *)
    KERNEL_CONST(Div, /)

#define CASE_DT(NAME, T)                                                                                            \
    case DT::T:                                                                                                     \
        return [n = this->size, constB = this->constB](runtime::Resources &, void const **inputs, void **outputs) { \
            using T_ = primitive_t<DT::T>::type;                                                                    \
            auto a = static_cast<T_ const *>(inputs[0]);                                                            \
            auto b = static_cast<T_ const *>(inputs[1]);                                                            \
            auto c = static_cast<T_ *>(outputs[0]);                                                                 \
            size_t blocksize = 1024;                                                                                \
            size_t gridsize = (n + blocksize - 1) / blocksize;                                                      \
            if (constB) {                                                                                           \
                _kernel_const_##NAME<<<gridsize, blocksize>>>(a, b, c, n);                                          \
            } else {                                                                                                \
                _kernel_##NAME<<<gridsize, blocksize>>>(a, b, c, n);                                                \
            }                                                                                                       \
        }

#define CASE_OP(NAME)                \
    case Op::NAME:                   \
        switch (dataType.internal) { \
            CASE_DT(NAME, F32);      \
            CASE_DT(NAME, U8);       \
            CASE_DT(NAME, I8);       \
            CASE_DT(NAME, U16);      \
            CASE_DT(NAME, I16);      \
            CASE_DT(NAME, I32);      \
            CASE_DT(NAME, I64);      \
            CASE_DT(NAME, F64);      \
            CASE_DT(NAME, U32);      \
            CASE_DT(NAME, U64);      \
            default:                 \
                UNREACHABLE();       \
        }

    auto K::lower() const noexcept -> Routine {
        switch (opType) {
            CASE_OP(Add)
            CASE_OP(Sub)
            CASE_OP(Mul)
            CASE_OP(Div)
            default:
                UNREACHABLE();
        }
    }
}// namespace refactor::kernel
