#ifndef EDITENTRY_H
#define EDITENTRY_H

#include <QWidget>
#include <QSqlDatabase>
#include <QBoxLayout>

class EditEntry : public QWidget
{
    Q_OBJECT

public:
    EditEntry(QString nameDb, QString entry, QSqlDatabase* db, QWidget* parent = nullptr);

private:
    void saveBooks();
    void saveOrders();
    void savePublishers();
    void saveCarts();
    void saveBuyers();
    void savePurchase();

    void deleteBooks();
    void deleteOrders();
    void deletePublishers();
    void deleteCarts();
    void deleteBuyers();
    void deletePurchase();

private slots:
    void saveEntry();
    void deleteEntry();

private:
    QSqlDatabase* database;
    QVBoxLayout* mainBox;
    QString nameDB;
    QStringList list;
};

#endif // EDITENTRY_H
