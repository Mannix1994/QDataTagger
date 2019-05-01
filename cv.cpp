#include "cv.h"
#include<QImage>
#include"convert.h"

CVFunctions::CVFunctions(QObject *parent) : QObject(parent)
{

}

bool CVFunctions::open(const QString &im_path)
{
    QImage im;
    if(im.load(im_path)){
        return this->open(im);
    }else{
        return false;
    }

}

bool CVFunctions::open(const QImage &image)
{
    if (image.isNull()){
        return false;
    }
    _origin = toMat(image);
    cv::cvtColor(_origin, _gray, cv::COLOR_BGR2GRAY);
    _mask = cv::Mat(_origin.size(), CV_8UC1);
}

QImage CVFunctions::canny(int threshold1, int threshold2)
{
    cv::Mat _canny;
    cv::Canny(_gray, _canny, threshold1, threshold2);
    return toQImage(_canny);
}

QImage CVFunctions::withCannny(int threshold1, int threshold2)
{
    cv::Mat _canny;
    cv::Canny(_gray, _canny, threshold1, threshold2);
    cv::Mat mat=_origin.clone();
    for(int i=0;i<_origin.rows;++i){
        for(int j=0;j<_origin.cols;++j){
            if(_canny.at<uchar>(i, j)==255){
                mat.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 255);
            }
        }
    }
    return toQImage(mat);
}

QImage CVFunctions::mask()
{
    return toQImage(_mask);
}
