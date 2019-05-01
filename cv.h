#ifndef CV_H
#define CV_H

#include <QObject>
#include <opencv2/opencv.hpp>

class CVFunctions : public QObject
{
    Q_OBJECT
public:
    explicit CVFunctions(QObject *parent = nullptr);
    bool open(const QString &im_path);
    bool open(const QImage &image);
    QImage canny(int threshold1, int threshold2);
    QImage withCannny(int threshold1, int threshold2);
    QImage mask();
private:
    cv::Mat _canny;
    cv::Mat _origin;
    cv::Mat _mask;
    cv::Mat _gray;
signals:

public slots:
};

#endif // CV_H
