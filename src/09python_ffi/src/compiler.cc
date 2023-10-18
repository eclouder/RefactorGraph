﻿#include "compiler.h"
#include "refactor/common.h"
#include <execution>

namespace refactor::python_ffi {
    using namespace frontend;
    namespace py = pybind11;

    Compiler::Compiler(frontend::Graph g) : _g(std::move(g)) {}

    void
    Compiler::substitute(CStr name, int64_t value) {
        ASSERT(_g.substitute(name, value),
               fmt::format("Variable {} not exist", name));
    }

    void
    Compiler::setInput(size_t index, int dataType, DimVec dims) {
        ASSERT(index < _g.internal().topology.globalInputsCount(),
               fmt::format("Input {} not exist", index));

        auto dataType_ = *DataType::parse(dataType);
        _g.internal().edges[index].tensor = Tensor::share(dataType_, dimVec2Shape(dims), {});
    }

    std::unordered_set<std::string>
    Compiler::fillEdgeInfo(bool calculate) { return _g.fillEdgeInfo(calculate); }

    std::shared_ptr<Executor>
    Compiler::compile(bool calculate, std::string target) {
        _g.collectVariables();
        std::vector<std::string_view> unknownVariables;
        for (auto const &[_, v] : _g.variables()) {
            if (!v->value.has_value()) {
                unknownVariables.emplace_back(v->name);
            }
        }
        if (!unknownVariables.empty()) {
            std::string msg = "Unknown variables: [ ";
            for (auto const &v : unknownVariables) {
                msg += v;
                msg += ' ';
            }
            msg += ']';
            RUNTIME_ERROR(std::move(msg));
        }
        _g.fillEdgeInfo(true);

        auto computation = _g.lower();
        computation.layoutPermute();

        kernel::Target target_ = kernel::Target::Cpu;
        if (target == "cpu") {
            target_ = kernel::Target::Cpu;
        } else if (target == "cuda") {
            target_ = kernel::Target::NvidiaGpu;
        } else {
            UNREACHABLE();
        }

        return std::make_shared<Executor>(std::move(computation), target_);
    }

    std::optional<py::array>
    Compiler::getTensor(CStr name) const {
        auto const &edges = _g.internal().edges;
        auto it = std::find_if(edges.begin(), edges.end(),
                               [name](auto const &edge) { return edge.name == name; });
        if (it == edges.end()) { return std::nullopt; }
        auto const &tensor = *it->tensor;

        std::vector<int64_t> shape(tensor.shape.size());
        std::transform(std::execution::unseq,
                       tensor.shape.begin(), tensor.shape.end(), shape.begin(),
                       [](auto const &d) { return d.value(); });

        auto ans = py::array(buildNumpyDType(tensor.dataType), std::move(shape), nullptr);
        if (tensor.data) { std::memcpy(ans.mutable_data(), tensor.data->get<void>(), ans.nbytes()); }
        return ans;
    }

}// namespace refactor::python_ffi
