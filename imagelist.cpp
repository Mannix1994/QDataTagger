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
            image_list.push_back(s);
        }
        return true;
    }
    return false;
}

QString ImageList::pre()
{
    QString s;
    if (index>0){
        --index;
        s = image_list[index];
    }
    return s;
}

QString ImageList::next()
{
    QString s;
    if (index<image_list.length()-1){
        ++index;
        s = image_list[index];
    }
    return s;
}

QString ImageList::current()
{
    if (image_list.size()>0)
        return image_list[index];
    else{
        return "";
    }
}
