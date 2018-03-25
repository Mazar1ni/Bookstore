#include "widget.h"
#include <QSqlQuery>
#include <qDebug>
#include <QBoxLayout>
#include <QHeaderView>
#include <QSqlError>
#include "editentry.h"
#include "addentry.h"
#include "mainmenu.h"
#include "printreports.h"

Widget::Widget(QWidget *par) : QWidget(par)
{
    this->resize(600, 560);

    QVBoxLayout* rightVBox = new QVBoxLayout(this);

    // подключение к бд
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("bookstore.db");

    database.open();

    QPushButton* back = new QPushButton;
    back->setText(QString::fromLocal8Bit("Назад"));
    back->setFixedSize(50, 35);
    connect(back, SIGNAL(clicked(bool)), this, SLOT(clickedBack()));

    name = new QLabel;
    name->setAlignment(Qt::AlignCenter);
    QFont font = name->font();
    font.setPointSize(20);
    font.setBold(true);
    font.setItalic(true);
    name->setFont(font);

    QHBoxLayout* upHBox = new  QHBoxLayout;
    upHBox->addWidget(back);
    upHBox->addWidget(name);

    rightVBox->addLayout(upHBox);

    search = new QLineEdit;
    QPushButton* searchButton = new QPushButton;
    searchButton->setText(QString::fromLocal8Bit("Поиск"));
    connect(searchButton, SIGNAL(clicked(bool)), this, SLOT(searchEntry()));

    QHBoxLayout* searchHBox = new QHBoxLayout;
    searchHBox->addWidget(search);
    searchHBox->addWidget(searchButton);

    rightVBox->addLayout(searchHBox, Qt::AlignTop);

    table = new QTableWidget;
    table->verticalHeader()->hide();

    connect(table, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(clickedEntry(QModelIndex)));
    rightVBox->addWidget(table, Qt::AlignTop);

    QHBoxLayout* hBox = new QHBoxLayout;

    addButton = new QPushButton;
    addButton->setFixedSize(125, 40);
    addButton->setText(QString::fromLocal8Bit("Добавить запись"));
    hBox->addWidget(addButton);
    connect(addButton, SIGNAL(clicked(bool)), this, SLOT(addEntry()));

    rightVBox->addLayout(hBox);

    table->setShowGrid(true);
}

Widget::~Widget()
{

}

void Widget::clickedBack()
{
    widget = new MainMenu;
    widget->show();
    deleteLater();
}

void Widget::updateSettingTable()
{
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->resizeColumnsToContents();
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setStretchLastSection(true);
}

void Widget::clearTable()
{
    for(int i = 0; i < table->rowCount(); i++)
    {
        for(int j = 0; j < table->columnCount(); j++)
        {
            delete table->item(i, j);
        }
    }

    table->setColumnCount(0);
    table->setRowCount(0);
    table->horizontalHeader()->setStretchLastSection(false);
}

void Widget::booksShow()
{
    name->setText(QString::fromLocal8Bit("Книги"));
    clearTable();

    QSqlQuery query = QSqlQuery(database);
    query.prepare("SELECT books.id_book, books.name, publishers.name, "
                  "books.author, books.genre, books.price "
                  "FROM books, publishers "
                  "WHERE books.id_publisher = publishers.id_publisher");
    query.exec();

    int row = 0;

    table->setColumnCount(6); // Указываем число колонок

    QStringList headers;
    headers << QString::fromLocal8Bit("Номер") << QString::fromLocal8Bit("Название")
            << QString::fromLocal8Bit("Издатель") << QString::fromLocal8Bit("Автор")
            << QString::fromLocal8Bit("Жанр") << QString::fromLocal8Bit("Цена");

    table->setHorizontalHeaderLabels(headers);

    for(int i = 0; query.next(); i++)
    {
        table->insertRow(i);
        table->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
        table->setItem(i, 1, new QTableWidgetItem(query.value(1).toString()));
        table->setItem(i, 2, new QTableWidgetItem(query.value(2).toString()));
        table->setItem(i, 3, new QTableWidgetItem(query.value(3).toString()));
        table->setItem(i, 4, new QTableWidgetItem(query.value(4).toString()));
        table->setItem(i, 5, new QTableWidgetItem(query.value(5).toString()));
        row++;
    }

    updateSettingTable();

    nameDatabaseIsShow = "books";
}

