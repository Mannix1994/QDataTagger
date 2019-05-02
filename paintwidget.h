//先理解画笔，不使用双缓冲绘图
#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QEvent>

class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = 0);//------------构造函数
    void setImage(QImage image);

public  slots:


protected:
    void paintEvent(QPaintEvent *);//-----------------------重绘事件(重点:由update()函数触发)
    void mousePressEvent(QMouseEvent *);//------------------鼠标按下事件
    void mouseMoveEvent(QMouseEvent *);//-------------------鼠标移动事件(重点理解)
    void mouseReleaseEvent(QMouseEvent *);//----------------鼠标释放事件

    void paint(QImage &theImage);//-------------------------画图工具
signals:

private:

    QImage _image;//-----------------------------------------一块画布
    QRgb backColor;//---------------------------------------画布背景色
    QPoint lastPoint;//-------------------------------------前一个点，因为线是由无数点组成的
    QPoint endPoint;//--------------------------------------后一个点(结束点)
    bool isDrawing;//---------------------------------------判断是否在绘图也就是判断鼠标是否操作
};
#endif
