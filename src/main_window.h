#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QElapsedTimer>
#include <QPushButton>
#include <QNetworkAccessManager>

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

namespace py = pybind11;
using namespace py::literals;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    void callPythonHttpRequest();
    void callQtHttpRequest();

public Q_SLOTS:
    void handleQtHttpReply(QNetworkReply* reply);

private:
    void initWidgets();
    void initLayout();
    void loadApiToken();

    QTextEdit* text_edit_;
    QPushButton* button_py_exec_;
    QPushButton* button_qt_exec_;

    // For Qt based Http access
    QNetworkAccessManager* network_access_;
    QString api_token_;
};
#endif // MAINWINDOW_H
