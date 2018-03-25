#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QBoxLayout>

class Widget;

class MainMenu : public QWidget
{
    Q_OBJECT
public:
    MainMenu(QWidget* parent = nullptr);

private slots:
    void clickedBooksTable();
    void clickedCartsTable();
    void clickedRurchaseTable();
    void clickedOrdersTable();
    void clickedPublishersTable();
    void clickedBuyersTable();

    void clickedNotPurchasedRequest();
    void clickedNumberOfPurchasedRequest();
    void clickedMostOrderedRequest();

    void clickedNotPurchasedReports();
    void clickedNumberOfPurchasedReports();
    void clickedMostOrderedReports();
    void clickedBooksReports();

    void help();

private:
    Widget* widget;
    QVBoxLayout* mainBox;

};

#endif // MAINMENU_H
