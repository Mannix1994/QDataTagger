#include "cv.h"
#include<QImage>
#include"convert.h"
#include<QMessageBox>

CVFunctions::CVFunctions(QObject *parent) : QObject(parent), _open(false)
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
    _open = false;
    if (image.isNull()){
        return false;
    }
    _origin = toMat(image, true);
    _origin_minus_1 = _origin - 1;
    if (_origin_minus_1.type() == CV_8UC4)
        cv::cvtColor(_origin_minus_1, _gray, cv::COLOR_BGRA2GRAY);
    else if(_origin_minus_1.type() == CV_8UC3)
        cv::cvtColor(_origin_minus_1, _gray, cv::COLOR_BGR2GRAY);
    else if(_origin_minus_1.type() == CV_8UC3)
        _gray = _origin_minus_1.clone();
    else{
        QMessageBox::critical(nullptr, "警告", "错误33: 不支持此类型的图像");
    }
    _mask = cv::Mat(_origin_minus_1.size(), CV_8UC1, cv::Scalar::all(0));
    _open = true;
    return true;
}

bool CVFunctions::isOpened()
{
    return _open;
}

QImage CVFunctions::canny(int blur, int threshold1, int threshold2, bool show)
{
    auto _canny = canny_(blur, threshold1, threshold2);
    if(show){
        showMat("Canny", _canny);
    }
    return toQImage(_canny);
}

QImage CVFunctions::withCanny(int blur, int threshold1, int threshold2, bool show)
{
    auto _canny = canny_(blur, threshold1, threshold2);
    cv::Mat mat = _origin_minus_1.clone();
    //cv::imshow("_origin1", _origin);
    //cv::imshow("mat", mat);
    if(mat.type() == CV_8UC4)
    {
        for(int i=0;i<_origin_minus_1.rows;++i){
            for(int j=0;j<_origin_minus_1.cols;++j){
                if(_canny.at<uchar>(i, j)==255){
                    mat.at<cv::Vec4b>(i, j) = cv::Vec4b(0, 0, 255, 255);
                }
            }
        }
    }else if(mat.type() == CV_8UC3)
    {
        for(int i=0;i<_origin_minus_1.rows;++i){
            for(int j=0;j<_origin_minus_1.cols;++j){
                if(_canny.at<uchar>(i, j)==255){
                    mat.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 255);
                }
            }
        }
    }else if(mat.type() == CV_8UC1)
    {
        for(int i=0;i<_origin_minus_1.rows;++i){
            for(int j=0;j<_origin_minus_1.cols;++j){
                if(_canny.at<uchar>(i, j)==255){
                    mat.at<uchar>(i, j) = 255;
                }
            }
        }
    }
    else{
        QMessageBox::critical(nullptr, "错误", "错误72: 不支持此类型的图像");
    }
    //cv::waitKey(50);
    if(show){
        showMat("原图+Canny", mat);
    }
    return toQImage(mat);
}

QImage CVFunctions::mask(bool show)
{
    if(show){
        showMat("Mask", _mask);
    }
    return toQImage(_mask);
}

QImage CVFunctions::origin(bool show)
{
    if(show){
        showMat("原图", _origin_minus_1);
    }
    return toQImage(_origin_minus_1);
}

void CVFunctions::showMat(std::string title, cv::Mat &mat)
{
    cv::namedWindow(title, cv::WINDOW_NORMAL);
    cv::imshow(title, mat);
    cv::waitKey(1);
}

void CVFunctions::closeWindow(CVFunctions::WINDOW window)
{
    switch (window) {
    case CANNY:
        cv::namedWindow("Canny");
        cv::destroyWindow("Canny");
        break;
    case MASK:
        cv::namedWindow("Mask");
        cv::destroyWindow("Mask");
        break;
    case WITH_CANNY:
        cv::namedWindow("原图+Canny");
        cv::destroyWindow("原图+Canny");
        break;
    case ORIGIN:
        cv::namedWindow("原图");
        cv::destroyWindow("原图");
        break;
    default:
        break;
    }
}

cv::Mat CVFunctions::canny_(int blur, int threshold1, int threshold2)
{
    cv::Mat _canny;
    cv::blur(_gray, _canny, cv::Size(blur, blur));
    cv::Canny(_canny, _canny, threshold1, threshold2);
    return _canny;
}
