#include "editentry.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlQuery>
#include <qDebug>
#include <QComboBox>

EditEntry::EditEntry(QString nameDb, QString entry, QSqlDatabase* db, QWidget *parent)
    : QWidget(parent), database(db), nameDB(nameDb)
{
    //setFixedSize(200, 400);
    int index = 0;

    while(index != -1)
    {
       index = entry.indexOf("~");
       list.append(entry.left(index));
       entry.remove(0, index + 1);
    }

    mainBox = new QVBoxLayout(this);
    mainBox->setSpacing(15);

    if(nameDb == "books")
    {
        QLabel* idLabel = new QLabel(QString::fromLocal8Bit("Номер"));

        QLineEdit* id = new QLineEdit(list[0]);
        id->setEnabled(false);

        QLabel* nameLabel = new QLabel(QString::fromLocal8Bit("Название книги"));

        QLineEdit* name = new QLineEdit(list[1]);

        QLabel* publisherLabel = new QLabel(QString::fromLocal8Bit("Издатель"));

        QComboBox* publisher = new QComboBox;

        QLabel* authorLabel = new QLabel(QString::fromLocal8Bit("Автор"));

        QLineEdit* author = new QLineEdit(list[3]);

        QLabel* genreLabel = new QLabel(QString::fromLocal8Bit("Жанр"));

        QLineEdit* genre = new QLineEdit(list[4]);

        QLabel* priceLabel = new QLabel(QString::fromLocal8Bit("Цена"));

        QLineEdit* price = new QLineEdit(list[5]);

        QSqlQuery query = QSqlQuery(*database);
        query.prepare("SELECT * FROM publishers");
        query.exec();

        while(query.next())
        {
            publisher->addItem(query.value("name").toString(), query.value("id_publisher").toString());
            if(list[2] == query.value("name").toString())
            {
                publisher->setCurrentText(list[2]);
            }
        }

        mainBox->addWidget(idLabel);
        mainBox->addWidget(id);
        mainBox->addWidget(nameLabel);
        mainBox->addWidget(name);
        mainBox->addWidget(publisherLabel);
        mainBox->addWidget(publisher);
        mainBox->addWidget(authorLabel);
        mainBox->addWidget(author);
        mainBox->addWidget(genreLabel);
        mainBox->addWidget(genre);
        mainBox->addWidget(priceLabel);
        mainBox->addWidget(price);
    }
    else if(nameDb == "carts")
    {
        QLabel* idLabel = new QLabel(QString::fromLocal8Bit("Номер"));

        QLineEdit* id = new QLineEdit(list[0]);
        id->setEnabled(false);

        QLabel* discountLabel = new QLabel(QString::fromLocal8Bit("Скидка"));

        QLineEdit* discount = new QLineEdit(list[1]);

        mainBox->addWidget(idLabel);
        mainBox->addWidget(id);
        mainBox->addWidget(discountLabel);
        mainBox->addWidget(discount);
    }
    else if(nameDb == "buyers")
    {
        QLabel* idLabel = new QLabel(QString::fromLocal8Bit("Номер"));

        QLineEdit* id = new QLineEdit(list[0]);
        id->setEnabled(false);

        QLabel* FIOLabel = new QLabel(QString::fromLocal8Bit("ФИО"));

        QLineEdit* FIO = new QLineEdit(list[1]);

        QLabel* birthdateLabel = new QLabel(QString::fromLocal8Bit("День Рождение"));

        QLineEdit* birthdate = new QLineEdit(list[2]);

        QLabel* id_cartLabel = new QLabel(QString::fromLocal8Bit("Номер карты"));

        QComboBox * id_cart = new QComboBox;

        QSqlQuery query = QSqlQuery(*database);
        query.prepare("SELECT * FROM carts");
        query.exec();

        id_cart->addItem(list[3]);
        id_cart->setCurrentText(list[3]);

        while(query.next())
        {
            QSqlQuery queryTest = QSqlQuery(*database);
            queryTest.prepare("SELECT * FROM buyers");
            queryTest.exec();

            bool z = false;

            while(queryTest.next())
            {
                if(queryTest.value("id_cart").toString() == query.value("id_cart").toString())
                {
                    z = true;
                    break;
                }
            }

            if(z == false)
            {
                id_cart->addItem(query.value("id_cart").toString());
            }
        }

        mainBox->addWidget(idLabel);
        mainBox->addWidget(id);
        mainBox->addWidget(FIOLabel);
        mainBox->addWidget(FIO);
        mainBox->addWidget(birthdateLabel);
        mainBox->addWidget(birthdate);
        mainBox->addWidget(id_cartLabel);
        mainBox->addWidget(id_cart);
    }
    else if(nameDb == "orders")
    {
        QLabel* idLabel = new QLabel(QString::fromLocal8Bit("Номер"));

        QLineEdit* id = new QLineEdit(list[0]);
        id->setEnabled(false);

        QLabel* id_publisherLabel = new QLabel(QString::fromLocal8Bit("Издатель"));

        QComboBox * id_publisher = new QComboBox;

        QLabel* id_bookLabel = new QLabel(QString::fromLocal8Bit("Название книги"));

        QComboBox * id_book = new QComboBox;

        QString* s = new QString(list[2]);

        connect(id_publisher, QOverload<const QString &>::of(&QComboBox::currentIndexChanged),
                [this, id_book, id_publisher, s](){
            id_book->clear();

            QSqlQuery query = QSqlQuery(*database);
            query.prepare("SELECT id_book, name FROM books WHERE id_publisher = :id_publisher");
            query.bindValue(":id_publisher", id_publisher->currentData().toString());
            query.exec();

            while(query.next())
            {
                id_book->addItem(query.value("name").toString(), query.value("id_book").toString());
                if(s == query.value("name").toString())
                {
                    id_book->setCurrentText(*s);
                }
            }
        });

        QLabel* id_buyerLabel = new QLabel(QString::fromLocal8Bit("ФИО покупателя"));

        QComboBox * id_buyer = new QComboBox;

        QLabel* order_amountLabel = new QLabel(QString::fromLocal8Bit("Цена"));

        QLineEdit* order_amount = new QLineEdit(list[4]);

        QLabel* order_dateLabel = new QLabel(QString::fromLocal8Bit("Дата заказа"));

        QLineEdit* order_date = new QLineEdit(list[5]);

        QSqlQuery query = QSqlQuery(*database);
        query.prepare("SELECT * FROM publishers");
        query.exec();

        while(query.next())
        {
            id_publisher->addItem(query.value("name").toString(), query.value("id_publisher").toString());
            if(query.value("name").toString() == list[1])
            {
                id_publisher->setCurrentText(query.value("name").toString());
            }
        }

        query.prepare("SELECT * FROM buyers");
        query.exec();

        while(query.next())
        {
            id_buyer->addItem(query.value("FIO").toString(), query.value("id_buyer").toString());
            if(query.value("FIO").toString() == list[3])
            {
                id_buyer->setCurrentText(list[3]);
            }
        }

        mainBox->addWidget(idLabel);
        mainBox->addWidget(id);
        mainBox->addWidget(id_publisherLabel);
        mainBox->addWidget(id_publisher);
        mainBox->addWidget(id_bookLabel);
        mainBox->addWidget(id_book);
        mainBox->addWidget(id_buyerLabel);
        mainBox->addWidget(id_buyer);
        mainBox->addWidget(order_amountLabel);
        mainBox->addWidget(order_amount);
        mainBox->addWidget(order_dateLabel);
        mainBox->addWidget(order_date);
    }
    else if(nameDb == "purchase")
    {

        QLabel* idLabel = new QLabel(QString::fromLocal8Bit("Номер"));

        QLineEdit* id = new QLineEdit(list[0]);
        id->setEnabled(false);

        QLabel* id_publisherLabel = new QLabel(QString::fromLocal8Bit("Издатель"));

        QComboBox * id_publisher = new QComboBox;

        QLabel* id_bookLabel = new QLabel(QString::fromLocal8Bit("Название книги"));

        QComboBox * id_book = new QComboBox;

        QString* s = new QString(list[2]);

        connect(id_publisher, QOverload<const QString &>::of(&QComboBox::currentIndexChanged),
                [this, id_book, id_publisher, s](){
            id_book->clear();

            QSqlQuery query = QSqlQuery(*database);
            query.prepare("SELECT id_book, name FROM books WHERE id_publisher = :id_publisher");
            query.bindValue(":id_publisher", id_publisher->currentData().toString());
            query.exec();

            while(query.next())
            {
                id_book->addItem(query.value("name").toString(), query.value("id_book").toString());
                if(s == query.value("name").toString())
                {
                    id_book->setCurrentText(*s);
                }
            }
        });

        QLabel* id_buyerLabel = new QLabel(QString::fromLocal8Bit("ФИО покупателя"));

        QComboBox * id_buyer = new QComboBox;

        QLabel* purchase_amountLabel = new QLabel(QString::fromLocal8Bit("Цена"));

        QLineEdit* purchase_amount = new QLineEdit(list[4]);

        QLabel* purchase_dateLabel = new QLabel(QString::fromLocal8Bit("Дата покупки"));

        QLineEdit* purchase_date = new QLineEdit(list[5]);

        QSqlQuery query = QSqlQuery(*database);

        query.prepare("SELECT * FROM publishers");
        query.exec();

        while(query.next())
        {
            id_publisher->addItem(query.value("name").toString(), query.value("id_publisher").toString());
            if(query.value("name").toString() == list[1])
            {
                id_publisher->setCurrentText(query.value("name").toString());
            }
        }

        query.prepare("SELECT * FROM buyers");
        query.exec();

        while(query.next())
        {
            id_buyer->addItem(query.value("FIO").toString(), query.value("id_buyer").toString());
            if(query.value("FIO").toString() == list[3])
            {
                id_buyer->setCurrentText(query.value("FIO").toString());
            }
        }

        mainBox->addWidget(idLabel);
        mainBox->addWidget(id);
        mainBox->addWidget(id_publisherLabel);
        mainBox->addWidget(id_publisher);
        mainBox->addWidget(id_bookLabel);
        mainBox->addWidget(id_book);
        mainBox->addWidget(id_buyerLabel);
        mainBox->addWidget(id_buyer);
        mainBox->addWidget(purchase_amountLabel);
        mainBox->addWidget(purchase_amount);
        mainBox->addWidget(purchase_dateLabel);
        mainBox->addWidget(purchase_date);
    }
    else if(nameDb == "publishers")
    {
        QLabel* idLabel = new QLabel(QString::fromLocal8Bit("Номер"));

        QLineEdit* id = new QLineEdit(list[0]);
        id->setEnabled(false);

        QLabel* nameLabel = new QLabel(QString::fromLocal8Bit("Издатель"));

        QLineEdit* name = new QLineEdit(list[1]);

        mainBox->addWidget(idLabel);
        mainBox->addWidget(id);
        mainBox->addWidget(nameLabel);
        mainBox->addWidget(name);
    }

    QPushButton* saveButton = new QPushButton;
    saveButton->setText(QString::fromLocal8Bit("Сохранить"));
    connect(saveButton, SIGNAL(clicked(bool)), this, SLOT(saveEntry()));

    QPushButton* deleteButton = new QPushButton;
    deleteButton->setText(QString::fromLocal8Bit("Удалить"));
    connect(deleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteEntry()));

    mainBox->addWidget(saveButton);
    mainBox->addWidget(deleteButton);
}

