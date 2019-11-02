#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

#include "view/sound_table_view.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initWidgets();
    void initLayout();

    QPushButton* button_refresh_;
    SoundTableView* sound_view_;
};
#endif // MAINWINDOW_H
