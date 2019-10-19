#ifndef MYSTRUCT_H
#define MYSTRUCT_H

#include <string>
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

namespace py = pybind11;

struct MyStruct {
    MyStruct(const std::string &name) : name(name) { }

    void setName(const std::string &name_) { name = name_; }

    const std::string &getName() const { return name; }

    std::string name;

    static void importInto(py::module& m) {
        py::class_<MyStruct>(m, "MyStruct")
                .def(py::init<const std::string &>())
                .def("setName", &MyStruct::setName)
                .def("getName", &MyStruct::getName)
                .def("__repr__",
                    [](const MyStruct &a) {
                        return "<Pet named '" + a.name + "'>";
                    }
                );
    }
};

#endif // MYSTRUCT_H
