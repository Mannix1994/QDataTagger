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

void PaintWidget::setPenColor(const QRgb &penColor)
{
    _penColor = penColor;
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
    if(event->button() == Qt::LeftButton){
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
    paint();
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
