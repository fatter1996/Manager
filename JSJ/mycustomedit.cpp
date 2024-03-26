#include "mycustomedit.h"
#include<QAction>
MyCustomEdit::MyCustomEdit()
{
    QAction* userAction = new QAction(this);
    //二、给action添加icon
    userAction->setIcon(QIcon(":/password.png"));
    //三、给空间添加action
    this->addAction(userAction,QLineEdit::LeadingPosition);

}
