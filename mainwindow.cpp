#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QDebug>
#include <QMessageBox>
#include <QScrollBar>
#include <QFileInfo>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), _area(new PaintWidget())
{
    ui->setupUi(this);
    ui->scrollArea->setWidget(_area);
    connect(_area, &PaintWidget::imageChanged, this, &MainWindow::on_image_changed);
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
    if(im_path==""){
        msg("检查是否打开包含图片列表的文件");
        return;
    }
    if(!_cvf.open(im_path)){
        msg("图像: \"" + im_path + "\" 无法打开");
        return;
    }
    ui->statusBar->showMessage(im_path);
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

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    static bool showScrollBar=true;
    if((e->modifiers() == Qt::ShiftModifier) && (e->key() == Qt::Key_S)){
        //如果是Shift键，按一次隐藏滑动条，再按一次显示滑动条
        if(showScrollBar){
            ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
            ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        }else{
            ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
        showScrollBar = !showScrollBar;
    }else if(e->key() == Qt::Key_A){
        on_pb_pre_clicked();
    }else if(e->key() == Qt::Key_D){
        on_pb_next_clicked();
    }
    e->accept();
}

void MainWindow::on_pb_open_clicked()
{
    QString home_path = QStandardPaths::standardLocations(QStandardPaths::HomeLocation)[0];
    QString list_file = QFileDialog::getOpenFileName(this, QString(),home_path,"TXT (*.txt)");
    ui->statusBar->showMessage(list_file);
    if (list_file == "")
        return;
    if (_imageList.open(list_file))
        setImage();
    else{
        msg("打开列表文件失败啦");
    }
}

void MainWindow::on_pb_pre_clicked()
{
    auto im_path = _imageList.pre();
    if(!im_path.length()){
        msg("没有上一张了啦");
        return;
    }
    setImage(im_path);
}

void MainWindow::on_pb_next_clicked()
{
    auto im_path = _imageList.next();
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



void MainWindow::on_sb_scale_valueChanged(int arg1)
{
    _area->setScale(arg1);
}

void MainWindow::on_sb_pensize_valueChanged(int arg1)
{
    _area->setPenSize(arg1);
}

void MainWindow::on_pb_cancel_clicked()
{
    _area->cancel();
}

void MainWindow::on_cb_mode_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "涂抹"){
        _area->setPenColor(qRgb(255, 255, 255));
        ui->sb_pensize->setValue(6);
    }else if(arg1 == "消除"){
        _area->setPenColor(qRgb(0, 255, 0));
        ui->sb_pensize->setValue(15);
    }else if(arg1 == "标记"){
        _area->setPenColor(qRgb(255, 255, 0));
        ui->sb_pensize->setValue(1);
    }
}

void MainWindow::on_image_changed()
{
    auto im = _area->grab().toImage();
    auto mask = _cvf.mask(im, ui->cb_mask->isChecked());
    ui->la_mask->setPixmap(QPixmap::fromImage(mask.scaled(ui->la_canny->size(), Qt::KeepAspectRatio)));
}

void MainWindow::on_cb_origin_canny_clicked(bool checked)
{
    if (checked)
    {
       if(_cvf.isOpened())
       {
           auto withCanny = _cvf.withCanny(ui->cb_blur->currentText().toInt(), ui->sb_threshold1->value(),
                                           ui->sb_threshold2->value(), true);
       }
    }
    else
        _cvf.closeWindow(CVFunctions::WITH_CANNY);
}

void MainWindow::on_cb_origin_clicked(bool checked)
{
    if (checked)
    {
        if(_cvf.isOpened())
        {
            auto origin = _cvf.origin(true);
            ui->la_origin->setPixmap(QPixmap::fromImage(origin.scaled(ui->la_canny->size(), Qt::KeepAspectRatio)));
        }
    }
    else
        _cvf.closeWindow(CVFunctions::ORIGIN);
}

void MainWindow::on_cb_canny_clicked(bool checked)
{
    if (checked){
        if(_cvf.isOpened())
        {
            auto canny = _cvf.canny(ui->cb_blur->currentText().toInt(), ui->sb_threshold1->value(), ui->sb_threshold2->value(), true);
            ui->la_canny->setPixmap(QPixmap::fromImage(canny.scaled(ui->la_canny->size(), Qt::KeepAspectRatio)));
        }
    }
    else
        _cvf.closeWindow(CVFunctions::CANNY);
}

void MainWindow::on_cb_mask_clicked(bool checked)
{
    if (checked)
    {
        if(_cvf.isOpened())
        {
            auto mask = _cvf.mask(true);
            ui->la_mask->setPixmap(QPixmap::fromImage(mask.scaled(ui->la_canny->size(), Qt::KeepAspectRatio)));
        }
    }
    else
        _cvf.closeWindow(CVFunctions::MASK);
}

void MainWindow::on_pb_choose_clicked()
{
    QString home_path = QStandardPaths::standardLocations(QStandardPaths::HomeLocation)[0];
    QString save_dir = QFileDialog::getExistingDirectory(this, QString(), home_path);
    if (save_dir == "")
        return;
    else{
        ui->le_save->setText(save_dir);
    }
}

void MainWindow::on_pb_save_clicked()
{
    if(_cvf.isOpened()){
        auto mask_im = _area->grab().toImage();
        auto mask = _cvf.mask(mask_im, false);
        auto image = _cvf.origin(mask_im, false);
        auto im_info = QFileInfo(ui->statusBar->currentMessage());
        auto im_name = im_info.fileName();
        auto save_path = ui->le_save->text();
        if (save_path == ""){
            msg("保存路径为空");
            return;
        }
        auto dir = QDir(save_path);
        if(!dir.exists()){
            auto button = QMessageBox::information(this, "提示", "保存目录不存在，是否创建?", QMessageBox::Ok, QMessageBox::Cancel);
            if(button == QMessageBox::Ok){
                if(!dir.mkpath(save_path)){
                    msg("创建保存目录失败，请检查保存是否有权限读写保存路径");
                    return;
                }
            }
        }
        QDir origin_dir = save_path+("/origin");
        QDir mask_dir = save_path+"/mask";
        if(!origin_dir.exists()){
            origin_dir.mkpath(origin_dir.absolutePath());
        }
        if(!mask_dir.exists()){
            mask_dir.mkpath(mask_dir.absolutePath());
        }
        qDebug()<<origin_dir.absolutePath() +"/" +im_name;
        if(!image.save(origin_dir.absolutePath() + "/"+im_name)){
            msg("保存原图失败，请检查保存目录是否存在");
        }
        if(!mask.save(mask_dir.absolutePath() + "/"+im_name)){
            msg("保存Mask图失败，请检查保存目录是否存在");
        }
    }
}
