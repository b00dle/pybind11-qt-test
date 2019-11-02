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
#include <QHeaderView>

#include "http_types.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , button_refresh_(new QPushButton(tr("refresh"), this))
    , sound_view_(new SoundTableView(this))
{
    initWidgets();
    initLayout();
}

MainWindow::~MainWindow()
{

}

void MainWindow::initWidgets()
{
    sound_view_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(button_refresh_, &QPushButton::clicked,
            sound_view_, &SoundTableView::update);
}

void MainWindow::initLayout()
{
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(sound_view_);
    layout->addWidget(button_refresh_);
    QWidget* container = new QWidget(this);
    container->setLayout(layout);
    setCentralWidget(container);
}

