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
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /**
     * @brief setImage 设置绘制窗口中的图像
     * @param im_path 要设置的图像路径。如果为空，则在图片列表中取出“当前”
     * 图像；路径并设置。
     */
    void setImage(QString im_path="");
    /**
     * @brief msg 显示信息
     * @param content 内容
     */
    void msg(const QString &content);
    /**
     * @brief msg 显示信息
     * @param title 标题
     * @param content 内容
     */
    void msg(const QString  &title, const QString &content);
    /**
     * @brief updateImages _cvf中取出图像，并更新主界面的图像
     */
    void updateImages();
    /**
     * @brief checkSaved 检查当前图像是否已经保存
     */
    void checkSaved();
    /**
     * @brief checkExisted 检查要保存的文件是否在保存目录中存在
     * @param im_path 源图像文件名
     */
    bool checkExisted(const QString &im_path);

protected:
    /**
     * @brief keyPressEvent 响应键盘事件。Shift+S隐藏绘制控件滑动条；
     * A是打开前一张，D是打开后一张。
     */
    void keyPressEvent(QKeyEvent *e);

private slots:
    // 打开图片列表文件
    void on_pb_open_clicked();
    // 打开前一张图像，如果不存在，则不更新
    void on_pb_pre_clicked();
    // 打开后一张图像，如果不存在，则不更新
    void on_pb_next_clicked();
    // cv::Canny的threshold1变化，重新载入Canny等图像
    void on_sb_threshold1_valueChanged(int arg1);
    // cv::Canny的threshold2变化，重新载入Canny等图像
    void on_sb_threshold2_valueChanged(int arg1);
    // cv::Blur的ksize变化，重新载入Canny等图像
    void on_cb_blur_currentIndexChanged(int index);
    // 缩放大小值变化，调整绘制窗口中图像的大小
    void on_sb_scale_valueChanged(int arg1);
    // 画笔大小变化
    void on_sb_pensize_valueChanged(int arg1);
    // 绘制模式变化。“涂抹”是被涂抹的红色点会作为Mask的中的阴影边；
    // “标记”是手动绘制阴影边；“消除”是把不容易绘制影音边的地方涂黑。
    void on_cb_mode_currentIndexChanged(const QString &arg1);
    // 绘制控件中的图像发生了变化
    void on_image_changed();
    // 显示原图+Canny图
    void on_cb_origin_canny_clicked(bool checked);
    // 显示原图
    void on_cb_origin_clicked(bool checked);
    // 显示Canny图
    void on_cb_canny_clicked(bool checked);
    // 显示Mask图
    void on_cb_mask_clicked(bool checked);
    // 选择保存路径
    void on_pb_choose_clicked();
    // 撤销上一次绘制
    void on_pb_cancel_clicked();
    // 保存当前的Mask和原图
    void on_pb_save_clicked();
    // 当前是使用亮度图求Canny边还是使用灰度值求Canny边
    void on_cb_canny_mode_currentIndexChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    // 画板组件
    PaintWidget *_area;
    // 图片列表管理
    ImageList _imageList;
    // 需要用到的OpenCV算法
    CVFunctions _cvf;
    // 是否保存的标志
    bool _image_updated;
};

#endif // MAINWINDOW_H
