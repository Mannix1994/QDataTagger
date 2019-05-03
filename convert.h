/****************************************************************************
** file: convert.h
** brief: cv::Mat 与 QImage 之间的转换函数。在 Qt 程序中使用 OpenCV 时有用。
** Copyright (C) LiYuan
** Author: LiYuan
** E-Mail: 18069211@qq.com
** Version 2.0.1
** Last modified: 2019.01.11
** Modified By: LiYuan
** https://blog.csdn.net/liyuanbhu/article/details/86307283
****************************************************************************/


#ifndef QT_OPENCV_CONVERT_H
#define QT_OPENCV_CONVERT_H

#include <QDebug>
#include <QImage>
#include <opencv2/opencv.hpp>

/**
 * @brief toQImage 将cv::Mat类型图像转换为QImage类型的图像
 * @param mat 将OpenCV的cv::Mat类型图像转换为QImage类型
 * @param swap 只针对CV_8UC3格式，如果true则会调换R通道与B通道， BGR->RGB。
 * @return 转换后的 QImage 图像
 */
QImage toQImage(const cv::Mat& mat, bool swap = true);


/**
 * @brief toMat 将QImage的类型图像转换为cv::Mat类型
 * @param image 待转换的图像，支持 Format_Indexed8/Format_Grayscale、24位彩色、32位彩色格式，
 * @param swap 只针对RGB888格式，如果true则会调换R通道与B通道，RGB->BGR。
 * @return
 */
cv::Mat toMat(const QImage &image, bool swap = true);

#endif


