#ifndef PRINTREPORTS_H
#define PRINTREPORTS_H

#include <QWidget>
#include <QPrinter>
#include <QSqlDatabase>
#include <QBoxLayout>
#include <QTextStream>

class PrintReports : public QWidget
{
    Q_OBJECT
public:
    PrintReports(QString nameDb, QSqlDatabase* db, int co = 0, int si = 0, QWidget* parent = nullptr);

    void print();

private:
    void createTable();
    void saveFile(QString name);

private slots:
    void clickedSave();
    void clickedPrint();
    void clickedBack();

private:
    QString nameDB;
    QSqlDatabase* database;
    QVBoxLayout* mainBox;
    QTextStream out;
    QString s;
    int count;
    int sign;
};

#endif // PRINTREPORTS_H
