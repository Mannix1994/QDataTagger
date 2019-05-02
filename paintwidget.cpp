// https://blog.csdn.net/u012891055/article/details/41721849
#include "paintwidget.h"
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPen>
#include<QDebug>

PaintWidget::PaintWidget(QWidget *parent) :
    QWidget(parent), _isDrawing(false), _penSize(5), _scale(1)
{
    _image = QImage(1,1,QImage::Format_RGB32);
    backColor = qRgb(255,255,255);
    _image.fill(backColor);
    setFocusPolicy(Qt::StrongFocus);
}

void PaintWidget::setImage(QImage image)
{
    _image = image;
    _size = image.size();
    update();
}

void PaintWidget::setScale(int scale)
{
    _image = _image.scaled(_size*scale);
    _scale = scale;
    update();
}

void PaintWidget::setPenSize(int size)
{
    if(size<1){
        return;
    }
    this->_penSize = size;
}

void PaintWidget::paintEvent(QPaintEvent *){

    QPainter painter(this);
    painter.drawImage(0,0,_image);
}

void PaintWidget::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        lastPoint = event->pos();
        endPoint = event->pos();
        _isDrawing = true;
    }else if(event->button() == Qt::RightButton){
        qDebug() << _history.size();

    }
}

void PaintWidget::mouseMoveEvent(QMouseEvent *event){

    if(event->buttons() & Qt::LeftButton){
        endPoint = event->pos();
        paint(_image);
    }
}
void PaintWidget::mouseReleaseEvent(QMouseEvent *event){
    Q_UNUSED(event);
    paint(_image);
    if(_isDrawing == true)
    {
        _history.push_back(_image.copy());
        if(_history.size()>3){
            _history.removeFirst();
        }
    }
    _isDrawing = false;
}

void PaintWidget::keyPressEvent(QKeyEvent *event)
{
    if((event->key() == Qt::Key_Z) && (event->modifiers()==Qt::ControlModifier)){
        if(_history.size()>1)
        {
            _history.removeLast();
            _image = _history.last();
            update();
        }
    }
}


void PaintWidget::paint(QImage &theImage){
    QPainter p(&theImage);
    QPen apen;
    apen.setWidth(this->_penSize*this->_scale);
    p.setPen(apen);
    p.drawLine(lastPoint,endPoint);
    lastPoint = endPoint;
    update();
}
