#include "py-wrappers/py_http_types.h"

PyHttpResponse::PyHttpResponse(int status_code, std::string message, const pybind11::object &data)
    : status_code(status_code)
    , message(message)
    , data(data)
{}

bool PyHttpResponse::dataIsNone() const
{
    return data.is_none();
}

bool PyHttpResponse::statusIsOk() const
{
    return status_code == 200;
}

PySound::PySound(const std::string &local_path, const std::string &uuid, const pybind11::kwargs &kwargs)
    : resource()
    , local_path(local_path)
    , uuid(uuid)
{}

PyResource::PyResource()
    : name()
    , type()
    , uuid()
{}

PyResource::PyResource(const std::string &name, int type, const std::string &uuid, const pybind11::kwargs &)
    : name(name)
    , type(type)
    , uuid(uuid)
{}

PYBIND11_EMBEDDED_MODULE(companion, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    py::class_<PyHttpResponse>(m, "PyHttpResponse")
            .def(py::init<int, const std::string&, const py::object&>(),
                 py::arg("status_code"),
                 py::arg("message")=std::string(),
                 py::arg("data")=py::none())
            .def_readwrite("status_code", &PyHttpResponse::status_code)
            .def_readwrite("data", &PyHttpResponse::data)
            .def_readwrite("message", &PyHttpResponse::message);

    py::class_<PySound>(m, "PySound")
            .def(py::init<const std::string&, const std::string&, const py::kwargs&>(),
                 py::arg("local_path"),
                 py::arg("uuid"))
            .def_readwrite("name", &PySound::local_path)
            .def_readwrite("resource", &PySound::resource)
            .def_readwrite("uuid", &PySound::uuid);

    py::class_<PyResource>(m, "PyResource")
            .def(py::init<const std::string&, int, const std::string&, const py::kwargs&>(),
                 py::arg("name"),
                 py::arg("type"),
                 py::arg("uuid"))
            .def_readwrite("name", &PyResource::name)
            .def_readwrite("uuid", &PyResource::uuid)
            .def_readwrite("type", &PyResource::type);
}
