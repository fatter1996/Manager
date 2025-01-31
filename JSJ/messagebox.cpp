﻿#include "messagebox.h"

MessageBox::MessageBox()
{

}

int MessageBox::SetMyStyle(QString text)
{
    MessageBox msgBox;
    msgBox.setStyleSheet("QMessageBox { background-color: #007fff;}"
                         "QMessageBox QLabel#qt_msgbox_label { textLabel color: #298DFF;"
                         "background-color: transparent;"
                         "min-width: 240px;"
                         "min-height: 40px;}"
                         "QMessageBox QLabel#qt_msgboxex_icon_label {width: 40px;"
                         "height: 40px;}"
                         "QMessageBox QPushButton { border: 1px solid #298DFF;"
                         "border-radius: 3px;"
                         "background-color: #F2F2F2;"
                         "color: #298DFF;"
                         "font-family: Microsoft YaHei;"
                         "font-size: 10pt;"
                         "min-width: 70px;"
                         "min-height: 25px;"
                         "}"
                         "QMessageBox QPushButton:hover {background-color: #298DFF;"
                         "color: #F2F2F2;"
                         "}"
                         "QMessageBox QPushButton:pressed {"
                         "background-color: #257FE6;"
                         "}"
                         "QMessageBox QDialogButtonBox#qt_msgbox_buttonbox { "
                         "button-layout: 0; "
                         "}");
    msgBox.setText(text);
    return msgBox.exec();
}

void MessageBox::resizeEvent(QResizeEvent *event)
{


}
