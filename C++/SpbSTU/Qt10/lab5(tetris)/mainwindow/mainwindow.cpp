#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    this->setWindowFlag(Qt::WindowStaysOnTopHint);
    ui->setupUi(this);
    if(!connect(ui->actionNew_game, &QAction::triggered, ui->gameWidget, &Glass::slotNewGame)){
        qDebug() << "No New game connection";
    }
    if(!connect(ui->actionInfo_3, &QAction::triggered, this, &MainWindow::slotInfo)){
        qDebug() << "No Info connection";
    }
    if(!connect(ui->gameWidget, &Glass::signalSizeComputed, this, &MainWindow::slotSetSize)){
        qDebug() << "No Fixed size connection";
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::slotSetSize(){
    this->setFixedSize(this->sizeHint());
}

void MainWindow::slotInfo(){
    QMessageBox::information(this, QStringLiteral("Information"),
                             QStringLiteral("Left/Right Arrow - Move\n"
                                            "Up/Down Arrow - Rotate Color\n"
                                            "Space - Fast throw"));
}

