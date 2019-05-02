#include "cv.h"
#include<QImage>
#include"convert.h"
#include<QMessageBox>

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

bool CVFunctions::open(QImage &image)
{
    if (image.isNull()){
        return false;
    }
    _origin = toMat(image, true);
    if (_origin.type() == CV_8UC4)
        cv::cvtColor(_origin, _gray, cv::COLOR_BGRA2GRAY);
    else if(_origin.type() == CV_8UC4)
        cv::cvtColor(_origin, _gray, cv::COLOR_BGR2GRAY);
    else{
        QMessageBox::critical(nullptr, "警告", "错误33: 不支持此类型的图像");
    }
    _mask = cv::Mat(_origin.size(), CV_8UC1);
    return true;
}

QImage CVFunctions::canny(int threshold1, int threshold2)
{
    cv::Mat _canny;
    cv::Canny(_gray, _canny, threshold1, threshold2);
    return toQImage(_canny);
}

QImage CVFunctions::withCanny(int threshold1, int threshold2)
{
    cv::Mat _canny;
    cv::Canny(_gray, _canny, threshold1, threshold2);
    cv::Mat mat = _origin.clone();
    //cv::imshow("_origin1", _origin);
    //cv::imshow("mat", mat);
    if(mat.type() == CV_8UC4)
    {
        for(int i=0;i<_origin.rows;++i){
            for(int j=0;j<_origin.cols;++j){
                if(_canny.at<uchar>(i, j)==255){
                    mat.at<cv::Vec4b>(i, j) = cv::Vec4b(0, 0, 255, 255);
                }
            }
        }
    }else if(mat.type() == CV_8UC3)
    {
        for(int i=0;i<_origin.rows;++i){
            for(int j=0;j<_origin.cols;++j){
                if(_canny.at<uchar>(i, j)==255){
                    mat.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 255);
                }
            }
        }
    }else{
        QMessageBox::critical(nullptr, "错误", "错误72: 不支持此类型的图像");
    }
    //cv::waitKey(50);
    return toQImage(mat);
}

QImage CVFunctions::mask()
{
    return toQImage(_mask);
}
