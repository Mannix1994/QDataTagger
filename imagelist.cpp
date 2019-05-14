#include "imagelist.h"
#include <QTextStream>
#include <QFile>
#include <QDebug>

ImageList::ImageList(QObject *parent) : QObject(parent), index(0)
{

}

bool ImageList::open(const QString &list_file)
{
    image_list.clear();
    index = 0;
    if(list_file.length()){
        QFile f(list_file);
        if (!f.open(QIODevice::ReadOnly | QIODevice::Text)){
            return false;
        }
        QTextStream ts(&f);
        while (!ts.atEnd()) {
            QString s = ts.readLine();
            image_list.push_back(ImageItem(s));
        }
        return true;
    }
    return false;
}

ImageItem ImageList::pre()
{
    ImageItem s;
    if (index>0){
        --index;
        s = image_list[index];
    }
    return s;
}

ImageItem ImageList::next()
{
    ImageItem s;
    if (index<image_list.length()-1){
        ++index;
        s = image_list[index];
    }
    return s;
}

ImageItem ImageList::current()
{
    if (image_list.size()>0)
        return image_list[index];
    else{
        return ImageItem();
    }
}

bool ImageList::empty()
{
    return image_list.empty();
}

ImageItem::ImageItem(QString a_line_of_list)
{
    QStringList sl = a_line_of_list.split(",");
    if(sl.length() != 2){
        qDebug() << "本行不是两个图像文件的路径. " << a_line_of_list;
    }
    _origin = sl[0];
    _target_ps = sl[1];
}