void EditEntry::saveBooks()
{
    QString id = dynamic_cast<QLineEdit*>(mainBox->takeAt(1)->widget())->text();
    QString name = dynamic_cast<QLineEdit*>(mainBox->takeAt(2)->widget())->text();
    QString publisher = dynamic_cast<QComboBox*>(mainBox->takeAt(3)->widget())->currentData().toString();
    QString author = dynamic_cast<QLineEdit*>(mainBox->takeAt(4)->widget())->text();
    QString genre = dynamic_cast<QLineEdit*>(mainBox->takeAt(5)->widget())->text();
    QString price = dynamic_cast<QLineEdit*>(mainBox->takeAt(6)->widget())->text();
    QSqlQuery query = QSqlQuery(*database);
    query.prepare("UPDATE books SET name = :name, id_publisher = :publisher, author = :author, genre = :genre, price = :price WHERE id_book = :id");
    query.bindValue(":id", id);
    query.bindValue(":name", name);
    query.bindValue(":publisher", publisher);
    query.bindValue(":author", author);
    query.bindValue(":genre", genre);
    query.bindValue(":price", price);
    query.exec();
}

void EditEntry::saveOrders()
{
    QString id = dynamic_cast<QLineEdit*>(mainBox->takeAt(1)->widget())->text();

    dynamic_cast<QComboBox*>(mainBox->takeAt(2)->widget())->currentData().toString();

    QString id_book = dynamic_cast<QComboBox*>(mainBox->takeAt(3)->widget())->currentData().toString();

    QString id_buyer = dynamic_cast<QComboBox*>(mainBox->takeAt(4)->widget())->currentData().toString();
    QString order_amount = dynamic_cast<QLineEdit*>(mainBox->takeAt(5)->widget())->text();
    QString order_date = dynamic_cast<QLineEdit*>(mainBox->takeAt(6)->widget())->text();
    QSqlQuery query = QSqlQuery(*database);
    query.prepare("UPDATE orders SET id_book = :id_book, id_buyer = :id_buyer, order_amount = :order_amount, order_date = :order_date WHERE id_order = :id");
    query.bindValue(":id", id);
    query.bindValue(":id_book", id_book);
    query.bindValue(":id_buyer", id_buyer);
    query.bindValue(":order_amount", order_amount);
    query.bindValue(":order_date", order_date);
    query.exec();
}

