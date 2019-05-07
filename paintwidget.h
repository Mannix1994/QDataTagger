//先理解画笔，不使用双缓冲绘图
#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QEvent>
#include <QVector>
#include <QList>
#include <QRgb>

/**
 * @brief 绘制图像控件
 */
class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    typedef enum {WHITE, GREEN, YELLOW} COLOR;
    explicit PaintWidget(QWidget *parent = nullptr);//------------构造函数
    /**
     * @brief setImage 设置当前要绘制的图像
     * @param image 原图+Canny边图
     */
    void setImage(QImage image);
    /**
     * @brief setScale 设置图像放大的倍数
     * @param scale 倍数
     */
    void setScale(int scale);
    /**
     * @brief setPenSize 设置画笔的大小
     * @param size 大小
     */
    void setPenSize(int size);
    /**
     * @brief setPenColor 设置画笔的颜色
     * @param penColor 画笔的颜色有三种。白色是涂抹操作，被白色涂抹的
     * 区域中的Canny会被当成阴影边存到Mask中；黄色是手动标记的阴影边，会被
     * 保存到mask中；绿色是当前区域有不好标记的影音边，直接在原图中涂黑。
     */
    void setPenColor(const COLOR &penColor);
    /**
     * @brief cancel 撤销上一次涂抹
     */
    void cancel();
    QImage drawedImage();
public  slots:

protected:
    // 重绘事件(重点:由update()函数触发)
    void paintEvent(QPaintEvent *event);
    // 鼠标按下事件，开始在图像上绘制
    void mousePressEvent(QMouseEvent *event);
    // 鼠标移动事件(重点理解)，在控件上移动，绘制图像
    void mouseMoveEvent(QMouseEvent *event);
    // 鼠标释放事件，结束绘制，并把本次绘制的结果图像存到历史记录里面。
    void mouseReleaseEvent(QMouseEvent *event);
    // 绘制函数
    void paint();
signals:
    void imageChanged();

private:
    // 当前绘制的图像
    QImage _image;
    // 作用不大
    QRgb backColor;
    // 前一个点
    QPoint lastPoint;
    // 后一个点(结束点)
    QPoint endPoint;
    // 判断是否在绘图也就是判断鼠标是否操作
    bool _isDrawing;
    // 笔的大小
    int _penSize;
    // 缩放倍数
    int _scale;
    // 最大历史记录
    int _maxHistorySize;
    // 笔的颜色
    QRgb _penColor;
    // 原图
    QImage _origin;
    // 原图的大小
    QSize _size;
    // 绘制的历史记录
    QList<QImage> _history;

    // 支持新的绘制模式
    QVector<QPoint> _two_points;
};
#endif
