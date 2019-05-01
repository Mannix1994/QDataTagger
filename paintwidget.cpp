// https://blog.csdn.net/u012891055/article/details/41721849
#include "paintwidget.h"
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPen>

PaintWidget::PaintWidget(QWidget *parent) :
    QWidget(parent)
{
    isDrawing = false;
    image = QImage(700,500,QImage::Format_RGB32);
    backColor = qRgb(255,255,255);
    image.fill(backColor);
}

void PaintWidget::paintEvent(QPaintEvent *){

    QPainter painter(this);
    painter.drawImage(0,0,image);
}

void PaintWidget::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        lastPoint = event->pos();
        endPoint = event->pos();
        isDrawing = true;
    }
}

void PaintWidget::mouseMoveEvent(QMouseEvent *event){ //重点理解部分

    if(event->buttons() & Qt::LeftButton){
        endPoint = event->pos();
        paint(image);
 }
}
void PaintWidget::mouseReleaseEvent(QMouseEvent *event){
    isDrawing = false;
    paint(image);
}


void PaintWidget::paint (QImage &theImage){
    QPainter p(&theImage);
    QPen apen;
    apen.setWidth(5);
    p.setPen(apen);
    p.drawLine(lastPoint,endPoint);
    lastPoint = endPoint;
    update();
}
