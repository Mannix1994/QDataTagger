#include "convert.h"
#include <QMessageBox>

QImage toQImage(const cv::Mat &mat, bool swap) {
    switch (mat.type())
    {
    // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB32);
        return image.copy();
    }

    // 8-bit, 3 channel
    case CV_8UC3:
    {
        QImage image( mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888 );
        if(swap)
            return image.copy().rgbSwapped();
        else
            return image.copy();
    }

    // 8-bit, 1 channel
    case CV_8UC1:
    {
        static QVector<QRgb>  sColorTable;
        // only create our color table once
        if (sColorTable.isEmpty())
        {
           for ( int i = 0; i < 256; ++i )
              sColorTable.push_back(qRgb( i, i, i ));
        }
        QImage image( mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        image.setColorTable(sColorTable);
        return image.copy();
    }

    default:
        qDebug("Image format is not supported: depth=%d and %d channels\n", mat.depth(), mat.channels());
        break;
    }
    return QImage();
}

cv::Mat toMat(const QImage &image, bool swap) {
    switch (image.format())
    {
    // 8-bit, 4 channel
    case QImage::Format_RGB32:
    {
        cv::Mat mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine());
        return mat.clone();
    }
    // 8-bit, 3 channel
    case QImage::Format_RGB888:
    {
        QImage s;
        if(swap)
            s = image.rgbSwapped();
        else
            s = image.copy();
        return cv::Mat(s.height(), s.width(), CV_8UC3, const_cast<uchar*>(s.bits()), s.bytesPerLine() ).clone();
    }

    // 8-bit, 1 channel
    case QImage::QImage::Format_Grayscale8:
    case QImage::Format_Indexed8:
    {
        cv::Mat  mat(image.height(), image.width(), CV_8UC1, const_cast<uchar*>(image.bits()), image.bytesPerLine());
        return mat.clone();
    }

    default:
        qDebug("Image format is not supported: depth=%d and %d format\n", image.depth(), image.format());
        break;
    }

    return cv::Mat();
}
