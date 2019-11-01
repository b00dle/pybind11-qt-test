#ifndef PY_HTTP_TYPES_H
#define PY_HTTP_TYPES_H

#include <string>
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

namespace py = pybind11;

struct PyHttpResponse {
    PyHttpResponse(int status_code, std::string message, const py::object& data);

    bool dataIsNone() const;

    bool statusIsOk() const;

    int status_code;
    std::string message;
    py::object data;
};

struct PyResource {
    PyResource();
    PyResource(const std::string& name, int type, std::string const& uuid, const py::kwargs&);

    std::string name;
    int type;
    std::string uuid;
};

struct PySound {
    PySound(const std::string& local_path, const std::string& uuid, const py::kwargs& kwargs);

    PyResource resource;
    std::string local_path;
    std::string uuid;
};

#endif // PY_HTTP_TYPES_H
