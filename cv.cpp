#include "cv.h"
#include<QImage>
#include"convert.h"
#include<QMessageBox>

CVFunctions::CVFunctions(QObject *parent) : QObject(parent), _open(false)
{

}

void CVFunctions::show(const QImage &image, CVFunctions::WINDOW window)
{
    auto mat = toMat(image, true);
    switch (window) {
    case MASK:
        showMat(_ws.MASK, mat);
        break;
    case ORIGIN:
       showMat(_ws.ORIGIN, mat);
        break;
    default:
        break;
    }
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
    case MASK:
        cv::namedWindow(_ws.MASK);
        cv::destroyWindow(_ws.MASK);
        break;
    case ORIGIN:
        cv::namedWindow(_ws.ORIGIN);
        cv::destroyWindow(_ws.ORIGIN);
        break;
    default:
        break;
    }
}

CVFunctions::WINDDOW_STRING::WINDDOW_STRING(): MASK("Mask"), ORIGIN("原图")
{

}
