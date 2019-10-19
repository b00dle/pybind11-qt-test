#include "main_window.h"

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

#include <QApplication>
#include <QDebug>

namespace py = pybind11;
using namespace py::literals;

int main(int argc, char *argv[])
{
    {
        py::scoped_interpreter guard{};

        py::module sys = py::module::import("sys");

        auto locals = py::dict("my_scripts"_a="/home/basti/Documents/code/pybind11-qt-test/pybind11-qt-test");
        py::exec(R"(
            import sys
            sys.path.insert(0, '{my_scripts}'.format(**locals()))
        )", py::globals(), locals);

        py::print(sys.attr("path"));

        py::module my_test = py::module::import("my_test");
        py::object add_result = my_test.attr("add")(1, 2);
        int n = add_result.cast<int>();
        qDebug() << "add result" << n;

        py::object list_result = my_test.attr("get_list")();
        py::list list = list_result.cast<py::list>();
        qDebug() << "add result" << list[0].cast<int>();

        py::object dict_result = my_test.attr("get_dict")();
        py::dict dict = dict_result.cast<py::dict>();
        qDebug() << "dict result" << dict["0"].cast<int>();
    }
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
