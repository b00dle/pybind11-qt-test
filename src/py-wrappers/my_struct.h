#ifndef MYSTRUCT_H
#define MYSTRUCT_H

#include <string>
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

namespace py = pybind11;

class MyDictClass {
public:
    MyDictClass(const py::dict &data)
        : data_(data)
    {}
    virtual ~MyDictClass() = default;

protected:
    py::dict data_;
};

class DictNamedClass: MyDictClass {
public:
    DictNamedClass(const py::dict& data)
        : MyDictClass(data)
        , name("No Name")
    {
        parseData();
    }

    ~DictNamedClass() override = default;

    void setName(const std::string &name_) { name = name_; }

    const std::string &getName() const { return name; }

    std::string name;

protected:
    void parseData() {
        if(data_.contains("name"))
            name = data_["name"].cast<std::string>();
    }
};

struct KwargsNamedClass {
    KwargsNamedClass(std::string const &name, const py::kwargs& kwargs): name(name) {}

    std::string name;
};

PYBIND11_EMBEDDED_MODULE(example, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    py::class_<DictNamedClass>(m, "DictNamedClass")
            .def(py::init<const py::dict&>(), py::arg("data")=py::dict())
            .def("setName", &DictNamedClass::setName)
            .def("getName", &DictNamedClass::getName)
            .def("__repr__",
                [](const DictNamedClass &a) {
                    return "<NamedStruct named '" + a.name + "'>";
                }
            );

    py::class_<KwargsNamedClass>(m, "KwargsNamedClass")
            .def(py::init<const std::string&, const py::kwargs&>(), py::arg("name")="No Name")
            .def_readwrite("name", &KwargsNamedClass::name)
            .def("__repr__",
                [](const KwargsNamedClass &a) {
                    return "<NamedStruct named '" + a.name + "'>";
                }
            );
}

#endif // MYSTRUCT_H
