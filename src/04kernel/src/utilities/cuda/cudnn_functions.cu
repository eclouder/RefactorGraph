﻿#include "cudnn_functions.h"

namespace refactor::kernel::cudnn {
    using DT = DataType;

    cudnnDataType_t cudnnDataTypeConvert(DT dataType) {
        switch (dataType) {
                // clang-format off
            case DT::F32 : return CUDNN_DATA_FLOAT;    break;
            case DT::F64 : return CUDNN_DATA_DOUBLE;   break;
            case DT::FP16: return CUDNN_DATA_HALF;     break;
            case DT::I8  : return CUDNN_DATA_INT8;     break;
            case DT::I32 : return CUDNN_DATA_INT32;    break;
            case DT::U8  : return CUDNN_DATA_UINT8;    break;
            case DT::BF16: return CUDNN_DATA_BFLOAT16; break;
            case DT::I64 : return CUDNN_DATA_INT64;    break;
            case DT::Bool: return CUDNN_DATA_BOOLEAN;  break;
            default: UNREACHABLEX(void, "Unsupported data type");
                // clang-format on
        }
    }

}// namespace refactor::kernel::cudnn
