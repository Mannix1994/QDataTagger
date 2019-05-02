#ifndef IMAGELIST_H
#define IMAGELIST_H

#include <QObject>
#include <QVector>

class ImageList : public QObject
{
    Q_OBJECT
public:
    explicit ImageList(QObject *parent = nullptr);
    bool open(const QString &list_file);
    QString pre();
    QString next();
    QString current();
    bool empty();
private:
    int index;
    QVector<QString> image_list;
signals:

public slots:
};

#endif // IMAGELIST_H