void EditEntry::savePublishers()
{
    QString id = dynamic_cast<QLineEdit*>(mainBox->takeAt(1)->widget())->text();
    QString name = dynamic_cast<QLineEdit*>(mainBox->takeAt(2)->widget())->text();
    QSqlQuery query = QSqlQuery(*database);
    query.prepare("UPDATE publishers SET name = :name WHERE id_publishers = :id");
    query.bindValue(":id", id);
    query.bindValue(":name", name);
    query.exec();
}

void EditEntry::saveCarts()
{
    QString id = dynamic_cast<QLineEdit*>(mainBox->takeAt(1)->widget())->text();
    QString discount = dynamic_cast<QLineEdit*>(mainBox->takeAt(2)->widget())->text();
    QSqlQuery query = QSqlQuery(*database);
    query.prepare("UPDATE carts SET discount = :discount WHERE id_cart = :id");
    query.bindValue(":id", id);
    query.bindValue(":discount", discount);
    query.exec();
}

void EditEntry::saveBuyers()
{
    QString id = dynamic_cast<QLineEdit*>(mainBox->takeAt(1)->widget())->text();
    QString FIO = dynamic_cast<QLineEdit*>(mainBox->takeAt(2)->widget())->text();
    QString birthdate = dynamic_cast<QLineEdit*>(mainBox->takeAt(3)->widget())->text();
    QString id_cart = dynamic_cast<QComboBox*>(mainBox->takeAt(4)->widget())->currentText();
    QSqlQuery query = QSqlQuery(*database);
    query.prepare("UPDATE buyers SET FIO = :FIO, birthdate = :birthdate, id_cart = :id_cart WHERE id_buyer = :id");
    query.bindValue(":id", id);
    query.bindValue(":FIO", FIO);
    query.bindValue(":birthdate", birthdate);
    query.bindValue(":id_cart", id_cart);
    query.exec();
}

