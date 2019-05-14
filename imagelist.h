#ifndef IMAGELIST_H
#define IMAGELIST_H

#include <QObject>
#include <QVector>

class ImageItem {
public:
    ImageItem(){}
    ImageItem(QString a_line_of_list);
    QString origin(){return _origin;}
    QString target_ps(){return _target_ps;}
    bool empty(){return _origin.length() == 0 || _target_ps.length() == 0;}
private:
    QString _origin;
    QString _target_ps;
};

/**
 * @brief 用来管理图片路径的类
 */
class ImageList : public QObject
{
    Q_OBJECT
public:
    explicit ImageList(QObject *parent = nullptr);
    /**
     * @brief open 打开一个包含图像文件列表的文件，并把路径全部存到此类中
     * @param list_file 路径文件。每一行是一个图像的绝对路径
     * @return 打开成功返回true
     */
    bool open(const QString &list_file);
    /**
     * @brief pre 返回前一个图像的路径
     * @return 如果不存在，返回空字符串
     */
    ImageItem pre();
    /**
     * @brief next 返回下一个图像的路径
     * @return 如果不存在，返回空字符串
     */
    ImageItem next();
    /**
     * @brief current 返回当前图像的路径
     * @return 如果没有打开列表文件，返回空字符串
     */
    ImageItem current();
    /**
     * @brief empty 返回当前的图像列表是否为空
     * @return 空返回true， 否则返回false
     */
    bool empty();
private:
    // 当前图像路径索引
    int index;
    // 存放图像路径的vector
    QVector<ImageItem> image_list;
signals:

public slots:
};

#endif // IMAGELIST_H
