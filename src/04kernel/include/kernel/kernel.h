﻿#ifndef KERNEL_KERNEL_H
#define KERNEL_KERNEL_H

#include "runtime/resource.h"
#include <absl/container/inlined_vector.h>
#include <functional>
#include <memory>
#include <string_view>

namespace refactor::kernel {
    using Addresses = absl::InlinedVector<void *, 2>;
    using Operation = std::function<void(runtime::Resources &, Addresses, Addresses)>;

    class Kernel {
    public:
        virtual size_t kernelTypeId() const = 0;
        virtual std::string_view description() const = 0;
        virtual Operation lower() const = 0;
    };

    using KernelBox = std::unique_ptr<Kernel>;

}// namespace refactor::kernel

#endif// KERNEL_KERNEL_H
