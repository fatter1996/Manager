#ifndef GEN_EXCELOUTPUT_H
#define GEN_EXCELOUTPUT_H

#include <QObject>
#include <QTableView>
#include<QTableWidget>
#include <QFileDialog>
#include <QDesktopServices>
#include <QMessageBox>
#include <QAxObject>
class gen_Exceloutput : public QObject
{
    Q_OBJECT
public:
    explicit gen_Exceloutput(QObject *parent = nullptr);
public:
     void Table2ExcelByHtml(QTableWidget *table,QString title);
     bool mergeCells(QAxObject*workSheet,QString start, QString end, QString value);
signals:

};

#endif // GEN_EXCELOUTPUT_H
