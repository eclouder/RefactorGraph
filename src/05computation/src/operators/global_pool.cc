﻿#include "computation/operators/global_pool.h"
#include "refactor/common.h"

namespace refactor::computation {

    size_t GlobalPool::typeId(PoolType type) noexcept {
        switch (type) {
            case PoolType::Average: {
                static uint8_t ID = 1;
                return reinterpret_cast<size_t>(&ID);
            }
            case PoolType::Lp: {
                static uint8_t ID = 1;
                return reinterpret_cast<size_t>(&ID);
            }
            case PoolType::Max: {
                static uint8_t ID = 1;
                return reinterpret_cast<size_t>(&ID);
            }
            default:
                UNREACHABLE();
        }
    }
    size_t GlobalPool::opTypeId() const noexcept {
        return typeId(type);
    }
    std::string_view GlobalPool::name() const noexcept {
        switch (type) {
            case PoolType::Average:
                return "GlobalAveragePool";
            case PoolType::Lp:
                return "GlobalLpPool";
            case PoolType::Max:
                return "GlobalMaxPool";
            default:
                UNREACHABLE();
        }
    }

}// namespace refactor::computation
