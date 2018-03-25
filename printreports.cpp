#include "printreports.h"
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QPushButton>
#include <qDebug>
#include <QSqlQuery>
#include <QTextDocument>
#include <QFile>
#include <QInputDialog>
#include <QPrintDialog>
#include "countentry.h"

PrintReports::PrintReports(QString nameDb, QSqlDatabase *db, int co, int si, QWidget *parent)
    : QWidget(parent), nameDB(nameDb), database(db), count(co), sign(si)
{
    mainBox = new QVBoxLayout(this);
    mainBox->setSpacing(15);

    QPushButton* backButton = new QPushButton;
    backButton->setText(QString::fromLocal8Bit("Назад"));
    backButton->setFixedHeight(35);
    connect(backButton, SIGNAL(clicked(bool)), this, SLOT(clickedBack()));

    QPushButton* saveButton = new QPushButton;
    saveButton->setText(QString::fromLocal8Bit("Сохранить таблицу в файл"));
    saveButton->setFixedHeight(35);
    connect(saveButton, SIGNAL(clicked(bool)), this, SLOT(clickedSave()));

    QPushButton* printAllButton = new QPushButton;
    printAllButton->setText(QString::fromLocal8Bit("Распечатать таблицу"));
    printAllButton->setFixedHeight(35);
    connect(printAllButton, SIGNAL(clicked(bool)), this, SLOT(clickedPrint()));

    mainBox->addWidget(backButton);
    mainBox->addWidget(saveButton);
    mainBox->addWidget(printAllButton);

    out.setString(&s, QIODevice::ReadWrite);
}

void PrintReports::print()
{
    QPrinter * printer = new QPrinter();

    QPrintDialog printDialog(printer);
    if (printDialog.exec() != QDialog::Accepted) {

    }

    QTextDocument *doc = new QTextDocument();
    doc->setHtml(s);
    doc->print(printer);

//    QPrinter * printer = new QPrinter();
//    QPrintPreviewDialog *printPreviewDialog = new QPrintPreviewDialog(printer);
//    connect(printPreviewDialog, QOverload<QPrinter*>::of(&QPrintPreviewDialog::paintRequested), [this](QPrinter* p){
//        QTextDocument doc;
//        doc.setHtml(s);
//        doc.print(p);
//    });
//    printPreviewDialog->exec();
//    delete printPreviewDialog;
    deleteLater();
}

