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
void MainWindow::setImage(QString im_path)
{
    if (im_path==""){
        im_path = _imageList.current();
    }
    if(!_cvf.open(im_path)){
        msg("图像：" + im_path + "无法打开");
    }
    _area->setImage(_cvf.withCanny(ui->sb_threshold1->value(), ui->sb_threshold2->value()));
}

void MainWindow::msg(const QString &content)
{
    msg("提示", content);
}

void MainWindow::msg(const QString &title, const QString &content)
{
    QMessageBox::information(this, title, content, QMessageBox::Ok);
}

void MainWindow::on_pb_open_clicked()
{
    QString home_path = QStandardPaths::standardLocations(QStandardPaths::HomeLocation)[0];
    QString list_file = QFileDialog::getOpenFileName(this, QString(),home_path,"TXT (*.txt)");
    qDebug() << list_file;
    _imageList.open(list_file);
    setImage();
}

void MainWindow::on_pb_pre_clicked()
{
    auto im_path = _imageList.pre();
    qDebug() << im_path;
    if(!im_path.length()){
        msg("没有上一张了啦");
        return;
    }
    setImage(im_path);
}

void MainWindow::on_pb_next_clicked()
{
    auto im_path = _imageList.next();
    qDebug() << im_path;
    if(!im_path.length()){
        msg("没有下一张了啦");
        return;
    }
    setImage(im_path);
}
