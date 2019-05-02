#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QDebug>
#include <QMessageBox>

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
        msg("图像: \"" + im_path + "\" 无法打开");
        return;
    }
    auto withCanny = _cvf.withCanny(ui->cb_blur->currentText().toInt(), ui->sb_threshold1->value(), ui->sb_threshold2->value(),
                                    ui->cb_origin_canny->isChecked());
    auto canny = _cvf.canny(ui->cb_blur->currentText().toInt(), ui->sb_threshold1->value(), ui->sb_threshold2->value(),
                            ui->cb_canny->isChecked());
    auto origin = _cvf.origin(ui->cb_origin->isChecked());
    auto mask = _cvf.mask(ui->cb_mask->isChecked());
    _area->setImage(withCanny);
    ui->la_canny->setPixmap(QPixmap::fromImage(canny.scaled(ui->la_canny->size(), Qt::KeepAspectRatio)));
    ui->la_origin->setPixmap(QPixmap::fromImage(origin.scaled(ui->la_canny->size(), Qt::KeepAspectRatio)));
    ui->la_mask->setPixmap(QPixmap::fromImage(mask.scaled(ui->la_canny->size(), Qt::KeepAspectRatio)));
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

void MainWindow::on_sb_threshold1_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    setImage();
}

void MainWindow::on_sb_threshold2_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    setImage();
}

void MainWindow::on_cb_blur_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    setImage();
}

void MainWindow::on_cb_origin_canny_clicked()
{
    if (ui->cb_origin_canny->isChecked())
        setImage();
    else
        _cvf.closeWindow(CVFunctions::WITH_CANNY);
}

void MainWindow::on_cb_origin_clicked()
{
    if (ui->cb_origin->isChecked())
        setImage();
    else
        _cvf.closeWindow(CVFunctions::ORIGIN);
}

void MainWindow::on_cb_canny_clicked()
{
    if (ui->cb_canny->isChecked())
        setImage();
    else
        _cvf.closeWindow(CVFunctions::CANNY);
}

void MainWindow::on_cb_mask_clicked()
{
    if (ui->cb_mask->isChecked())
        setImage();
    else
        _cvf.closeWindow(CVFunctions::MASK);
}
