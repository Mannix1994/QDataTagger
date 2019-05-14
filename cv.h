#ifndef CV_H
#define CV_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include <string>
#include "imagelist.h"

/**
 * @brief 管理当前图像的所有相关图像
 */
class CVFunctions : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief 定义窗口类型的字符串，就算将来修改字符串，各个地方还是一样的
     */
    class WINDDOW_STRING
    {
    public:
        WINDDOW_STRING();
        std::string CANNY, WITH_CANNY, MASK, ORIGIN, ORIGIN_EDGE;
    };
    // 窗口类型，分别是Canny、原图+Canny、Mask、原图
    typedef enum {CANNY, WITH_CANNY, MASK, ORIGIN, ORIGIN_EDGE} WINDOW;
    typedef enum {USE_GARY, USE_LIGHT, USE_EQ_HIST, USE_PS} CANNY_SOURCE;

    explicit CVFunctions(QObject *parent = nullptr);
    /**
     * @brief open 打开一张待处理的图像。后面的所有函数都是与当
     * 前打开的图像相关联。
     * @param im_path 图像的路径
     * @return 如果打开成功返回true，打开失败返回false
     */
    bool open(ImageItem &item);
    /**
     * @brief open 打开一张待处理的图像
     * @param image QImage图像对象
     * @return 如果打开成功返回true，打开失败返回false
     */
    bool open(QImage &origin, QImage &target_ps);
    /**
     * @brief isOpened 是否打开了一张图像
     * @return 打开了返回true，没打开返回false
     */
    bool isOpened();
    /**
     * @brief canny 调用OpenCV的Canny函数，计算_origin_1的Canny图像
     * @param blur cv::Blur函数的ksize参数
     * @param threshold1 cv::Canny的threshold1参数
     * @param threshold2 cv::Canny的threshold2参数
     * @param show 如果为true，则显示canny结果
     * @return 返回类型为QImage的canny图像
     */
    QImage canny(int blur, int threshold1, int threshold2, bool show=false);
    /**
     * @brief withCanny 在_origin_1上把_canny边涂成红色，并返回该图像
     * @param blur cv::Blur函数的ksize参数
     * @param threshold1 cv::Canny的threshold1参数
     * @param threshold2 cv::Canny的threshold2参数
     * @param show 如果为true，则显示该图像
     * @return 返回类型为QImage的原图+红色canny边图像
     */
    QImage withCanny(int blur, int threshold1, int threshold2, bool show=false);
    /**
     * @brief mask 返回当前阴影边的mask
     * @param show 如果为true，则显示该图像
     * @return 返回类型为QImage的mask图像
     */
    QImage mask(bool show=false);
    /**
     * @brief mask 这个重载版本和上面的非常不一样，这个函数是根据主界面
     * 中绘制的image对象来生成mask，是会更新mask的
     * @param image 主界面绘制的图像
     * @param show 如果为true，则显示该图像
     * @return 返回类型为QImage的阴影边mask图像
     */
    QImage mask(const QImage &image, bool show=false);
    /**
     * @brief origin 返回原图图像
     * @param show 如果为true，则显示原图
     * @return 返回类型为QImage的原图
     */
    QImage origin(bool show=false);
    /**
     * @brief origin 这个函数是根据主界面中绘制的image对象来生成原图，
     * 主界面中被涂成绿色的部分，会在返回的图像中涂成黑色，其他部分不变。
     * @param image 主界面绘制的图像
     * @param show 如果为true，则显示处理后的原图
     * @return 返回处理后的原图
     */
    QImage origin(const QImage &image, bool show=false);
    /**
     * @brief closeWindow 关闭window指定的窗口
     * @param window 就是之前说明的几种窗口
     */
    void closeWindow(WINDOW window);
    /**
     * @brief setCannySource 设置Canny时的源图。
     * @param cs USE_GRAY就是使用灰度图，USE_LIGHT是使用亮度图。
     */
    void setCannySource(CANNY_SOURCE _cs);
    /**
     * @brief target 返回加了红色阴影边的原图
     * @param show 是否显示
     * @return 如果_mask为空，则返回空QImage
     */
    QImage target(bool show=false);
private:
    /**
     * @brief canny_ 计算原图的Canny图
     * @param blur cv::Blur函数的ksize参数
     * @param threshold1 cv::Canny的threshold1参数
     * @param threshold2 cv::Canny的threshold2参数
     * @return 返回canny图
     */
    cv::Mat canny_(int blur, int threshold1, int threshold2);
    /**
     * @brief showMat 显示图像
     * @param title 窗口标题
     * @param mat 要显示的图像
     */
    void showMat(std::string title, cv::Mat &mat);
    // 原图
    cv::Mat _origin;
    // 从PS来的原图
    cv::Mat _target_ps;
    // 把_origin的每个像素减去1，这样我在主窗口用的标记就不会有重复值
    cv::Mat _origin_1;
    // Mask图像
    cv::Mat _mask;
    // 灰度图
    cv::Mat _gray_or_light;
    // Canny图像
    cv::Mat _canny;
    // 是否有打开图像
    bool _open;
    // 窗口子字符串
    WINDDOW_STRING _ws;
    // Canny 源图
    CANNY_SOURCE _cs;

signals:

public slots:
};

#endif // CV_H
