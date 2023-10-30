﻿#ifndef KERNEL_SPLIT_CPU_KERNEL_HH
#define KERNEL_SPLIT_CPU_KERNEL_HH

#include "kernel/attributes/split_info.h"
#include "kernel/kernel.h"

namespace refactor::kernel {

    struct SplitCpu final : public Kernel {
        SplitInfo info;

        explicit SplitCpu(SplitInfo) noexcept;

        static KernelBox build(SplitInfo) noexcept;
        static size_t typeId() noexcept;

        size_t kernelTypeId() const noexcept final;
        std::string_view description() const noexcept final;
        Routine lower() const noexcept final;
    };

}// namespace refactor::kernel

#endif// KERNEL_SPLIT_CPU_KERNEL_HH
