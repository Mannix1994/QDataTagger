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
    ui(new Ui::MainWindow), _area(new PaintWidget()), _image_updated(false)
{
    ui->setupUi(this);
    ui->scrollArea->setWidget(_area);
    connect(_area, &PaintWidget::imageChanged, this, &MainWindow::on_image_changed);
    setWindowTitle("QDataCleaner");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _area;
}


void MainWindow::setImage(QString im_path)
{
    if (im_path==""){
        im_path = _imageList.current();
    }
    if(im_path==""){
        msg("检查是否打开包含图片列表的文件");
        return;
    }
    QImage origin;
    if(!origin.load(im_path)){
        msg("图像: \"" + im_path + "\" 无法打开");
        return;
    }
    if(checkExisted(im_path)){
        setWindowTitle("QDataCleaner ("+QFileInfo(im_path).fileName()+" 已处理过)");
    }else{
        setWindowTitle("QDataCleaner ("+QFileInfo(im_path).fileName()+")");
    }
    ui->statusBar->showMessage(im_path);
    _area->setImage(origin);
    updateImages();
    _image_updated = false;
}

void MainWindow::msg(const QString &content)
{
    msg("提示", content);
}

void MainWindow::msg(const QString &title, const QString &content)
{
    QMessageBox::information(this, title, content, QMessageBox::Ok);
}

void MainWindow::updateImages()
{
    auto origin = _area->origin();
    ui->la_origin->setPixmap(QPixmap::fromImage(origin.scaled(ui->la_origin->size(), Qt::KeepAspectRatio)));
    auto d_image = _area->drawedImage();
    ui->la_mask->setPixmap(QPixmap::fromImage(d_image.scaled(ui->la_mask->size(), Qt::KeepAspectRatio)));
}

void MainWindow::checkSaved()
{
    if(_image_updated){
        auto button = QMessageBox::question(this, "提示", "当前图像尚未保存，是否保存？", QMessageBox::Ok, QMessageBox::Cancel);
        if(button == QMessageBox::Ok){
            // 保存图像
            on_pb_save_clicked();
        }
    }
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
    }else if(e->key() == Qt::Key_W){// 通过按W键在5和25，50之间快速切换
        auto val = ui->sb_pensize->value();
        if(val == 5){
            ui->sb_pensize->setValue(25);
        }else if(val == 25){
            ui->sb_pensize->setValue(50);
        }else if(val == 50){
            ui->sb_pensize->setValue(5);
        }
        else{
            ui->sb_pensize->setValue(50);
        }
    }
    // 其他按键事件在ui界面去修改
    e->accept();
}

void MainWindow::on_pb_open_clicked()
{
    QString home_path = QStandardPaths::standardLocations(QStandardPaths::HomeLocation)[0];
    QString list_file = QFileDialog::getOpenFileName(this, QString(),home_path,"TXT (*.txt)");
    //ui->statusBar->showMessage(list_file);
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
    checkSaved();
    auto im_path = _imageList.pre();
    if(!im_path.length()){
        msg("没有上一张了啦");
        return;
    }
    setImage(im_path);
}

void MainWindow::on_pb_next_clicked()
{
    checkSaved();
    auto im_path = _imageList.next();
    if(!im_path.length()){
        msg("没有下一张了啦");
        return;
    }
    setImage(im_path);
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
    //on_image_changed();
}

void MainWindow::on_cb_mode_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "涂抹"){
        _area->setPenColor(qRgb(0, 0, 0));
        ui->sb_pensize->setValue(5);
    }
}

void MainWindow::on_image_changed()
{
    _image_updated = true;
    auto im = _area->drawedImage();
    ui->la_mask->setPixmap(QPixmap::fromImage(im.scaled(ui->la_mask->size(), Qt::KeepAspectRatio)));
}

void MainWindow::on_cb_origin_clicked(bool checked)
{
    if (checked)
    {
        auto origin = _area->origin();
        _cvf.show(origin, CVFunctions::ORIGIN);
    }else{
        _cvf.closeWindow(CVFunctions::ORIGIN);
    }
}

void MainWindow::on_cb_mask_clicked(bool checked)
{
    if (checked)
    {
        auto origin = _area->drawedImage();
        _cvf.show(origin, CVFunctions::MASK);
    }else{
        _cvf.closeWindow(CVFunctions::MASK);
    }
}

void MainWindow::on_pb_choose_clicked()
{
    QString s = ui->statusBar->currentMessage();
    if(s == ""){
        s =  QStandardPaths::standardLocations(QStandardPaths::HomeLocation)[0];
    }
    QString save_dir = QFileDialog::getExistingDirectory(this, QString(), s);
    if (save_dir == "")
        return;
    else{
        ui->le_save->setText(save_dir);
    }
}

void MainWindow::on_pb_save_clicked()
{
    auto image = _area->drawedImage();
    auto im_info = QFileInfo(ui->statusBar->currentMessage());
    auto im_name = im_info.fileName();
    auto save_path = ui->le_save->text();
    if (save_path == ""){
        msg("保存路径为空");
        return;
    }
    if(!(save_path.endsWith('/') || save_path.endsWith('\\'))){
        save_path += '/';
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

    if(!image.save(dir.absolutePath() + "/"+im_name)){
        msg("保存原图失败，请检查保存目录是否存在");
    }
    _image_updated = false;
}


bool MainWindow::checkExisted(const QString &im_path)
{
    QFileInfo fi(im_path);
    auto im_name = fi.fileName();
    auto save_path = ui->le_save->text();
    if (save_path == ""){
        return false;
    }
    if(!(save_path.endsWith('/') || save_path.endsWith('\\'))){
        save_path += '/';
    }
    QFile origin(save_path+im_name);
    QFile mask(save_path+im_name);
    return origin.exists() && mask.exists();
}

