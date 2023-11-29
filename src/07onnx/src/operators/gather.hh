﻿#ifndef ONNX_GATHER_HH
#define ONNX_GATHER_HH

#include "frontend/operator.h"

namespace refactor::onnx {
    using namespace frontend;

    struct Gather final : public Operator {
        Int axis;

        explicit Gather(Int);

        static OpBox build(ModelContext const &, std::string_view, Attributes);
        static size_t typeId();

        size_t opTypeId() const final;
        std::string_view opTypeName() const final;
        InferResult infer(TensorRefs, InferOptions const &) const final;
        computation::OpBox lower(TensorRefs) const final;
    };

}// namespace refactor::onnx

#endif// ONNX_GATHER_ELEMENTS_HH
