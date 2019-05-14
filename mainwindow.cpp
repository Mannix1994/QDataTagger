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
    ui(new Ui::MainWindow), _area(new PaintWidget(this)), _image_updated(false)
{
    ui->setupUi(this);
    ui->scrollArea->setWidget(_area);
    connect(_area, &PaintWidget::imageChanged, this, &MainWindow::on_image_changed);
    setWindowTitle("QDataCleaner");
    setMinimumSize(size());
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _area;
}


void MainWindow::setImage(ImageItem item)
{
    if (item.empty()){
        item = _imageList.current();
    }
    if(item.empty()){
        msg("检查是否打开包含图片列表的文件");
        return;
    }
    if(!_cvf.open(item)){
        msg("图像: \"" + item.target_ps() + "\" 无法打开");
        return;
    }
    if(checkExisted(item.origin())){
        setWindowTitle("QDataCleaner ("+QFileInfo(item.target_ps()).fileName()+" 已处理过)");
    }else{
        setWindowTitle("QDataCleaner ("+QFileInfo(item.target_ps()).fileName()+")");
    }
    ui->statusBar->showMessage(item.target_ps());
    auto withCanny = _cvf.withCanny(ui->cb_blur->currentText().toInt(), ui->sb_threshold1->value(), ui->sb_threshold2->value(),
                                    ui->cb_origin_canny->isChecked());
    _area->setImage(withCanny);
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
    if(!_cvf.isOpened())
    {
        return;
    }
    auto withCanny = _cvf.withCanny(ui->cb_blur->currentText().toInt(), ui->sb_threshold1->value(), ui->sb_threshold2->value(),
                                    ui->cb_origin_canny->isChecked());
    auto canny = _cvf.canny(ui->cb_blur->currentText().toInt(), ui->sb_threshold1->value(), ui->sb_threshold2->value(),
                            ui->cb_canny->isChecked());
    auto origin = _cvf.origin(ui->cb_origin->isChecked());
    auto mask = _cvf.mask(ui->cb_mask->isChecked());
    _cvf.target(ui->cb_origin_edge->isChecked());
    ui->la_canny->setPixmap(QPixmap::fromImage(canny.scaled(ui->la_canny->size(), Qt::KeepAspectRatio)));
    ui->la_origin->setPixmap(QPixmap::fromImage(origin.scaled(ui->la_origin->size(), Qt::KeepAspectRatio)));
    ui->la_mask->setPixmap(QPixmap::fromImage(mask.scaled(ui->la_mask->size(), Qt::KeepAspectRatio)));
}

bool MainWindow::checkSaved()
{
    if(_image_updated){
        auto button = QMessageBox::question(this, "提示", "当前图像尚未保存，是否保存？", "保存", "不保存", "取消", 2);
        //qDebug()<<button;
        if(button == 0){
            // 保存图像
            return save();
        }else if(button == 1){
            return true;
        }else {
            return false;
        }
    }else{
        return true;
    }
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
    QFile origin(save_path+"origin/"+im_name);
    QFile mask(save_path+"mask/"+im_name);
    //qDebug() << save_path+"/origin/"+im_name;
    return origin.exists() && mask.exists();
}

bool MainWindow::save()
{
    if(_cvf.isOpened()){
        auto mask_im = _area->drawedImage();
        auto mask = _cvf.mask(mask_im, false);
        auto image = _cvf.origin(mask_im, false);
        auto target = _cvf.target(false);
        auto im_info = QFileInfo(ui->statusBar->currentMessage());
        auto im_name = im_info.fileName();
        auto save_path = ui->le_save->text();
        if (save_path == ""){
            msg("保存路径为空");
            return false;
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
                    return false;
                }
            }
        }
        QDir origin_dir = save_path+("origin");
        QDir mask_dir = save_path+"mask";
        QDir target_dir = save_path+"target";
        if(!origin_dir.exists()){
            origin_dir.mkpath(origin_dir.absolutePath());
        }
        if(!mask_dir.exists()){
            mask_dir.mkpath(mask_dir.absolutePath());
        }
        if(!target_dir.exists()){
            target_dir.mkpath(target_dir.absolutePath());
        }
        qDebug()<<origin_dir.absolutePath() +"/" +im_name;
        if(!image.save(origin_dir.absolutePath() + "/"+im_name)){
            msg("保存原图失败，请检查保存目录是否存在");
            return false;
        }
        if(!mask.save(mask_dir.absolutePath() + "/"+im_name)){
            msg("保存Mask图失败，请检查保存目录是否存在");
            return false;
        }
        if(!target.save(target_dir.absolutePath()+"/"+im_name)){
            msg("保存target图失败，请检查保存目录是否存在");
            return false;
        }
        _image_updated = false;
        return true;
    }else {
        return  false;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    static bool showScrollBar=true;
    if((e->modifiers() == Qt::CTRL)){
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
    }else if(e->key() == Qt::Key_E){
        if(ui->cb_mode->currentIndex() == 0 ){
            ui->cb_mode->setCurrentIndex(1);
        }else if(ui->cb_mode->currentIndex() == 1 ){
            ui->cb_mode->setCurrentIndex(2);
        }else if(ui->cb_mode->currentIndex() == 2 ){
            ui->cb_mode->setCurrentIndex(0);
        }
    }else if(e->key()==Qt::Key_S){
        ui->sb_pensize->setValue(5);
    }
    // 其他按键事件在ui界面去修改
    e->accept();
}

