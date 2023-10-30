#include "kernel/collectors/where.h"
#include "../kernels/where/cpu_kernel.hh"
#include "../kernels/where/where_cuda.hh"

namespace refactor::kernel {

    std::vector<KernelBox>
    WhereCollector::filter(TensorRefs inputs, TensorRefs) const {
        std::vector<KernelBox> ans;
        switch (target) {
            case Target::Cpu:
                if (auto ptr = WhereCpu::build(inputs); ptr) {
                    ans.emplace_back(std::move(ptr));
                }
                break;
            case Target::NvidiaGpu:
                if (auto ptr = WhereCuda::build(inputs); ptr) {
                    ans.emplace_back(std::move(ptr));
                }
                break;
            default:
                UNREACHABLEX(void, "Unknown target");
        }
        return ans;
    }

}// namespace refactor::kernel
