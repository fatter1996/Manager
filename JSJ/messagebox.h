#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QObject>
#include<QMessageBox>
#include <QResizeEvent>
class MessageBox : public QMessageBox
{
    Q_OBJECT
public:
    MessageBox();
   static int SetMyStyle(QString text);
protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // MESSAGEBOX_H
