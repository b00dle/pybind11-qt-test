#include "main_window.h"

#include <QDebug>
#include <QTimer>
#include <QTextStream>
#include <QVBoxLayout>

#include "py-wrappers/py_http_types.h"

#define TRIGGER_INTERVAL 1000

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , text_edit_(new QTextEdit(this))
    , button_(new QPushButton(tr("execute"), this))
{
    initWidgets();
    initLayout();
}

MainWindow::~MainWindow()
{

}

void MainWindow::callPythonCode()
{
    QString* content = new QString();
    QTextStream text_stream;
    text_stream.setString(content);
    {
        py::scoped_interpreter guard{};

        // add script path to sys.path
        py::module sys = py::module::import("sys");
        auto locals = py::dict("my_scripts"_a="/home/basti/Documents/git/pybind11-qt-test/src");
        py::exec(R"(
            import sys
            sys.path.insert(0, '{my_scripts}'.format(**locals()))
        )", py::globals(), locals);

        // import module for client requests
        py::module companion_client = py::module::import("companion_client");

        // call get_sounds request
        py::object response_result = companion_client.attr("get_sounds")();

        // extract response object
        PyHttpResponse response = response_result.cast<PyHttpResponse>();
        text_stream << "=== HTTP RESPONSE ===\n"
                    << "status: " << response.status_code << "\n"
                    << "content: " <<response.message.c_str() << "\n";

        // parse response object content
        if(!response.dataIsNone()) {
            for(auto sound_obj: response.data.cast<py::list>()) {
                PySound sound = sound_obj.cast<PySound>();
                text_stream << "<Sound"
                            << " local_path=" << sound.local_path.c_str()
                            << " name=" << sound.resource.name.c_str()
                            << " uuid=" << sound.uuid.c_str() << ">\n";
            }
        }
    }
    text_edit_->append("\n" + text_stream.readAll());
    delete content;
}

void MainWindow::initWidgets()
{
    connect(button_, &QPushButton::clicked,
            this, &MainWindow::callPythonCode);
}

void MainWindow::initLayout()
{
    text_edit_->setReadOnly(true);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(text_edit_);
    layout->addWidget(button_);
    QWidget* container = new QWidget(this);
    container->setLayout(layout);

    setCentralWidget(container);
}

