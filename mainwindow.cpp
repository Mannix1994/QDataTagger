#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QDebug>
#include<QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), _area(new PaintWidget())
{
    ui->setupUi(this);
    ui->scrollArea->setWidget(_area);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _area;
}

/**
 * @brief MainWindow::setImage 获取当前
 * @param im_path
 */
void MainWindow::setImage(QString im_path="")
{
    if (im_path==""){
        im_path = _imageList.current();
    }
    _cvf.open(im_path);
}

void MainWindow::on_pb_open_clicked()
{
    QString home_path = QStandardPaths::standardLocations(QStandardPaths::HomeLocation)[0];
    QString list_file = QFileDialog::getOpenFileName(this, QString(),home_path,"TXT (*.txt)");
    qDebug() << list_file;
    _imageList.open(list_file);
}

void MainWindow::on_pb_pre_clicked()
{
    auto im_path = _imageList.pre();
    qDebug() << im_path;
    if(!im_path.length()){
        QMessageBox::information(this, "提示", "没有上一张了啦", QMessageBox::Ok);
    }
}

void MainWindow::on_pb_next_clicked()
{
    auto im_path = _imageList.next();
    qDebug() << im_path;
    if(!im_path.length()){
        QMessageBox::information(this, "提示", "没有下一张了啦", QMessageBox::Ok);
    }
}