void Widget::ordersShow()
{
    name->setText(QString::fromLocal8Bit("Заказы"));
    clearTable();
    QSqlQuery query = QSqlQuery(database);
    query.prepare("SELECT orders.id_order, books.name, publishers.name, "
                  "buyers.FIO, orders.order_amount, orders.order_date "
                  "FROM orders, books, buyers, publishers "
                  "WHERE orders.id_book = books.id_book "
                  "AND books.id_publisher = publishers.id_publisher "
                  "AND orders.id_buyer = buyers.id_buyer");
    query.exec();

    int row = 0;

    table->setColumnCount(6); // Указываем число колонок

    QStringList headers;
    headers << QString::fromLocal8Bit("Номер") << QString::fromLocal8Bit("Название книги")
            << QString::fromLocal8Bit("Издатель") << QString::fromLocal8Bit("ФИО покупателя")
            << QString::fromLocal8Bit("Цена") << QString::fromLocal8Bit("Дата покупки");
    table->setHorizontalHeaderLabels(headers);

    for(int i = 0; query.next(); i++)
    {
        table->insertRow(i);
        table->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
        table->setItem(i, 1, new QTableWidgetItem(query.value(1).toString()));
        table->setItem(i, 2, new QTableWidgetItem(query.value(2).toString()));
        table->setItem(i, 3, new QTableWidgetItem(query.value(3).toString()));
        table->setItem(i, 4, new QTableWidgetItem(query.value(4).toString()));
        table->setItem(i, 5, new QTableWidgetItem(query.value(5).toString()));
        row++;
    }

    updateSettingTable();

    nameDatabaseIsShow = "orders";
}

void Widget::publishersShow()
{
    name->setText(QString::fromLocal8Bit("Издатели"));
    clearTable();
    QSqlQuery query = QSqlQuery(database);
    query.prepare("SELECT * FROM publishers");
    query.exec();

    int row = 0;

    table->setColumnCount(2); // Указываем число колонок

    QStringList headers;
    headers << QString::fromLocal8Bit("Номер") << QString::fromLocal8Bit("Издатель");
    table->setHorizontalHeaderLabels(headers);

    for(int i = 0; query.next(); i++)
    {
        table->insertRow(i);
        table->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
        table->setItem(i, 1, new QTableWidgetItem(query.value(1).toString()));
        row++;
    }

    updateSettingTable();

    nameDatabaseIsShow = "publishers";
}

void Widget::purchaseShow()
{
    name->setText(QString::fromLocal8Bit("Покупки"));
    clearTable();
    QSqlQuery query = QSqlQuery(database);
    query.prepare("SELECT purchase.id_purchase, books.name, publishers.name, "
                  "buyers.FIO, purchase.purchase_amount, purchase.purchase_date "
                  "FROM purchase, books, publishers, buyers "
                  "WHERE purchase.id_book = books.id_book "
                  "AND books.id_publisher = publishers.id_publisher "
                  "AND purchase.id_buyer = buyers.id_buyer");
    query.exec();

    int row = 0;

    table->setColumnCount(6); // Указываем число колонок

    QStringList headers;
    headers << QString::fromLocal8Bit("Номер") << QString::fromLocal8Bit("Название книги")
            << QString::fromLocal8Bit("Издатель")
            << QString::fromLocal8Bit("ФИО покупателя") << QString::fromLocal8Bit("Цена")
            << QString::fromLocal8Bit("Дата покупки");
    table->setHorizontalHeaderLabels(headers);

    for(int i = 0; query.next(); i++)
    {
        table->insertRow(i);
        table->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
        table->setItem(i, 1, new QTableWidgetItem(query.value(1).toString()));
        table->setItem(i, 2, new QTableWidgetItem(query.value(2).toString()));
        table->setItem(i, 3, new QTableWidgetItem(query.value(3).toString()));
        table->setItem(i, 4, new QTableWidgetItem(query.value(4).toString()));
        table->setItem(i, 5, new QTableWidgetItem(query.value(5).toString()));
        row++;
    }

    updateSettingTable();

    nameDatabaseIsShow = "purchase";
}

void Widget::cartsShow()
{
    name->setText(QString::fromLocal8Bit("Карты"));
    clearTable();
    QSqlQuery query = QSqlQuery(database);
    query.prepare("SELECT * FROM carts");
    query.exec();

    int row = 0;

    table->setColumnCount(2); // Указываем число колонок

    QStringList headers;
    headers << QString::fromLocal8Bit("Номер") << QString::fromLocal8Bit("Скидка");
    table->setHorizontalHeaderLabels(headers);

    for(int i = 0; query.next(); i++)
    {
        table->insertRow(i);
        table->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
        table->setItem(i, 1, new QTableWidgetItem(query.value(1).toString()));
        row++;
    }

    updateSettingTable();

    nameDatabaseIsShow = "carts";
}

