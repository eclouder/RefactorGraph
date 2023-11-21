﻿#include "cuda_kernel.hh"
#include "kernel/cuda/split.cuh"
#include "mem_manager/foreign_blob.hh"
#include "runtime/mem_manager.hh"
#include <thrust/device_vector.h>

namespace refactor::kernel {
    using namespace runtime;

    auto SplitCuda::lower(Resources &) const noexcept -> RoutineWorkspace {
        auto sub = std::min(info.submultiple(), 16u);
        return [segments = thrust::device_vector<dim_t>(info.segments.begin(), info.segments.end()),
                params = cuda::ThreadsDistributer()(info.blockCount * info.sum / sub),
                sum = info.sum / sub,
                sub](Resources &res, void *workspace, void const *const *inputs, void *const *outputs) {
            auto size = segments.size() * sizeof(void *);
            auto outputs_ = mem_manager::ForeignBlob::share(res.fetch<MemManager>()->manager, size);
            outputs_->copyIn(outputs, size);
            cuda::launchSplit(
                params,
                inputs[0],
                segments.data().get(),
                reinterpret_cast<void **>((void *) *outputs_),
                segments.size(),
                sum,
                sub);
        };
    }

}// namespace refactor::kernel
