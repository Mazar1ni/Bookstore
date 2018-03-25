#include "addentry.h"
#include <QLabel>
#include <QPushButton>
#include <QSqlQuery>
#include <qDebug>
#include <QComboBox>
#include <QSqlRecord>
#include <QDate>
#include <QComboBox>

AddEntry::AddEntry(QString nameDb, QSqlDatabase *db, QWidget *parent)
    : QWidget(parent), database(db), nameDB(nameDb)
{
    mainBox = new QVBoxLayout(this);
    mainBox->setSpacing(15);

    if(nameDb == "books")
    {
        QLabel* nameLabel = new QLabel(QString::fromLocal8Bit("Название книги"));

        QLineEdit* name = new QLineEdit;

        QLabel* publisherLabel = new QLabel(QString::fromLocal8Bit("Издатель"));

        QComboBox * id_publisher = new QComboBox;

        QLabel* authorLabel = new QLabel(QString::fromLocal8Bit("Автор"));

        QLineEdit* author = new QLineEdit;

        QLabel* genreLabel = new QLabel(QString::fromLocal8Bit("Жанр"));

        QLineEdit* genre = new QLineEdit;

        QLabel* priceLabel = new QLabel(QString::fromLocal8Bit("Цена"));

        QLineEdit* price = new QLineEdit;

        QSqlQuery query = QSqlQuery(*database);
        query.prepare("SELECT * FROM publishers");
        query.exec();

        while(query.next())
        {
            id_publisher->addItem(query.value("name").toString(), query.value("id_publisher").toString());
        }

        mainBox->addWidget(nameLabel);
        mainBox->addWidget(name);
        mainBox->addWidget(publisherLabel);
        mainBox->addWidget(id_publisher);
        mainBox->addWidget(authorLabel);
        mainBox->addWidget(author);
        mainBox->addWidget(genreLabel);
        mainBox->addWidget(genre);
        mainBox->addWidget(priceLabel);
        mainBox->addWidget(price);
    }
    else if(nameDb == "orders")
    {
        QLabel* id_publisherLabel = new QLabel(QString::fromLocal8Bit("Издатель"));

        QComboBox * id_publisher = new QComboBox;

        QLabel* id_bookLabel = new QLabel(QString::fromLocal8Bit("Название книги"));

        QComboBox * id_book = new QComboBox;

        connect(id_book, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateOrder_amountBook(QString)));

        connect(id_publisher, QOverload<int>::of(&QComboBox::currentIndexChanged),
                [this, id_book, id_publisher](){
            id_book->clear();

            QSqlQuery query = QSqlQuery(*database);
            query.prepare("SELECT id_book, name FROM books WHERE id_publisher = :id_publisher");
            query.bindValue(":id_publisher", id_publisher->currentData().toString());
            query.exec();

            while(query.next())
            {
                id_book->addItem(query.value("name").toString(), query.value("id_book").toString());
            }
        });

        QLabel* id_buyerLabel = new QLabel(QString::fromLocal8Bit("ФИО покупателя"));

        QComboBox * id_buyer = new QComboBox;

        connect(id_buyer, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateOrder_amountBuyer(QString)));

        QLabel* order_amountLabel = new QLabel(QString::fromLocal8Bit("Цена"));

        order_amount = new QLineEdit;
        order_amount->setEnabled(false);

        QLabel* order_dateLabel = new QLabel(QString::fromLocal8Bit("Цена"));

        QLineEdit* order_date = new QLineEdit;
        order_date->setText(QDate::currentDate().toString("dd.MM.yyyy"));

        QSqlQuery query = QSqlQuery(*database);
        query.prepare("SELECT * FROM publishers");
        query.exec();

        while(query.next())
        {
            id_publisher->addItem(query.value("name").toString(), query.value("id_publisher").toString());
        }

        query.prepare("SELECT * FROM buyers");
        query.exec();

        while(query.next())
        {
            id_buyer->addItem(query.value("FIO").toString(), query.value("id_buyer").toString());
        }

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
    else if(nameDb == "publishers")
    {
        QLabel* nameLabel = new QLabel("name");

        QLineEdit* name = new QLineEdit;

        mainBox->addWidget(nameLabel);
        mainBox->addWidget(name);
    }
    else if(nameDb == "purchase")
    {
        QLabel* id_publisherLabel = new QLabel(QString::fromLocal8Bit("Издатель"));

        QComboBox * id_publisher = new QComboBox;

        QLabel* id_bookLabel = new QLabel(QString::fromLocal8Bit("Название книги"));

        QComboBox * id_book = new QComboBox;
        connect(id_book, SIGNAL(currentIndexChanged(QString)), this, SLOT(updatePurchase_amountBook(QString)));

        connect(id_publisher, QOverload<int>::of(&QComboBox::currentIndexChanged),
                [this, id_book, id_publisher](){
            id_book->clear();

            QSqlQuery query = QSqlQuery(*database);
            query.prepare("SELECT id_book, name FROM books WHERE id_publisher = :id_publisher");
            query.bindValue(":id_publisher", id_publisher->currentData().toString());
            query.exec();

            while(query.next())
            {
                id_book->addItem(query.value("name").toString(), query.value("id_book").toString());
            }
        });

        QLabel* id_buyerLabel = new QLabel(QString::fromLocal8Bit("ФИО покупателя"));

        QComboBox * id_buyer = new QComboBox;
        connect(id_buyer, SIGNAL(currentIndexChanged(QString)), this, SLOT(updatePurchase_amountBuyer(QString)));

        QLabel* purchase_amountLabel = new QLabel(QString::fromLocal8Bit("Цена"));

        purchase_amount = new QLineEdit;
        purchase_amount->setEnabled(false);

        QLabel* purchase_dateLabel = new QLabel(QString::fromLocal8Bit("Дата покупки"));

        QLineEdit* purchase_date = new QLineEdit;
        purchase_date->setText(QDate::currentDate().toString("dd.MM.yyyy"));

        QSqlQuery query = QSqlQuery(*database);
        query.prepare("SELECT * FROM publishers");
        query.exec();

        while(query.next())
        {
            id_publisher->addItem(query.value("name").toString(), query.value("id_publisher").toString());
        }

        query.prepare("SELECT * FROM buyers");
        query.exec();

        while(query.next())
        {
            id_buyer->addItem(query.value("FIO").toString(), query.value("id_buyer").toString());
        }

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
    else if(nameDb == "buyers")
    {
        QLabel* FIOLabel = new QLabel(QString::fromLocal8Bit("ФИО"));

        QLineEdit* FIO = new QLineEdit;

        QLabel* birthdateLabel = new QLabel(QString::fromLocal8Bit("Дата Рождения"));

        QLineEdit* birthdate = new QLineEdit;

        QLabel* id_cartLabel = new QLabel(QString::fromLocal8Bit("Номер карты"));

        QComboBox * id_cart = new QComboBox;

        QSqlQuery query = QSqlQuery(*database);
        query.prepare("SELECT * FROM carts");
        query.exec();

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

        mainBox->addWidget(FIOLabel);
        mainBox->addWidget(FIO);
        mainBox->addWidget(birthdateLabel);
        mainBox->addWidget(birthdate);
        mainBox->addWidget(id_cartLabel);
        mainBox->addWidget(id_cart);
    }
    else if(nameDb == "carts")
    {
        QLabel* discountLabel = new QLabel(QString::fromLocal8Bit("Скидка"));

        QLineEdit* discount = new QLineEdit;

        mainBox->addWidget(discountLabel);
        mainBox->addWidget(discount);
    }

    QPushButton* addButton = new QPushButton;
    addButton->setText(QString::fromLocal8Bit("Добавить"));
    connect(addButton, SIGNAL(clicked(bool)), this, SLOT(addEntry()));

    mainBox->addWidget(addButton);

}

