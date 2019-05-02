#ifndef CV_H
#define CV_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include <string>

class CVFunctions : public QObject
{
    Q_OBJECT
public:
    typedef enum {CANNY, WITH_CANNY, MASK, ORIGIN} WINDOW;
    explicit CVFunctions(QObject *parent = nullptr);
    bool open(const QString &im_path);
    bool open(QImage &image);
    bool isOpened();
    QImage canny(int blur, int threshold1, int threshold2, bool show=false);
    QImage withCanny(int blur, int threshold1, int threshold2, bool show=false);
    QImage mask(bool show=false);
    QImage origin(bool show=false);
    void showMat(std::string title, cv::Mat &mat);
    void closeWindow(WINDOW window);
private:
    cv::Mat _canny;
    cv::Mat _origin;
    cv::Mat _mask;
    cv::Mat _gray;
    bool _open;
signals:

public slots:
};

#endif // CV_H