void PrintReports::createTable()
{
    QString test;

    if(nameDB == "books")
    {
        QSqlQuery query = QSqlQuery(*database);
        query.prepare("SELECT books.id_book, publishers.name, books.name, "
                      "books.author, books.genre, books.price "
                      "FROM books, publishers "
                      "WHERE books.id_publisher = publishers.id_publisher");
        query.exec();

        out << "<h2 align='center' style='color:Black'>" + QString::fromLocal8Bit("Книги") + "</h2>";
        out << "<table width=\"100%\" border=1 cellspacing=0>\n ";

        test = "<tr>";

        test += "<th>" + QString::fromLocal8Bit("Номер") + "</th>";
        test += "<th>" + QString::fromLocal8Bit("Издатель") + "</th>";
        test += "<th>" + QString::fromLocal8Bit("Название") + "</th>";
        test += "<th>" + QString::fromLocal8Bit("Автор") + "</th>";
        test += "<th>" + QString::fromLocal8Bit("Жанр") + "</th>";
        test += "<th>" + QString::fromLocal8Bit("Цена") + "</th>";

        test += "</tr>";

        out << test;

        while(query.next())
        {
            test = "<tr>";

            test += "<td>" + query.value(0).toString() + "</td>";
            test += "<td>" + query.value(1).toString() + "</td>";
            test += "<td>" + query.value(2).toString() + "</td>";
            test += "<td>" + query.value(3).toString() + "</td>";
            test += "<td>" + query.value(4).toString() + "</td>";
            test += "<td>" + query.value(5).toString() + "</td>";

            test += "</tr>";

            out << test;
        }
    }
    else if(nameDB == "notPurchased")
    {
        QSqlQuery query = QSqlQuery(*database);
        query.prepare("SELECT publishers.name, books.name, books.author, "
                      "books.genre, books.price "
                      "FROM books, publishers "
                      "WHERE (books.id_book NOT IN (SELECT id_book FROM purchase)) "
                      "AND (books.id_publisher = publishers.id_publisher)"
                      "AND (books.id_book NOT IN (SELECT id_book FROM orders))");
        query.exec();

        out << "<h2 align='center' style='color:Black'>" + QString::fromLocal8Bit("Не покупаемые книги") + "</h2>";
        out << "<table width=\"100%\" border=1 cellspacing=0> \n";

        test = "<tr>";

        test += "<th>" + QString::fromLocal8Bit("Издатель") + "</th>";
        test += "<th>" + QString::fromLocal8Bit("Название") + "</th>";
        test += "<th>" + QString::fromLocal8Bit("Автор") + "</th>";
        test += "<th>" + QString::fromLocal8Bit("Жанр") + "</th>";
        test += "<th>" + QString::fromLocal8Bit("Цена") + "</th>";

        test += "</tr>";

        out << test;

        for(int i = 0; query.next(); i++)
        {
            test = "<tr>";

            test += "<td>" + query.value(0).toString() + "</td>";
            test += "<td>" + query.value(1).toString() + "</td>";
            test += "<td>" + query.value(2).toString() + "</td>";
            test += "<td>" + query.value(3).toString() + "</td>";
            test += "<td>" + query.value(4).toString() + "</td>";

            test += "</tr>";

            out << test;
        }
    }
    else if(nameDB == "numberOfPurchased")
    {
        QSqlQuery query = QSqlQuery(*database);
        switch(sign)
        {
        case 1:
            query.prepare("SELECT publishers.name, books.name, books.author, "
                          "books.genre, books.price "
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
            query.prepare("SELECT publishers.name, books.name, books.author, "
                          "books.genre, books.price "
                          "FROM books, publishers "
                          "WHERE (SELECT COUNT(*) FROM purchase WHERE books.id_book = purchase.id_book) < :count "
                          "AND books.id_publisher = publishers.id_publisher");
            break;
        }

        query.bindValue(":count", count);
        query.exec();

        out << "<h2 align='center' style='color:Black'>" + QString::fromLocal8Bit("По количеству купленных книг")
               + "</h2>";
        out << "<table width=\"100%\" border=1 cellspacing=0>\n ";

        test = "<tr>";

        test += "<th>" + QString::fromLocal8Bit("Издатель") + "</th>";
        test += "<th>" + QString::fromLocal8Bit("Название") + "</th>";
        test += "<th>" + QString::fromLocal8Bit("Автор") + "</th>";
        test += "<th>" + QString::fromLocal8Bit("Жанр") + "</th>";
        test += "<th>" + QString::fromLocal8Bit("Цена") + "</th>";

        test += "</tr>";

        out << test;

        for(int i = 0; query.next(); i++)
        {
            test = "<tr>";

            test += "<td>" + query.value(0).toString() + "</td>";
            test += "<td>" + query.value(1).toString() + "</td>";
            test += "<td>" + query.value(2).toString() + "</td>";
            test += "<td>" + query.value(3).toString() + "</td>";
            test += "<td>" + query.value(4).toString() + "</td>";

            test += "</tr>";

            out << test;
        }
    }
    else if(nameDB == "mostOrdered")
    {
        QSqlQuery query = QSqlQuery(*database);
         query.prepare("SELECT books.id_book, publishers.name, books.name, "
                       "books.author, books.genre, books.price "
                       "FROM books, publishers "
                       "WHERE books.id_book IN (SELECT id_book FROM orders) "
                       "AND books.id_publisher = publishers.id_publisher "
                       "ORDER BY (SELECT COUNT(*) FROM orders WHERE books.id_book = orders.id_book) DESC");
        query.exec();

        out << "<h2 align='center' style='color:Black'>" + QString::fromLocal8Bit("Самые заказываемые книги")
               + "</h2>";
        out << "<table width=\"100%\" border=1 cellspacing=0> ";

        test = "<tr>";

        test += "<th>" + QString::fromLocal8Bit("Кол-во раз") + "</th>";
        test += "<th>" + QString::fromLocal8Bit("Издатель") + "</th>";
        test += "<th>" + QString::fromLocal8Bit("Название") + "</th>";
        test += "<th>" + QString::fromLocal8Bit("Автор") + "</th>";
        test += "<th>" + QString::fromLocal8Bit("Жанр") + "</th>";
        test += "<th>" + QString::fromLocal8Bit("Цена") + "</th>";

        test += "</tr>";

        out << test;

        for(int i = 0; i < query.next(); i++)
        {
            QSqlQuery query1 = QSqlQuery(*database);
            query1.prepare("SELECT COUNT(*) FROM orders WHERE :id = orders.id_book");
            query1.bindValue(":id", query.value(0).toString());
            query1.exec();

            if(query1.first())
            {
                test = "<tr>";

                test += "<td>" + query1.value(0).toString() + "</td>";
                test += "<td>" + query.value(1).toString() + "</td>";
                test += "<td>" + query.value(2).toString() + "</td>";
                test += "<td>" + query.value(3).toString() + "</td>";
                test += "<td>" + query.value(4).toString() + "</td>";
                test += "<td>" + query.value(5).toString() + "</td>";

                test += "</tr>";

                out << test;
            }
        }
    }

    out << "</table>";
}

void PrintReports::saveFile(QString name)
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml(s);

    QFile destinationFile;

    destinationFile.setFileName(name + ".rtf");
    destinationFile.open( QIODevice::WriteOnly | QIODevice::Truncate);

    destinationFile.write(doc->toHtml().toLocal8Bit());
    destinationFile.close();

    deleteLater();
}

void PrintReports::clickedSave()
{
    createTable();
    QString text = QInputDialog::getText(this,QString::fromLocal8Bit("Введите имя файла"),
                                         QString::fromLocal8Bit("Имя файла:"));
    saveFile(text);
}

void PrintReports::clickedPrint()
{
    createTable();
    print();
}

void PrintReports::clickedBack()
{
    CountEntry* c = new CountEntry(this, "NumberOfPurchasedReports");
    c->show();
    deleteLater();
}