void MainWindow::wheelEvent(QWheelEvent *e)
{
    if(e->modifiers() == Qt::ShiftModifier){
        // 放大图像
        static bool tag = true;
        if(tag){
            auto delta = e->delta();
            auto val = ui->sb_scale->value();
            auto new_val = 0;
            if(delta>0){
                new_val = val+1;
                ui->sb_scale->setValue(new_val);
            }else{
                new_val = val-1;
                ui->sb_scale->setValue(new_val);
            }
        }
        tag = !tag;
    }
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    auto size = e->size();
    auto old_size = e->oldSize();
    if(old_size == QSize(-1, -1))
        return;
    auto diff_x = size.width() - old_size.width();
    auto diff_y = size.height() - old_size.height();
    // 三个显示窗口
    auto rect = ui->sa_show->geometry();
    rect.setX(rect.x() + diff_x);
    rect.setWidth(rect.width()+diff_x);
    ui->sa_show->setGeometry(rect);
    // cany选项
    rect = ui->sa_canny_options->geometry();
    rect.setX(rect.x() + diff_x);
    rect.setWidth(rect.width()+diff_x);
    ui->sa_canny_options->setGeometry(rect);
    // 主窗口
    rect = ui->scrollArea->geometry();
    rect.setWidth(rect.width() + diff_x);
    rect.setHeight(rect.height() + diff_y);
    ui->scrollArea->setGeometry(rect);
    // 显示选项
    rect = ui->sa_show_options->geometry();
    rect.setX(rect.x() + diff_x);
    rect.setY(rect.y() + diff_y);
    rect.setWidth(rect.width() + diff_x);
    rect.setHeight(rect.height() + diff_y);
    ui->sa_show_options->setGeometry(rect);
    // 保存选项
    rect = ui->sa_save_block->geometry();
    rect.setY(rect.y() + diff_y);
    rect.setHeight(rect.height() + diff_y);
    ui->sa_save_block->setGeometry(rect);
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
    if(!checkSaved())
        return;
    auto item = _imageList.pre();
    if(!item.origin().length()){
        msg("没有上一张了啦");
        return;
    }
    setImage(item);
}

void MainWindow::on_pb_next_clicked()
{
    if(!checkSaved())
        return;
    auto item = _imageList.next();
    if(!item.origin().length()){
        msg("没有下一张了啦");
        return;
    }
    setImage(item);
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
    //on_image_changed();
}

void MainWindow::on_cb_mode_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "涂抹"){
        _area->setPenColor(PaintWidget::WHITE);
        ui->sb_pensize->setValue(5);
    }else if(arg1 == "消除"){
        _area->setPenColor(PaintWidget::GREEN);
        ui->sb_pensize->setValue(15);
    }else if(arg1 == "标记"){
        _area->setPenColor(PaintWidget::YELLOW);
        ui->sb_pensize->setValue(1);
    }
}

void MainWindow::on_image_changed()
{
    _image_updated = true;
    auto im = _area->drawedImage();
    if(im.isNull())
    {
        qDebug() << __LINE__ << " " << "drawedImage为空";
        return;
    }
    auto mask = _cvf.mask(im, ui->cb_mask->isChecked());
    ui->la_mask->setPixmap(QPixmap::fromImage(mask.scaled(ui->la_canny->size(), Qt::KeepAspectRatio)));
    _cvf.target(ui->cb_origin_edge->isChecked());
}

void MainWindow::on_cb_origin_edge_clicked(bool checked)
{
    if (checked)
    {
       updateImages();
    }
    else
        _cvf.closeWindow(CVFunctions::ORIGIN_EDGE);
}

void MainWindow::on_cb_origin_canny_clicked(bool checked)
{
    if (checked)
    {
       updateImages();
    }
    else
        _cvf.closeWindow(CVFunctions::WITH_CANNY);
}

void MainWindow::on_cb_origin_clicked(bool checked)
{
    if (checked)
    {
        updateImages();
    }
    else
        _cvf.closeWindow(CVFunctions::ORIGIN);
}

void MainWindow::on_cb_canny_clicked(bool checked)
{
    if (checked){
        updateImages();
    }
    else
        _cvf.closeWindow(CVFunctions::CANNY);
}

void MainWindow::on_cb_mask_clicked(bool checked)
{
    if (checked)
    {
        updateImages();
    }
    else
        _cvf.closeWindow(CVFunctions::MASK);
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
    save();
}

void MainWindow::on_cb_canny_mode_currentIndexChanged(const QString &arg1)
{
    if(arg1=="灰度图"){
        ui->sb_threshold1->setValue(50);
        ui->sb_threshold2->setValue(120);
        _cvf.setCannySource(CVFunctions::USE_GARY);
    }else if(arg1 == "亮度图"){
        ui->sb_threshold1->setValue(50);
        ui->sb_threshold2->setValue(120);
        _cvf.setCannySource(CVFunctions::USE_LIGHT);
    }else if(arg1 == "PS图"){
        ui->sb_threshold1->setValue(80);
        ui->sb_threshold2->setValue(200);
        _cvf.setCannySource(CVFunctions::USE_PS);
    }
    setImage();
}

void MainWindow::on_pb_fullscreen_clicked()
{
    if(ui->sa_show->isVisible()){
        ui->sa_show->setVisible(false);
        auto width = ui->scrollArea->width();
        auto height = ui->scrollArea->height();
        ui->scrollArea->resize(width + 390,height);
    }else {
        ui->sa_show->setVisible(true);
        auto width = ui->scrollArea->width();
        auto height = ui->scrollArea->height();
        ui->scrollArea->resize(width - 390,height);
    }
}