void Widget::buyersShow()
{
    name->setText(QString::fromLocal8Bit("Покупатели"));
    clearTable();
    QSqlQuery query = QSqlQuery(database);
    query.prepare("SELECT * FROM buyers");
    query.exec();

    int row = 0;

    table->setColumnCount(4); // Указываем число колонок

    QStringList headers;
    headers << QString::fromLocal8Bit("Номер") << QString::fromLocal8Bit("ФИО")
            << QString::fromLocal8Bit("День Рождения") << QString::fromLocal8Bit("Номер карты");
    table->setHorizontalHeaderLabels(headers);

    for(int i = 0; query.next(); i++)
    {
        table->insertRow(i);
        table->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
        table->setItem(i, 1, new QTableWidgetItem(query.value(1).toString()));
        table->setItem(i, 2, new QTableWidgetItem(query.value(2).toString()));
        table->setItem(i, 3, new QTableWidgetItem(query.value(3).toString()));
        row++;
    }

    updateSettingTable();

    nameDatabaseIsShow = "buyers";
}

void Widget::notPurchasedRequest()
{
    name->setText(QString::fromLocal8Bit("Не покупаемые книги"));
    clearTable();

    QSqlQuery query = QSqlQuery(database);
    query.prepare("SELECT publishers.name, books.name, "
                  "books.author, books.genre, books.price "
                  "FROM books, publishers "
                  "WHERE (books.id_book NOT IN (SELECT id_book FROM purchase)) "
                  "AND (books.id_publisher = publishers.id_publisher)"
                  "AND (books.id_book NOT IN (SELECT id_book FROM orders))");
    query.exec();

    int row = 0;

    table->setColumnCount(5); // Указываем число колонок

    QStringList headers;
    headers << QString::fromLocal8Bit("Название") << QString::fromLocal8Bit("Издатель")
            << QString::fromLocal8Bit("Автор") << QString::fromLocal8Bit("Жанр")
            << QString::fromLocal8Bit("Цена");
    table->setHorizontalHeaderLabels(headers);

    for(int i = 0; query.next(); i++)
    {
        table->insertRow(i);
        table->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
        table->setItem(i, 1, new QTableWidgetItem(query.value(1).toString()));
        table->setItem(i, 2, new QTableWidgetItem(query.value(2).toString()));
        table->setItem(i, 3, new QTableWidgetItem(query.value(3).toString()));
        table->setItem(i, 4, new QTableWidgetItem(query.value(4).toString()));
        row++;
    }

    updateSettingTable();

    addButton->hide();
    nameDatabaseIsShow = "notPurchasedRequest";
}

void Widget::numberOfPurchasedRequest(int count, int sign)
{
    signL = sign;
    countL = count;

    name->setText(QString::fromLocal8Bit("По количеству купленных"));
    clearTable();

    QSqlQuery query = QSqlQuery(database);
    switch(sign)
    {
    case 1:
        query.prepare("SELECT publishers.name, books.name, books.author,"
                      " books.genre, books.price "
                      "FROM books, publishers "
                      "WHERE (SELECT COUNT(*) FROM purchase WHERE books.id_book = purchase.id_book) > :count "
                      "AND books.id_publisher = publishers.id_publisher");
        break;
    case 2:
        query.prepare("SELECT publishers.name, books.name, books.author, "
                      "books.genre, books.price "
                      "FROM books, publishers "
                      "WHERE (SELECT COUNT(*) FROM purchase WHERE books.id_book = purchase.id_book) = :count "
                      "AND books.id_publisher = publishers.id_publisher");
        break;
    case 3:
        query.prepare("SELECT publishers.name, books.name, books.author, books.genre, "
                      "books.price "
                      "FROM books, publishers "
                      "WHERE (SELECT COUNT(*) FROM purchase WHERE books.id_book = purchase.id_book) < :count "
                      "AND books.id_publisher = publishers.id_publisher");
        break;
    }

    query.bindValue(":count", count);
    query.exec();

    int row = 0;

    table->setColumnCount(5); // Указываем число колонок

    QStringList headers;
    headers << QString::fromLocal8Bit("Название") << QString::fromLocal8Bit("Издатель")
            << QString::fromLocal8Bit("Автор") << QString::fromLocal8Bit("Жанр")
            << QString::fromLocal8Bit("Цена");
    table->setHorizontalHeaderLabels(headers);

    for(int i = 0; query.next(); i++)
    {
        table->insertRow(i);
        table->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
        table->setItem(i, 1, new QTableWidgetItem(query.value(1).toString()));
        table->setItem(i, 2, new QTableWidgetItem(query.value(2).toString()));
        table->setItem(i, 3, new QTableWidgetItem(query.value(3).toString()));
        table->setItem(i, 4, new QTableWidgetItem(query.value(4).toString()));
        row++;
    }

    updateSettingTable();

    addButton->hide();
    nameDatabaseIsShow = "numberOfPurchasedRequest";
}

