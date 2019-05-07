// https://blog.csdn.net/u012891055/article/details/41721849
#include "paintwidget.h"
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPen>
#include<QDebug>

PaintWidget::PaintWidget(QWidget *parent) :
    QWidget(parent), _isDrawing(false), _penSize(5), _scale(1), _maxHistorySize(10)
{
    _penColor = qRgb(255,255,255);
}

void PaintWidget::setImage(QImage image)
{

    _size = image.size();
    _image = image;
    _history.clear();
    _history.push_back(_image.copy());
    update();
}

void PaintWidget::setScale(int scale)
{
    _scale = scale;
    if(!_image.isNull()){
        //_image = _image;
        setMinimumSize(_image.size()*scale);
        update();
    }
}

void PaintWidget::setPenSize(int size)
{
    if(size<1){
        return;
    }
    this->_penSize = size;
}

void PaintWidget::setPenColor(const COLOR &penColor)
{
    switch (penColor) {
    case WHITE:
        _penColor = qRgb(255, 255, 255);
        break;
    case YELLOW:
        _penColor = qRgb(255, 255, 0);
        break;
    case GREEN:
        _penColor = qRgb(0, 255, 0);
        break;
    }
}

void PaintWidget::cancel()
{
    if(_history.size()>1)
    {
        _history.removeLast();
        _image = _history.last();
        update();
        emit imageChanged();
    }
}

QImage PaintWidget::drawedImage()
{
    return _image.copy();
}

void PaintWidget::paintEvent(QPaintEvent *){
    if(_image.isNull())
        return;
    QPainter painter(this);
    painter.drawImage(0,0,_image.scaled(_size*_scale));
}

void PaintWidget::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::RightButton){
        _two_points.push_back(event->pos()/this->_scale);
        if(_two_points.size()>1){
            // store old color
            auto penc = _penColor;
            auto pens = _penSize;
            // set new color and pensize
            _penColor = qRgb(255, 255, 0);
            _penSize = 1;
            // draw
            lastPoint = _two_points[0];
            endPoint = _two_points[1];
            paint();
            // clear
            _two_points.clear();
            // restore
            _penColor = penc;
            _penSize = pens;
            _isDrawing = true;
        }
    }
    else if(event->button() == Qt::LeftButton){
        _two_points.clear();
        lastPoint = event->pos()/this->_scale;
        endPoint = event->pos()/this->_scale;
        _isDrawing = true;
    }
}

void PaintWidget::mouseMoveEvent(QMouseEvent *event){
    if(event->buttons() & Qt::LeftButton){
        endPoint = event->pos()/this->_scale;
        paint();
    }
}
void PaintWidget::mouseReleaseEvent(QMouseEvent *event){
    Q_UNUSED(event);
    //qDebug() << event->button();
    if(event->button() == Qt::LeftButton){
        paint();
    }
    if(_isDrawing == true && !_image.isNull())
    {
        _history.push_back(_image);
        if(_history.size()>_maxHistorySize){
            _history.removeFirst();
        }
        emit imageChanged();
    }
    _isDrawing = false;
}

void PaintWidget::paint(){
    if(_image.isNull())
        return;
    QPainter p(&_image);
    QPen apen;
    apen.setColor(_penColor);
    apen.setWidth(this->_penSize);
    p.setPen(apen);
    p.drawLine(lastPoint,endPoint);
    lastPoint = endPoint;
    update();
}
