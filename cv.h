#ifndef CV_H
#define CV_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include <string>

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
        std::string MASK, ORIGIN;
    };
    // 窗口类型，分别是Canny、原图+Canny、Mask、原图
    typedef enum {MASK, ORIGIN} WINDOW;

    explicit CVFunctions(QObject *parent = nullptr);
    /**
     * @brief show 显示图像
     * @param image 要显示的图像
     * @param window WINDOW，对应窗口名
     */
    void show(const QImage& image, WINDOW window);
    /**
     * @brief closeWindow 关闭window指定的窗口
     * @param window 就是之前说明的几种窗口
     */
    void closeWindow(WINDOW window);
private:
    /**
     * @brief showMat 显示图像
     * @param title 窗口标题
     * @param mat 要显示的图像
     */
    void showMat(std::string title, cv::Mat &mat);
    // 是否有打开图像
    bool _open;
    // 窗口子字符串
    WINDDOW_STRING _ws;

signals:

public slots:
};

#endif // CV_H
