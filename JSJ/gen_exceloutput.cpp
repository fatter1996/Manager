#include "gen_exceloutput.h"
#include <QDebug>
#include"jsj_services.h"
gen_Exceloutput::gen_Exceloutput(QObject *parent) : QObject(parent)
{

}
//第一个参数是页面上的表格，第二个是导出文件的表头数据
bool gen_Exceloutput::mergeCells(QAxObject *workSheet, QString start, QString end, QString value)
{
    if (workSheet == NULL)
        return false;
    QAxObject *range = workSheet->querySubObject("Range(const Qvariant&)", QVariant("=(" + start + ": " + end + ")"));
    if (range == NULL)
        return false;

    range->setProperty("MergeCells", true); // 合并单元格
    range->setProperty("Value", value);
    return true;
}

#include<QHeaderView>
void gen_Exceloutput::Table2ExcelByHtml(QTableWidget *table, QString title)
{
    QString fileName = QFileDialog::getSaveFileName(table, "保存", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), "Excel 文件(*.xls *.xlsx)");
    if (fileName != "")
    {
        QAxObject *excel = new QAxObject;
        if (excel->setControl("Excel.Application")) //连接Excel控件
        {
            excel->dynamicCall("SetVisible (bool Visible)", "false"); //不显示窗体
            excel->setProperty("DisplayAlerts", false);//不显示任何警告信息。如果为true那么在关闭是会出现类似“文件已修改，是否保存”的提示
            QAxObject *workbooks = excel->querySubObject("WorkBooks");//获取工作簿集合
            workbooks->dynamicCall("Add");//新建一个工作簿
            QAxObject *workbook = excel->querySubObject("ActiveWorkBook");//获取当前工作簿
            QAxObject *worksheet = workbook->querySubObject("Worksheets(int)", 1);

            int i, j;
            //QTablewidget 获取数据的列数
            int colcount = table->model()->columnCount();
            //QTablewidget 获取数据的行数
            int rowscount = table->model()->rowCount();
            QAxObject *cell, *col;
            //标题行
            cell = worksheet->querySubObject("Cells(int,int)", 1, 1);
            cell->dynamicCall("SetValue(const QString&)", title);
            cell->querySubObject("Font")->setProperty("Size", 18);
            cell->setProperty("HorizontalAlignment", -4108);//xlCenter
            cell->setProperty("VerticalAlignment", -4108);//xlCenter
            //调整行高
            worksheet->querySubObject("Range(const QString&)", "1:1")->setProperty("RowHeight", 30);
            //合并标题行
            QString cellTitle;
            cellTitle.append("A1:");
            cellTitle.append(QChar(colcount - 1 + 'A'));
            cellTitle.append(QString::number(1));
            QAxObject *range = worksheet->querySubObject("Range(const QString&)", cellTitle);
            range->setProperty("WrapText", true);
            range->setProperty("MergeCells", true);
            range->setProperty("HorizontalAlignment", -4108);//xlCenter
            range->setProperty("VerticalAlignment", -4108);//xlCenter
            //列标题
            for (i = 0; i < colcount; i++)
            {
                QString columnName;
                columnName.append(QChar(i  + 'A'));
                columnName.append(":");
                columnName.append(QChar(i + 'A'));
                col = worksheet->querySubObject("Columns(const QString&)", columnName);
                if (i == 0 || i == 3)
                    col->setProperty("ColumnWidth", 5);
                else if (i == 1 || i == 4)
                    col->setProperty("ColumnWidth", 20);
                else if (i == 9)
                    col->setProperty("ColumnWidth", 25);
                else if (i == 8)
                    col->setProperty("ColumnWidth", 15);
                else
                    col->setProperty("ColumnWidth", 10);

                cell = worksheet->querySubObject("Cells(int,int)", 2, i + 1);
                //QTableWidget 获取表格头部文字信息
                columnName = table->horizontalHeader()->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();

                qDebug()<<"CC:"<<columnName;//QTableView 获取表格头部文字信息
                // columnName=ui->tableView_right->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
                cell->dynamicCall("SetValue(const QString&)", columnName);
                cell->querySubObject("Font")->setProperty("Bold", true);
                cell->querySubObject("Interior")->setProperty("Color", QColor(191, 191, 191));
                cell->setProperty("HorizontalAlignment", -4108);//xlCenter
                cell->setProperty("VerticalAlignment", -4108);//xlCenter
            }
            //数据区

            //QTableWidget 获取表格数据部分
            for (i = 0; i < rowscount; i++)
            {
                for (j = 0; j < colcount; j++)
                {
                    QString str = table->item(i,j)->text();
                    qDebug()<<table->item(i,j)->text();
                    worksheet->querySubObject("Cells(int,int)", i + 3, j+1)->dynamicCall("SetValue(const QString&)", "'" +str);
                }
            }
            //画框线
            QString lrange;
            lrange.append("A2:");
            lrange.append(colcount - 1 + 'A');
            lrange.append(QString::number(table->model()->rowCount() + 2));
            range = worksheet->querySubObject("Range(const QString&)", lrange);
            range->querySubObject("Borders")->setProperty("LineStyle", QString::number(1));
            range->querySubObject("Borders")->setProperty("Color", QColor(0, 0, 0));
            //调整数据区行高
            QString rowsName;
            rowsName.append("2:");
            rowsName.append(QString::number(table->model()->rowCount() + 2));
            range = worksheet->querySubObject("Range(const QString&)", rowsName);
            range->setProperty("RowHeight", 20);
            workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(fileName)); //保存至fileName
            workbook->dynamicCall("Close()");//关闭工作簿
            excel->dynamicCall("Quit()");//关闭excel
            delete excel;
            excel = nullptr;
            if (QMessageBox::question(NULL, "完成", "文件已经导出，是否现在打开？", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
            {
                QDesktopServices::openUrl(QUrl("file:///" + QDir::toNativeSeparators(fileName)));
            }
        }
        else
        {
            QMessageBox::warning(NULL, "错误", "未能创建 Excel 对象，请安装 Microsoft Excel。", QMessageBox::Apply);
        }
    }
}