void EditEntry::savePurchase()
{
    QString id = dynamic_cast<QLineEdit*>(mainBox->takeAt(1)->widget())->text();

    dynamic_cast<QComboBox*>(mainBox->takeAt(2)->widget())->currentData().toString();

    QString id_book = dynamic_cast<QComboBox*>(mainBox->takeAt(3)->widget())->currentData().toString();

    QString id_buyer = dynamic_cast<QComboBox*>(mainBox->takeAt(4)->widget())->currentData().toString();

    QString purchase_amount = dynamic_cast<QLineEdit*>(mainBox->takeAt(5)->widget())->text();
    QString purchase_date = dynamic_cast<QLineEdit*>(mainBox->takeAt(6)->widget())->text();
    QSqlQuery query = QSqlQuery(*database);
    query.prepare("UPDATE purchase SET id_book = :id_book, id_buyer = :id_buyer, purchase_amount = :purchase_amount, purchase_date = :purchase_date WHERE id_purchase = :id");
    query.bindValue(":id", id);
    query.bindValue(":id_book", id_book);
    query.bindValue(":purchase_date", purchase_date);
    query.bindValue(":id_buyer", id_buyer);
    query.bindValue(":purchase_amount", purchase_amount);
    query.exec();  
}

void EditEntry::deleteBooks()
{
    QSqlQuery query = QSqlQuery(*database);
    query.prepare("DELETE FROM books WHERE id_book = :id");
    query.bindValue(":id", dynamic_cast<QLineEdit*>(mainBox->takeAt(1)->widget())->text());
    query.exec();
}

