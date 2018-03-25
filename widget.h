#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>

class MainMenu;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *par = nullptr);
    ~Widget();

    void booksShow();
    void ordersShow();
    void publishersShow();
    void purchaseShow();
    void cartsShow();
    void buyersShow();

    void notPurchasedRequest();
    void numberOfPurchasedRequest(int count, int sign);
    void mostOrderedRequest();

    void notPurchasedReports();
    void numberOfPurchasedReports(int count, int sign);
    void mostOrderedReports();
    void booksReports();

private:
    void updateSettingTable();
    void clearTable();

private slots:
    void addEntry();
    void clickedEntry(QModelIndex index);
    void updateTable();
    void searchEntry();
    void clickedBack();

private:
    QSqlDatabase database;
    QTableWidget* table;
    QString nameDatabaseIsShow = "";
    QLineEdit* search;
    QLabel* name;
    MainMenu* widget;
    QPushButton* addButton;
    int signL, countL;

};

#endif // WIDGET_H
