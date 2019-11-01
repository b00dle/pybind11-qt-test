#include "main_window.h"

#include <QDebug>
#include <QTimer>
#include <QTextStream>

#include "py-wrappers/my_struct.h"

#define TRIGGER_INTERVAL 1000

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , run_time_()
    , frame_trigger_(new QTimer(this))
    , monitoring_trigger_(new QTimer(this))
    , text_edit_(new QTextEdit(this))
    , frame_count_(0)
{
    initWidgets();
    initLayout();
}

MainWindow::~MainWindow()
{

}

void MainWindow::initWidgets()
{
    connect(frame_trigger_, &QTimer::timeout, this, [=]{
        QString* content = new QString();
        QTextStream text_stream;
        text_stream.setString(content);
        frame_count_ += 1;
        //py::initialize_interpreter();
        {
            py::scoped_interpreter guard{};
            py::module sys = py::module::import("sys");
            auto locals = py::dict("my_scripts"_a="/home/basti/Documents/git/pybind11-qt-test/src");
            py::exec(R"(
                import sys
                sys.path.insert(0, '{my_scripts}'.format(**locals()))
            )", py::globals(), locals);

            py::module my_test = py::module::import("my_test");

            py::object add_result = my_test.attr("add")(1, 2);
            int n = add_result.cast<int>();
            text_stream << "add result" << n;

            py::object list_result = my_test.attr("get_list")();
            py::list list = list_result.cast<py::list>();
            text_stream << "add result" << list[0].cast<int>();

            py::object dict_result = my_test.attr("get_dict")();
            py::dict dict = dict_result.cast<py::dict>();
            text_stream << "dict result" << dict["0"].cast<int>();

            py::object struct_result = my_test.attr("get_my_struct")();
            MyStruct s = struct_result.cast<MyStruct>();
            text_stream << "struct " << s.getName().c_str();
        }
        //py::finalize_interpreter();
        text_edit_->append("\n" + text_stream.readAll());
        delete content;
    });
    frame_trigger_->setInterval(TRIGGER_INTERVAL);
    frame_trigger_->start();


    connect(monitoring_trigger_, &QTimer::timeout, this, [=]{
        QString* content = new QString();
        QTextStream text_stream;
        text_stream.setString(content);

        qDebug() << "============= FPS " << frame_count_ << " =============";
        frame_count_ = 0;

        //text_edit_->append("\n" + text_stream.readAll());
        delete content;
    });
    monitoring_trigger_->setInterval(1000);
    monitoring_trigger_->start();

    run_time_.start();
}

void MainWindow::initLayout()
{
    text_edit_->setReadOnly(true);
    setCentralWidget(text_edit_);
}

