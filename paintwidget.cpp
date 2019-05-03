// https://blog.csdn.net/u012891055/article/details/41721849
#include "paintwidget.h"
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPen>
#include<QDebug>

PaintWidget::PaintWidget(QWidget *parent) :
    QWidget(parent), _isDrawing(false), _penSize(5), _scale(1), _maxHistorySize(5)
{
    _image = QImage(1,1,QImage::Format_RGB32);
    backColor = qRgb(255,255,255);
    _image.fill(backColor);
    _penColor = qRgb(255,255,255);
}

void PaintWidget::setImage(QImage image)
{

    _size = image.size();
    _image = image.scaled(_size*_scale);
    _history.clear();
    _history.push_back(_image.copy());
    update();
}

void PaintWidget::setScale(int scale)
{
    _scale = scale;
    if(!_image.isNull()){
        _image = _image.scaled(_size*scale);
        setMinimumSize(_image.size());
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
        _image = _history.last().scaled(_size*_scale);
        update();
    }
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
        _history.push_back(_image.scaled(_size));
        if(_history.size()>_maxHistorySize){
            _history.removeFirst();
        }
    }
    _isDrawing = false;
}

void PaintWidget::paint(QImage &theImage){
    QPainter p(&theImage);
    QPen apen;
    apen.setColor(_penColor);
    apen.setWidth(this->_penSize*this->_scale);
    p.setPen(apen);
    p.drawLine(lastPoint,endPoint);
    lastPoint = endPoint;
    update();
}
