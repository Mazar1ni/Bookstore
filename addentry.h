#ifndef ADDENTRY_H
#define ADDENTRY_H

#include <QWidget>
#include <QSqlDatabase>
#include <QBoxLayout>
#include <QLineEdit>

class AddEntry : public QWidget
{
    Q_OBJECT
public:
    AddEntry(QString nameDb, QSqlDatabase* db, QWidget* parent = nullptr);

private:
    void updatePurchase_amount();
    void updateOrder_amount();

private slots:
    void addEntry();
    void addBook();
    void addOrder();
    void addCart();
    void addPublisher();
    void addPurchase();
    void addBuyer();

    void updatePurchase_amountBook(QString name);
    void updatePurchase_amountBuyer(QString name);

    void updateOrder_amountBook(QString name);
    void updateOrder_amountBuyer(QString name);

private:
    QSqlDatabase* database;
    QVBoxLayout* mainBox;
    QString nameDB;
    QLineEdit* purchase_amount;
    QString price;
    QString discount;
    QLineEdit* order_amount;
    QString order_price;
    QString order_discount;
};

#endif // ADDENTRY_H