void Widget::mostOrderedRequest()
{
    name->setText(QString::fromLocal8Bit("Самые заказываемые книги"));
    clearTable();

    QSqlQuery query = QSqlQuery(database);
    query.prepare("SELECT books.id_book, publishers.name, books.name, "
                  "books.author, books.genre, books.price "
                  "FROM books, publishers "
                  "WHERE books.id_book IN (SELECT id_book FROM orders) "
                  "AND books.id_publisher = publishers.id_publisher "
                  "ORDER BY (SELECT COUNT(*) FROM orders WHERE books.id_book = orders.id_book) DESC");
    query.exec();

    int row = 0;

    table->setColumnCount(5); // Указываем число колонок

    QStringList headers;
    headers << QString::fromLocal8Bit("Кол-во раз") << QString::fromLocal8Bit("Название")
            << QString::fromLocal8Bit("Издатель")
            << QString::fromLocal8Bit("Автор") << QString::fromLocal8Bit("Жанр")
            << QString::fromLocal8Bit("Цена");
    table->setHorizontalHeaderLabels(headers);

    for(int i = 0; query.next(); i++)
    {
        table->insertRow(i);
        table->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
        table->setItem(i, 1, new QTableWidgetItem(query.value(1).toString()));
        table->setItem(i, 2, new QTableWidgetItem(query.value(2).toString()));
        table->setItem(i, 3, new QTableWidgetItem(query.value(3).toString()));
        table->setItem(i, 4, new QTableWidgetItem(query.value(4).toString()));
        table->setItem(i, 5, new QTableWidgetItem(query.value(5).toString()));
        row++;
    }

    for(int i = 0; i < row; i++)
    {
        query.prepare("SELECT COUNT(*) FROM orders WHERE :id = orders.id_book");
        query.bindValue(":id", table->item(i, 0)->text());
        query.exec();

        if(query.first())
        {
            table->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
        }
    }

    updateSettingTable();

    addButton->hide();
    nameDatabaseIsShow = "mostOrderedRequest";
}

void Widget::notPurchasedReports()
{
    PrintReports* p = new PrintReports("notPurchased", &database);
    p->show();
}

void Widget::numberOfPurchasedReports(int count, int sign)
{
    PrintReports* p = new PrintReports("numberOfPurchased", &database, count, sign);
    p->show();
}

void Widget::mostOrderedReports()
{
    PrintReports* p = new PrintReports("mostOrdered", &database);
    p->show();
}

void Widget::booksReports()
{
    PrintReports* p = new PrintReports("books", &database);
    p->show();
}

void Widget::addEntry()
{
    AddEntry* entry = new AddEntry(nameDatabaseIsShow, &database);
    entry->setAttribute(Qt::WA_ShowModal, true);
    entry->show();
    connect(entry, SIGNAL(destroyed(QObject*)), this, SLOT(updateTable()));
}

void Widget::clickedEntry(QModelIndex index)
{
    QString value;
    for (int i = 0; i < table->columnCount(); ++i)
    {
         value += table->item(index.row(), i)->text();
         value += "~";
    }
    qDebug() << value;

    EditEntry* entry = new EditEntry(nameDatabaseIsShow, value, &database);
    entry->setAttribute(Qt::WA_ShowModal, true);
    entry->show();
    connect(entry, SIGNAL(destroyed(QObject*)), this, SLOT(updateTable()));
}

void Widget::updateTable()
{
    if(nameDatabaseIsShow == "books")
    {
        booksShow();
    }
    else if(nameDatabaseIsShow == "orders")
    {
        ordersShow();
    }
    else if(nameDatabaseIsShow == "publishers")
    {
        publishersShow();
    }
    else if(nameDatabaseIsShow == "purchase")
    {
        purchaseShow();
    }
    else if(nameDatabaseIsShow == "buyers")
    {
        buyersShow();
    }
    else if(nameDatabaseIsShow == "carts")
    {
        cartsShow();
    }
}