void AddEntry::updatePurchase_amount()
{
    if(discount == "")
    {
        purchase_amount->setText(price);
    }
    else
    {
        purchase_amount->setText(QString::number(price.toDouble() - (price.toDouble() * (discount.toDouble() / 100))));
    }
}

void AddEntry::updateOrder_amount()
{
    if(order_discount == "")
    {
        order_amount->setText(order_price);
    }
    else
    {
        order_amount->setText(QString::number(order_price.toDouble() - (order_price.toDouble() * (order_discount.toDouble() / 100))));
    }
}

void AddEntry::addEntry()
{
    if(nameDB == "books")
    {
        addBook();
        deleteLater();
    }
    else if(nameDB == "buyers")
    {
        addBuyer();
        deleteLater();
    }
    else if(nameDB == "carts")
    {
        addCart();
        deleteLater();
    }
    else if(nameDB == "orders")
    {
        addOrder();
        deleteLater();
    }
    else if(nameDB == "purchase")
    {
        addPurchase();
        deleteLater();
    }
    else if(nameDB == "publishers")
    {
        addPublisher();
        deleteLater();
    }
}

void AddEntry::addBook()
{
    QString name = dynamic_cast<QLineEdit*>(mainBox->takeAt(1)->widget())->text();
    QString id_publisher = dynamic_cast<QComboBox*>(mainBox->takeAt(2)->widget())->currentData().toString();;
    QString author = dynamic_cast<QLineEdit*>(mainBox->takeAt(3)->widget())->text();
    QString genre = dynamic_cast<QLineEdit*>(mainBox->takeAt(4)->widget())->text();
    QString price = dynamic_cast<QLineEdit*>(mainBox->takeAt(5)->widget())->text();
    QSqlQuery query = QSqlQuery(*database);
    query.prepare("INSERT INTO books (name, id_publisher, author, genre, price) VALUES (:name, :id_publisher, :author, :genre, :price)");
    query.bindValue(":name", name);
    query.bindValue(":id_publisher", id_publisher);
    query.bindValue(":author", author);
    query.bindValue(":genre", genre);
    query.bindValue(":price", price);
    query.exec();
}

