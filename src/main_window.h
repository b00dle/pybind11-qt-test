#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QElapsedTimer>
#include <QPushButton>

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
    void callPythonCode();

private:
    void initWidgets();
    void initLayout();

    QTextEdit* text_edit_;
    QPushButton* button_;
};
#endif // MAINWINDOW_H