void Widget::searchEntry()
{
    if(search->text() == "")
    {
        search->clear();
        updateTable();
    }
    if(nameDatabaseIsShow == "books")
    {
        clearTable();
        QSqlQuery query = QSqlQuery(database);
        query.prepare("SELECT books.id_book, books.name, publishers.name, "
                      "books.author, books.genre, books.price "
                      "FROM books, publishers "
                      "WHERE books.id_book LIKE :id_book "
                      "OR books.name LIKE :name "
                      "OR books.author LIKE :author "
                      "OR books.genre LIKE :genre "
                      "OR books.price LIKE :price "
                      "OR publishers.name LIKE :nameP "
                      "AND books.id_publisher = publishers.id_publisher");
        query.bindValue(":id_book", "%" + search->text() + "%");
        query.bindValue(":name", "%" + search->text() + "%");
        query.bindValue(":author", "%" + search->text() + "%");
        query.bindValue(":genre", "%" + search->text() + "%");
        query.bindValue(":price", "%" + search->text() + "%");
        query.bindValue(":nameP", "%" + search->text() + "%");
        query.exec();

        int row = 0;

        table->setColumnCount(6); // Указываем число колонок

        QStringList headers;
        headers << QString::fromLocal8Bit("Номер") << QString::fromLocal8Bit("Название")
                << QString::fromLocal8Bit("Издатель") << QString::fromLocal8Bit("Автор")
                << QString::fromLocal8Bit("Жанр") << QString::fromLocal8Bit("Цена");

        table->setHorizontalHeaderLabels(headers);

        for(int i = 0; query.next(); i++)
        {
            table->insertRow(i);
            table->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
            table->setItem(i, 1, new QTableWidgetItem(query.value(1).toString()));
            table->setItem(i, 2, new QTableWidgetItem(query.value(2).toString()));
            table->setItem(i, 3, new QTableWidgetItem(query.value(3).toString()));
            table->setItem(i, 4, new QTableWidgetItem(query.value(4).toString()));
            table->setItem(i, 5, new QTableWidgetItem(query.value(5).toString()));
            row++;
        }

        updateSettingTable();
    }
    else if(nameDatabaseIsShow == "orders")
    {
        clearTable();
        QSqlQuery query = QSqlQuery(database);
        query.prepare("SELECT orders.id_order, books.name, publishers.name, "
                      "buyers.FIO, orders.order_amount, orders.order_date "
                      "FROM orders, buyers, books, publishers "
                      "WHERE (orders.id_order LIKE :id_order "
                      "OR orders.order_amount LIKE :order_amount "
                      "OR orders.order_date LIKE :order_date "
                      "OR books.name LIKE :nameB "
                      "OR publishers.name LIKE :nameP "
                      "OR buyers.FIO LIKE :FIO) "
                      "AND (orders.id_book = books.id_book "
                      "AND books.id_publisher = publishers.id_publisher "
                      "AND orders.id_buyer = buyers.id_buyer)");
        query.bindValue(":id_order", "%" + search->text() + "%");
        query.bindValue(":order_amount", "%" + search->text() + "%");
        query.bindValue(":order_date", "%" + search->text() + "%");
        query.bindValue(":nameB", "%" + search->text() + "%");
        query.bindValue(":nameP", "%" + search->text() + "%");
        query.bindValue(":FIO", "%" + search->text() + "%");
        query.exec();

        int row = 0;

        table->setColumnCount(6); // Указываем число колонок

        QStringList headers;
        headers << QString::fromLocal8Bit("Номер") << QString::fromLocal8Bit("Название книги")
                << QString::fromLocal8Bit("Издатель") << QString::fromLocal8Bit("ФИО покупателя")
                << QString::fromLocal8Bit("Цена") << QString::fromLocal8Bit("Дата покупки");
        table->setHorizontalHeaderLabels(headers);

        for(int i = 0; query.next(); i++)
        {
            table->insertRow(i);
            table->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
            table->setItem(i, 1, new QTableWidgetItem(query.value(1).toString()));
            table->setItem(i, 2, new QTableWidgetItem(query.value(2).toString()));
            table->setItem(i, 3, new QTableWidgetItem(query.value(3).toString()));
            table->setItem(i, 4, new QTableWidgetItem(query.value(4).toString()));
            table->setItem(i, 5, new QTableWidgetItem(query.value(5).toString()));
            row++;
        }

        updateSettingTable();
    }
    else if(nameDatabaseIsShow == "carts")
    {
        clearTable();
        QSqlQuery query = QSqlQuery(database);
        query.prepare("SELECT * FROM carts WHERE id_cart LIKE :id_cart OR discount LIKE :discount");
        query.bindValue(":id_book", "%" + search->text() + "%");
        query.bindValue(":discount", "%" + search->text() + "%");
        query.exec();

        int row = 0;

        table->setColumnCount(2); // Указываем число колонок

        for(int i = 0; query.next(); i++)
        {
            table->insertRow(i);
            table->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
            table->setItem(i, 1, new QTableWidgetItem(query.value(1).toString()));
            row++;
        }

        updateSettingTable();
    }
    else if(nameDatabaseIsShow == "publishers")
    {
        clearTable();
        QSqlQuery query = QSqlQuery(database);
        query.prepare("SELECT * FROM publishers WHERE id_publishers LIKE :id_publishers OR name LIKE :name");
        query.bindValue(":id_publishers", "%" + search->text() + "%");
        query.bindValue(":name", "%" + search->text() + "%");
        query.exec();

        int row = 0;

        table->setColumnCount(2); // Указываем число колонок

        for(int i = 0; query.next(); i++)
        {
            table->insertRow(i);
            table->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
            table->setItem(i, 1, new QTableWidgetItem(query.value(1).toString()));
            row++;
        }

        updateSettingTable();
    }
    else if(nameDatabaseIsShow == "purchase")
    {
        clearTable();
        QSqlQuery query = QSqlQuery(database);
        query.prepare("SELECT purchase.id_purchase, books.name, publishers.name, "
                      "buyers.FIO, purchase.purchase_amount, purchase.purchase_date "
                      "FROM purchase, books, publishers, buyers "
                      "WHERE (purchase.id_purchase LIKE :id_purchase "
                      "OR purchase.purchase_amount LIKE :purchase_amount "
                      "OR purchase.purchase_date LIKE :purchase_date "
                      "OR books.name LIKE :nameB OR publishers.name LIKE :nameP "
                      "OR buyers.FIO LIKE :FIO) AND (purchase.id_book = books.id_book "
                      "AND purchase.id_buyer = buyers.id_buyer AND books.id_publisher = publishers.id_publisher)");
        query.bindValue(":id_purchase", "%" + search->text() + "%");
        query.bindValue(":purchase_amount", "%" + search->text() + "%");
        query.bindValue(":purchase_date", "%" + search->text() + "%");
        query.bindValue(":nameB", "%" + search->text() + "%");
        query.bindValue(":nameP", "%" + search->text() + "%");
        query.bindValue(":FIO", "%" + search->text() + "%");
        query.exec();

        int row = 0;

        table->setColumnCount(6); // Указываем число колонок

        QStringList headers;
        headers << QString::fromLocal8Bit("Номер") << QString::fromLocal8Bit("Название книги")
                << QString::fromLocal8Bit("Издатель")
                << QString::fromLocal8Bit("ФИО покупателя") << QString::fromLocal8Bit("Цена")
                << QString::fromLocal8Bit("Дата покупки");
        table->setHorizontalHeaderLabels(headers);

        for(int i = 0; query.next(); i++)
        {
            table->insertRow(i);
            table->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
            table->setItem(i, 1, new QTableWidgetItem(query.value(1).toString()));
            table->setItem(i, 2, new QTableWidgetItem(query.value(2).toString()));
            table->setItem(i, 3, new QTableWidgetItem(query.value(3).toString()));
            table->setItem(i, 4, new QTableWidgetItem(query.value(4).toString()));
            table->setItem(i, 5, new QTableWidgetItem(query.value(5).toString()));
            row++;
        }

        query.prepare("SELECT * FROM books");
        query.exec();

        for(int i = 0; i < table->rowCount(); i++)
        {
            query.prepare("SELECT * FROM books");
            query.exec();
            while(query.next())
            {
                if(table->item(i, 1)->text() == query.value("id_book").toString())
                {
                    table->item(i, 1)->setText(query.value("name").toString());
                }
            }
        }

        for(int i = 0; i < table->rowCount(); i++)
        {
            query.prepare("SELECT * FROM buyers");
            query.exec();
            while(query.next())
            {
                if(table->item(i, 2)->text() == query.value("id_buyer").toString())
                {
                    table->item(i, 2)->setText(query.value("FIO").toString());
                }
            }
        }

        updateSettingTable();
    }
    else if(nameDatabaseIsShow == "buyers")
    {
        clearTable();
        QSqlQuery query = QSqlQuery(database);
        query.prepare("SELECT * FROM buyers "
                      "WHERE id_buyer LIKE :id_buyer "
                      "OR FIO LIKE :FIO OR birthdate LIKE :birthdate "
                      "OR id_cart LIKE :id_cart");
        query.bindValue(":id_buyer", "%" + search->text() + "%");
        query.bindValue(":FIO", "%" + search->text() + "%");
        query.bindValue(":birthdate", "%" + search->text() + "%");
        query.bindValue(":id_cart", "%" + search->text() + "%");
        query.exec();

        int row = 0;

        table->setColumnCount(4); // Указываем число колонок

        for(int i = 0; query.next(); i++)
        {
            table->insertRow(i);
            table->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
            table->setItem(i, 1, new QTableWidgetItem(query.value(1).toString()));
            table->setItem(i, 2, new QTableWidgetItem(query.value(2).toString()));
            table->setItem(i, 3, new QTableWidgetItem(query.value(3).toString()));
            row++;
        }

        updateSettingTable();
    }
    else if(nameDatabaseIsShow == "notPurchasedRequest")
    {
        clearTable();

        QSqlQuery query = QSqlQuery(database);
        query.prepare("SELECT books.name, publishers.name,"
                      "books.author, books.genre, books.price "
                      "FROM books, publishers "
                      "WHERE ((publishers.name LIKE :nameP) "
                      "OR (books.name LIKE :nameB)"
                      "OR (books.author LIKE :author)"
                      "OR (books.genre LIKE :genre)"
                      "OR (books.price LIKE :price))"
                      "AND ((books.id_book NOT IN (SELECT id_book FROM purchase)) "
                      "AND (books.id_publisher = publishers.id_publisher)"
                      "AND (books.id_book NOT IN (SELECT id_book FROM orders))");
        query.bindValue(":nameP", "%" + search->text() + "%");
        query.bindValue(":nameB", "%" + search->text() + "%");
        query.bindValue(":author", "%" + search->text() + "%");
        query.bindValue(":genre", "%" + search->text() + "%");
        query.bindValue(":price", "%" + search->text() + "%");
        query.exec();

        table->setColumnCount(5); // Указываем число колонок

        QStringList headers;
        headers << QString::fromLocal8Bit("Название") << QString::fromLocal8Bit("Издатель")
                << QString::fromLocal8Bit("Автор") << QString::fromLocal8Bit("Жанр")
                << QString::fromLocal8Bit("Цена");
        table->setHorizontalHeaderLabels(headers);

        for(int i = 0; query.next(); i++)
        {
            table->insertRow(i);
            table->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
            table->setItem(i, 1, new QTableWidgetItem(query.value(1).toString()));
            table->setItem(i, 2, new QTableWidgetItem(query.value(2).toString()));
            table->setItem(i, 3, new QTableWidgetItem(query.value(3).toString()));
            table->setItem(i, 4, new QTableWidgetItem(query.value(4).toString()));
        }

        updateSettingTable();

        addButton->hide();
    }
    else if(nameDatabaseIsShow == "numberOfPurchasedRequest")
    {
        name->setText(QString::fromLocal8Bit("По количеству купленных"));
        clearTable();

        QSqlQuery query = QSqlQuery(database);
        switch(signL)
        {
        case 1:
            query.prepare("SELECT publishers.name, books.name, books.author,"
                          " books.genre, books.price "
                          "FROM books, publishers "
                          "WHERE (((SELECT COUNT(*) FROM purchase WHERE books.id_book = purchase.id_book) > :count) "
                          "AND (books.id_publisher = publishers.id_publisher)) "
                          "AND ((publishers.name LIKE :nameP)"
                          "OR (books.name LIKE :nameB)"
                          "OR (books.author LIKE :author)"
                          "OR (books.genre LIKE :genre)"
                          "OR (books.price LIKE :price))");
            break;
        case 2:
            query.prepare("SELECT publishers.name, books.name, books.author, "
                          "books.genre, books.price "
                          "FROM books, publishers "
                          "WHERE (((SELECT COUNT(*) FROM purchase WHERE books.id_book = purchase.id_book) = :count) "
                          "AND (books.id_publisher = publishers.id_publisher)) "
                          "AND ((publishers.name LIKE :nameP)"
                          "OR (books.name LIKE :nameB)"
                          "OR (books.author LIKE :author)"
                          "OR (books.genre LIKE :genre)"
                          "OR (books.price LIKE :price))");
            break;
        case 3:
            query.prepare("SELECT publishers.name, books.name, books.author, books.genre, "
                          "books.price "
                          "FROM books, publishers "
                          "WHERE ((publishers.name LIKE :nameP)"
                          "OR (books.name LIKE :nameB)"
                          "OR (books.author LIKE :author)"
                          "OR (books.genre LIKE :genre)"
                          "OR (books.price LIKE :price)) "
                          "AND (((SELECT COUNT(*) FROM purchase WHERE books.id_book = purchase.id_book) < :count) "
                          "AND (books.id_publisher = publishers.id_publisher))");
            break;
        }

        query.bindValue(":nameP", "%" + search->text() + "%");
        query.bindValue(":nameB", "%" + search->text() + "%");
        query.bindValue(":author", "%" + search->text() + "%");
        query.bindValue(":genre", "%" + search->text() + "%");
        query.bindValue(":price", "%" + search->text() + "%");
        query.bindValue(":count", countL);
        query.exec();

        table->setColumnCount(5); // Указываем число колонок

        QStringList headers;
        headers << QString::fromLocal8Bit("Название") << QString::fromLocal8Bit("Издатель")
                << QString::fromLocal8Bit("Автор") << QString::fromLocal8Bit("Жанр")
                << QString::fromLocal8Bit("Цена");
        table->setHorizontalHeaderLabels(headers);

        for(int i = 0; query.next(); i++)
        {
            table->insertRow(i);
            table->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
            table->setItem(i, 1, new QTableWidgetItem(query.value(1).toString()));
            table->setItem(i, 2, new QTableWidgetItem(query.value(2).toString()));
            table->setItem(i, 3, new QTableWidgetItem(query.value(3).toString()));
            table->setItem(i, 4, new QTableWidgetItem(query.value(4).toString()));
        }

        updateSettingTable();

        addButton->hide();
    }
    else if(nameDatabaseIsShow == "mostOrderedRequest")
    {
        name->setText(QString::fromLocal8Bit("Самые заказываемые книги"));
        clearTable();

        QSqlQuery query = QSqlQuery(database);
        query.prepare("SELECT books.id_book, publishers.name, books.name, "
                      "books.author, books.genre, books.price "
                      "FROM books, publishers "
                      "WHERE ((books.id_book IN (SELECT id_book FROM orders)) "
                      "AND (books.id_publisher = publishers.id_publisher))"
                      "AND ((publishers.name LIKE :nameP)"
                      "OR (books.name LIKE :nameB)"
                      "OR (books.author LIKE :author)"
                      "OR (books.genre LIKE :genre)"
                      "OR (books.price LIKE :price)) "
                      "ORDER BY (SELECT COUNT(*) FROM orders WHERE books.id_book = orders.id_book) DESC");
        query.bindValue(":nameP", "%" + search->text() + "%");
        query.bindValue(":nameB", "%" + search->text() + "%");
        query.bindValue(":author", "%" + search->text() + "%");
        query.bindValue(":genre", "%" + search->text() + "%");
        query.bindValue(":price", "%" + search->text() + "%");
        query.exec();

        int row = 0;

        table->setColumnCount(5); // Указываем число колонок

        QStringList headers;
        headers << QString::fromLocal8Bit("Кол-во раз") << QString::fromLocal8Bit("Название")
                << QString::fromLocal8Bit("Издатель")
                << QString::fromLocal8Bit("Автор") << QString::fromLocal8Bit("Жанр")
                << QString::fromLocal8Bit("Цена");
        table->setHorizontalHeaderLabels(headers);

        for(int i = 0; query.next(); i++)
        {
            table->insertRow(i);
            table->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
            table->setItem(i, 1, new QTableWidgetItem(query.value(1).toString()));
            table->setItem(i, 2, new QTableWidgetItem(query.value(2).toString()));
            table->setItem(i, 3, new QTableWidgetItem(query.value(3).toString()));
            table->setItem(i, 4, new QTableWidgetItem(query.value(4).toString()));
            table->setItem(i, 5, new QTableWidgetItem(query.value(5).toString()));
            row++;
        }

        for(int i = 0; i < row; i++)
        {
            query.prepare("SELECT COUNT(*) FROM orders WHERE :id = orders.id_book");
            query.bindValue(":id", table->item(i, 0)->text());
            query.exec();

            if(query.first())
            {
                table->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
            }
        }

        updateSettingTable();

        addButton->hide();
    }
}
