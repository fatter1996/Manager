#ifndef ARROWLABEL_H
#define ARROWLABEL_H

#include <QObject>
#include<QLabel>
class ArrowLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ArrowLabel(QObject *parent = nullptr);

signals:
public:
    bool is_show = false;
};

#endif // ARROWLABEL_H
