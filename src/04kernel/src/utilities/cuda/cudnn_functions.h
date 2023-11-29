﻿#ifndef KERNEL_CUDNN_FUNCTIONS_H
#define KERNEL_CUDNN_FUNCTIONS_H

#include "common.h"
#include <cudnn.h>

#define CUDNN_ASSERT(STATUS)                                      \
    if (auto status = (STATUS); status != CUDNN_STATUS_SUCCESS) { \
        fmt::println("cudnn failed on \"" #STATUS "\" with {}",   \
                     cudnnGetErrorString(status));                \
        abort();                                                  \
    }

namespace refactor::kernel::cudnn {

    cudnnDataType_t cudnnDataTypeConvert(DataType);

    // A helper function that set CuDNN tensor descriptor given tensor shape and type
    void setCudnnTensor(cudnnTensorDescriptor_t, DataType, slice_t<int>);

}// namespace refactor::kernel::cudnn

#endif// KERNEL_CUDNN_FUNCTIONS_H