void EditEntry::deleteOrders()
{
    QSqlQuery query = QSqlQuery(*database);
    query.prepare("DELETE FROM orders WHERE id_order = :id");
    query.bindValue(":id", dynamic_cast<QLineEdit*>(mainBox->takeAt(1)->widget())->text());
    query.exec();
}

void EditEntry::deletePublishers()
{
    QSqlQuery query = QSqlQuery(*database);
    query.prepare("DELETE FROM publischers WHERE id_publischers = :id");
    query.bindValue(":id", dynamic_cast<QLineEdit*>(mainBox->takeAt(1)->widget())->text());
    query.exec();
}

void EditEntry::deleteCarts()
{
    QSqlQuery query = QSqlQuery(*database);
    query.prepare("DELETE FROM carts WHERE id_cart = :id");
    query.bindValue(":id", dynamic_cast<QLineEdit*>(mainBox->takeAt(1)->widget())->text());
    query.exec();
}

void EditEntry::deleteBuyers()
{
    QSqlQuery query = QSqlQuery(*database);
    query.prepare("DELETE FROM buyers WHERE id_buyer = :id");
    query.bindValue(":id", dynamic_cast<QLineEdit*>(mainBox->takeAt(1)->widget())->text());
    query.exec();
}

void EditEntry::deletePurchase()
{
    QSqlQuery query = QSqlQuery(*database);
    query.prepare("DELETE FROM purchase WHERE id_purchase = :id");
    query.bindValue(":id", dynamic_cast<QLineEdit*>(mainBox->takeAt(1)->widget())->text());
    query.exec();
}

void EditEntry::saveEntry()
{
    if(nameDB == "books")
    {
        saveBooks();
        deleteLater();
    }
    else if(nameDB == "buyers")
    {
        saveBuyers();
        deleteLater();
    }
    else if(nameDB == "carts")
    {
        saveCarts();
        deleteLater();
    }
    else if(nameDB == "orders")
    {
        saveOrders();
        deleteLater();
    }
    else if(nameDB == "purchase")
    {
        savePurchase();
        deleteLater();
    }
    else if(nameDB == "publishers")
    {
        savePublishers();
        deleteLater();
    }
}

void EditEntry::deleteEntry()
{
    if(nameDB == "books")
    {
        deleteBooks();
        deleteLater();
    }
    else if(nameDB == "buyers")
    {
        deleteBuyers();
        deleteLater();
    }
    else if(nameDB == "carts")
    {
        deleteCarts();
        deleteLater();
    }
    else if(nameDB == "orders")
    {
        deleteOrders();
        deleteLater();
    }
    else if(nameDB == "purchase")
    {
        deletePurchase();
        deleteLater();
    }
    else if(nameDB == "publishers")
    {
        deletePublishers();
        deleteLater();
    }
}
