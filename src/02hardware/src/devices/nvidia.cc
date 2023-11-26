﻿#include "hardware/devices/nvidia.h"
#include "hardware/mem_pool.h"

namespace refactor::hardware {

    Nvidia::Nvidia(std::string_view deviceTypeName,
                   int32_t typeId,
                   int32_t cardId)
        : Device(deviceTypeName,
                 typeId,
                 cardId,
                 nullptr) {}

    Arc<Device> Nvidia::build(
        std::string_view deviceTypeName,
        int32_t typeId,
        int32_t cardId,
        std::string_view args) {
        return Arc<Device>(new Nvidia(deviceTypeName, typeId, cardId));
    }

}// namespace refactor::hardware
