#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QElapsedTimer>

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

private:
    void initWidgets();
    void initLayout();

    QElapsedTimer run_time_;
    QTimer* frame_trigger_;
    QTimer* monitoring_trigger_;
    QTextEdit* text_edit_;
    int frame_count_;
};
#endif // MAINWINDOW_H
