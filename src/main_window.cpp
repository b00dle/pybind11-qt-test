#include "main_window.h"

#include <QDebug>
#include <QTimer>
#include <QTextStream>
#include <QVBoxLayout>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

#include "py-wrappers/py_http_types.h"
#include "http_types.h"

#define TRIGGER_INTERVAL 1000

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , text_edit_(new QTextEdit(this))
    , button_py_exec_(new QPushButton(tr("execute Python request"), this))
    , button_qt_exec_(new QPushButton(tr("execute Qt request"), this))
    , network_access_(new QNetworkAccessManager(this))
    , api_token_()
{
    initWidgets();
    initLayout();
    loadApiToken();
}

MainWindow::~MainWindow()
{

}

void MainWindow::callPythonHttpRequest()
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

void MainWindow::callQtHttpRequest()
{
    QNetworkRequest request;
    request.setUrl(QUrl("https://0.0.0.0:5000/api/sound?access_token="+api_token_));
    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(conf);
    network_access_->get(request);
}

void MainWindow::handleQtHttpReply(QNetworkReply* reply)
{
    QString* content = new QString();
    QTextStream text_stream;
    text_stream.setString(content);
    {
        if (reply->error()) {
            text_stream << reply->errorString() << "\n";
            return;
        }

        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());

        QList<Sound> sounds;
        if(doc.isArray()) {
            for(auto val: doc.array()) {
                if(val.isObject()) {
                    sounds.append(Sound::fromJsonObject(val.toObject()));
                    text_stream << "<Sound"
                                << " local_path=" << sounds.back().local_path
                                << " name=" << sounds.back().resource.name
                                << " uuid=" << sounds.back().uuid << ">\n";
                }
            }
        }
    }
    text_edit_->append("\n" + text_stream.readAll());
    delete content;
}

void MainWindow::initWidgets()
{
    connect(button_py_exec_, &QPushButton::clicked,
            this, &MainWindow::callPythonHttpRequest);

    connect(button_qt_exec_, &QPushButton::clicked,
            this, &MainWindow::callQtHttpRequest);

    connect(network_access_, &QNetworkAccessManager::finished,
            this, &MainWindow::handleQtHttpReply);
}

void MainWindow::initLayout()
{
    text_edit_->setReadOnly(true);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(text_edit_);
    layout->addWidget(button_py_exec_);
    layout->addWidget(button_qt_exec_);
    QWidget* container = new QWidget(this);
    container->setLayout(layout);

    setCentralWidget(container);
}

void MainWindow::loadApiToken()
{
    QString val;
    QFile file;
    file.setFileName("../src/secret.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
    qDebug() << val;
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject obj = d.object();
    if(obj.contains("access_token"))
        api_token_ = obj["access_token"].toString();
}

