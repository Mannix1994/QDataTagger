#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "paintwidget.h"
#include "imagelist.h"
#include "cv.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void setImage(QString im_path="");
    void msg(const QString &content);
    void msg(const QString  &title, const QString &content);

private slots:
    void on_pb_open_clicked();

    void on_pb_pre_clicked();

    void on_pb_next_clicked();

    void on_sb_threshold1_valueChanged(int arg1);

    void on_sb_threshold2_valueChanged(int arg1);

    void on_cb_blur_currentIndexChanged(int index);

    void on_cb_origin_canny_clicked();

    void on_cb_origin_clicked();

    void on_cb_canny_clicked();

    void on_cb_mask_clicked();

private:
    Ui::MainWindow *ui;
    PaintWidget *_area;//-----------------------------------创建一个画板组件
    ImageList _imageList;
    CVFunctions _cvf;
};

#endif // MAINWINDOW_H
