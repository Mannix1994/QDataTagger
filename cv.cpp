#include "cv.h"
#include<QImage>
#include"convert.h"
#include<QMessageBox>

CVFunctions::CVFunctions(QObject *parent) : QObject(parent), _open(false), _cs(USE_GARY)
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
    // 检查图像是否为空
    if (image.isNull()){
        return false;
    }
    // 把QImage转换为cv::Mat
    _origin = toMat(image, true);
    // 统一转换为三通道的图
    if (_origin.type() == CV_8UC4){
        cv::cvtColor(_origin, _origin, cv::COLOR_BGRA2BGR);
    }else if(_origin.type() == CV_8UC3){
        // 不做操作
    }else if(_origin.type() == CV_8UC1){
        cv::cvtColor(_origin, _origin, cv::COLOR_GRAY2BGR);
    }else{
        QMessageBox::critical(nullptr, "警告", "错误33: 不支持此类型的图像");
    }
    // 减去1，保证颜色标记都大于图像值
    _origin_1 = _origin - 1;
    // 根据不同的选项求Canny源
    if(_cs == USE_GARY) { // 求灰度图作为Canny源
        cv::cvtColor(_origin_1, _gray_or_light, cv::COLOR_BGR2GRAY);
    }else if (_cs == USE_LIGHT){ // 求亮度图作为Canny源
        cv::Mat bgr_image;
        // 转换颜色空间
        cv::cvtColor(_origin_1, bgr_image, cv::COLOR_BGR2Lab);
        // 分离三个通道
        cv::Mat channels[3];
        cv::split(bgr_image, channels);
        // 第一个通道就是亮度图
        _gray_or_light = channels[0].clone();
    }else if(_cs==USE_EQ_HIST){
        cv::cvtColor(_origin_1, _gray_or_light, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(_gray_or_light, _gray_or_light);
        //cv::imshow("msf", _gray_or_light);
        //cv::waitKey(1);
    }
    else{
        QMessageBox::critical(nullptr, "警告", "错误81: 暂时不支持此Canny源");
    }
    //cv::imshow("light", _gray_or_light);
    //cv::waitKey(1);
    // 定义一个空的Mask
    _mask = cv::Mat(_origin_1.size(), CV_8UC1, cv::Scalar::all(0));
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
        showMat(_ws.CANNY, _canny);
    }
    return toQImage(_canny);
}

QImage CVFunctions::withCanny(int blur, int threshold1, int threshold2, bool show)
{
    auto _canny = canny_(blur, threshold1, threshold2);
    cv::Mat mat = _origin_1.clone();
    if(mat.type() == CV_8UC3)
    {
        for(int i=0;i<_origin_1.rows;++i){
            for(int j=0;j<_origin_1.cols;++j){
                if(_canny.at<uchar>(i, j)==255){
                    mat.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 255);
                }
            }
        }
    }else{
        QMessageBox::critical(nullptr, "错误", "错误72: 代码有错误，请开发者检查");
    }
    if(show){
        showMat(_ws.WITH_CANNY, mat);
    }
    return toQImage(mat);
}

QImage CVFunctions::mask(bool show)
{
    if(show){
        showMat(_ws.MASK, _mask);
    }
    return toQImage(_mask);
}

QImage CVFunctions::mask(const QImage &image, bool show)
{
    auto im = toMat(image);
    if(im.type() == CV_8UC4)
        cv::cvtColor(im, im, cv::COLOR_BGRA2BGR);
    else if(im.type() == CV_8UC1)
        cv::cvtColor(im, im, cv::COLOR_GRAY2BGR);
    cv::resize(im, im, _origin.size());
    _mask.setTo(cv::Scalar::all(0));
    if(im.type() == CV_8UC3)
    {
        for(int i=0;i<_origin_1.rows;++i){
            for(int j=0;j<_origin_1.cols;++j){
                auto val = im.at<cv::Vec3b>(i, j);
                if(val == cv::Vec3b(0, 255, 0))
                    continue;
                if((_canny.at<uchar>(i, j)==255 && val==cv::Vec3b(255, 255, 255)) || val == cv::Vec3b(0, 255, 255))
                {
                    _mask.at<uchar>(i, j) = 255;
                }
            }
        }
    }
    else{
        QMessageBox::critical(nullptr, "错误", "错误132: 请开发者检查代码错误");
    }
    return mask(show);
}

QImage CVFunctions::origin(bool show)
{
    if(show){
        showMat(_ws.ORIGIN, _origin);
    }
    return toQImage(_origin);
}

QImage CVFunctions::origin(const QImage &image, bool show)
{
    auto mask_im = toMat(image);
    if(mask_im.type() == CV_8UC4)
        cv::cvtColor(mask_im, mask_im, cv::COLOR_BGRA2BGR);
    else if(mask_im.type() == CV_8UC1)
        cv::cvtColor(mask_im, mask_im, cv::COLOR_GRAY2BGR);
    cv::resize(mask_im, mask_im, _origin.size());
    auto im = _origin.clone();
    if(mask_im.type() == CV_8UC3)
    {
        for(int i=0;i<_origin_1.rows;++i){
            for(int j=0;j<_origin_1.cols;++j){
                auto val = mask_im.at<cv::Vec3b>(i, j);
                if(val == cv::Vec3b(0, 255, 0))
                {
                    im.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 0);
                }
            }
        }
    }
    else{
        QMessageBox::critical(nullptr, "错误", "错误181: 请开发者检查代码错误");
    }
    if(show){
        showMat(_ws.ORIGIN, im);
    }
    return toQImage(im);
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
        cv::namedWindow(_ws.CANNY);
        cv::destroyWindow(_ws.CANNY);
        break;
    case MASK:
        cv::namedWindow(_ws.MASK);
        cv::destroyWindow(_ws.MASK);
        break;
    case WITH_CANNY:
        cv::namedWindow(_ws.WITH_CANNY);
        cv::destroyWindow(_ws.WITH_CANNY);
        break;
    case ORIGIN:
        cv::namedWindow(_ws.ORIGIN);
        cv::destroyWindow(_ws.ORIGIN);
        break;
    default:
        break;
    }
}

void CVFunctions::setCannySource(CVFunctions::CANNY_SOURCE cs)
{
    _cs = cs;
}

cv::Mat CVFunctions::canny_(int blur, int threshold1, int threshold2)
{
    cv::blur(_gray_or_light, _canny, cv::Size(blur, blur));
    cv::Canny(_canny, _canny, threshold1, threshold2);
    return _canny;
}

CVFunctions::WINDDOW_STRING::WINDDOW_STRING():CANNY("Canny"), WITH_CANNY("原图+Canny"), MASK("Mask"), ORIGIN("原图")
{

}