void AddEntry::addOrder()
{
    QString id_book = dynamic_cast<QComboBox*>(mainBox->takeAt(1)->widget())->currentData().toString();

    dynamic_cast<QComboBox*>(mainBox->takeAt(2)->widget())->currentData().toString();

    QString id_buyer = dynamic_cast<QComboBox*>(mainBox->takeAt(3)->widget())->currentData().toString();

    QString order_amount = dynamic_cast<QLineEdit*>(mainBox->takeAt(4)->widget())->text();

    QString order_date = dynamic_cast<QLineEdit*>(mainBox->takeAt(5)->widget())->text();

    QSqlQuery query = QSqlQuery(*database);
    query.prepare("INSERT INTO orders (id_book, id_buyer, order_amount, order_date) VALUES (:id_book, :id_buyer, :order_amount, :order_date)");
    query.bindValue(":id_book", id_book);
    query.bindValue(":id_buyer", id_buyer);
    query.bindValue(":order_amount", order_amount);
    query.bindValue(":order_date", order_date);
    query.exec();
}

void AddEntry::addCart()
{
    QString discount = dynamic_cast<QLineEdit*>(mainBox->takeAt(1)->widget())->text();
    QSqlQuery query = QSqlQuery(*database);
    query.prepare("INSERT INTO carts (discount) VALUES (:discount)");
    query.bindValue(":discount", discount);
    query.exec();
}

void AddEntry::addPublisher()
{
    QString name = dynamic_cast<QLineEdit*>(mainBox->takeAt(1)->widget())->text();
    QSqlQuery query = QSqlQuery(*database);
    query.prepare("INSERT INTO publishers (name) VALUES (:name)");
    query.bindValue(":name", name);
    query.exec();
}

void AddEntry::addPurchase()
{
    dynamic_cast<QComboBox*>(mainBox->takeAt(1)->widget())->currentText();

    QString id_book = dynamic_cast<QComboBox*>(mainBox->takeAt(2)->widget())->currentData().toString();

    QString id_buyer = dynamic_cast<QComboBox*>(mainBox->takeAt(3)->widget())->currentData().toString();

    QString purchase_amount = dynamic_cast<QLineEdit*>(mainBox->takeAt(4)->widget())->text();
    QString purchase_date = dynamic_cast<QLineEdit*>(mainBox->takeAt(5)->widget())->text();
    QSqlQuery query = QSqlQuery(*database);
    query.prepare("INSERT INTO purchase (id_book, id_buyer, purchase_amount, purchase_date) VALUES (:id_book, :id_buyer, :purchase_amount, :purchase_date)");
    query.bindValue(":id_book", id_book);
    query.bindValue(":id_buyer", id_buyer);
    query.bindValue(":purchase_amount", purchase_amount.toDouble());
    query.bindValue(":purchase_date", purchase_date);
    query.exec();
}

void AddEntry::addBuyer()
{
    QString FIO = dynamic_cast<QLineEdit*>(mainBox->takeAt(1)->widget())->text();
    QString birthdate = dynamic_cast<QLineEdit*>(mainBox->takeAt(2)->widget())->text();
    QString id_cart = dynamic_cast<QComboBox*>(mainBox->takeAt(3)->widget())->currentText();
    QSqlQuery query = QSqlQuery(*database);
    query.prepare("INSERT INTO buyers (FIO, birthdate, id_cart) VALUES (:FIO, :birthdate, :id_cart)");
    query.bindValue(":FIO", FIO);
    query.bindValue(":birthdate", birthdate);
    query.bindValue(":id_cart", id_cart);
    query.exec();
}

void AddEntry::updatePurchase_amountBook(QString name)
{
    QSqlQuery query;
    query.exec("SELECT * FROM books");

    while(query.next())
    {
        if(query.value("name").toString() == name)
        {
            price = query.value("price").toString();
        }
    }
    updatePurchase_amount();
}

void AddEntry::updatePurchase_amountBuyer(QString name)
{
    QSqlQuery query = QSqlQuery(*database);
    query.prepare("SELECT * FROM buyers");
    query.exec();

    QString id_cart;

    while(query.next())
    {
        if(query.value("FIO").toString() == name)
        {
            id_cart = query.value("id_cart").toString();
        }
    }

    query.prepare("SELECT * FROM carts");
    query.exec();

    while(query.next())
    {
        if(query.value("id_cart").toString() == id_cart)
        {
            discount = query.value("discount").toString();
        }
    }
    updatePurchase_amount();
}

void AddEntry::updateOrder_amountBook(QString name)
{
    QSqlQuery query;
    query.exec("SELECT * FROM books");

    while(query.next())
    {
        if(query.value("name").toString() == name)
        {
            order_price = query.value("price").toString();
        }
    }
    updateOrder_amount();
}

void AddEntry::updateOrder_amountBuyer(QString name)
{
    QSqlQuery query = QSqlQuery(*database);
    query.prepare("SELECT * FROM buyers");
    query.exec();

    QString id_cart;

    while(query.next())
    {
        if(query.value("FIO").toString() == name)
        {
            id_cart = query.value("id_cart").toString();
        }
    }

    query.prepare("SELECT * FROM carts");
    query.exec();

    while(query.next())
    {
        if(query.value("id_cart").toString() == id_cart)
        {
            order_discount = query.value("discount").toString();
        }
    }
    updateOrder_amount();
}
