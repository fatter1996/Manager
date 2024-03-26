#ifndef MYTIMEEDIT_H
#define MYTIMEEDIT_H

#include <QObject>
#include<QTimeEdit>
#include"jsj_services.h"
class MyTimeEdit : public QTimeEdit
{
    Q_OBJECT
public:
    explicit MyTimeEdit(QWidget *parent = nullptr);
    explicit MyTimeEdit(const QTime &time, QWidget *parent = nullptr);
public slots:
    void SetTimeByUserSelect(int Index);
public:
    void GetType(QString Type);
public:
    QString Types;
};

#endif // MYTIMEEDIT_H
