#include "mainmenu.h"
#include <QPushButton>
#include <QLabel>
#include "widget.h"
#include "countentry.h"
#include <QMenuBar>
#include <QMessageBox>

MainMenu::MainMenu(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(600, 560);
    //this->setStyleSheet("background: #afdafc;");

    QMenuBar* mnuBar = new QMenuBar(this);
    QMenu*   pmnu   = new QMenu(QString::fromLocal8Bit("Меню"));

    pmnu->addAction(QString::fromLocal8Bit("Справка"),
                    this,
                    SLOT(help()));

    mnuBar->addMenu(pmnu);

    mnuBar->show();

    mainBox = new QVBoxLayout;
    mainBox->setSpacing(15);

    this->setLayout(mainBox);

    QLabel* name = new QLabel(QString::fromLocal8Bit("Книжный магазин"));
    name->setMaximumHeight(30);
    name->setAlignment(Qt::AlignCenter);
    QFont font = name->font();
    font.setPointSize(20);
    font.setBold(true);
    font.setItalic(true);
    name->setFont(font);

    QHBoxLayout* upHBox = new QHBoxLayout;

    QLabel* image = new QLabel;
    image->setStyleSheet("image: url(books.png);");
    image->setFixedSize(290, 200);

    upHBox->addWidget(name, Qt::AlignTop);
    upHBox->addWidget(image, Qt::AlignTop);

    mainBox->addLayout(upHBox);

    QLabel* tables = new QLabel(QString::fromLocal8Bit("Таблицы"));
    font = tables->font();
    font.setPointSize(14);
    font.setItalic(true);
    tables->setFont(font);
    tables->setAlignment(Qt::AlignCenter);
    QLabel* requests = new QLabel(QString::fromLocal8Bit("Запросы"));
    font = requests->font();
    font.setPointSize(14);
    font.setItalic(true);
    requests->setFont(font);
    requests->setAlignment(Qt::AlignCenter);
    QLabel* reports = new QLabel(QString::fromLocal8Bit("Отчёты"));
    font = reports->font();
    font.setPointSize(14);
    font.setItalic(true);
    reports->setFont(font);
    reports->setAlignment(Qt::AlignCenter);

    QPushButton* booksTable = new QPushButton(QString::fromLocal8Bit("Книги"));
    booksTable->setFixedHeight(35);
    connect(booksTable, SIGNAL(clicked(bool)), this, SLOT(clickedBooksTable()));
    QPushButton* ordersTable = new QPushButton(QString::fromLocal8Bit("Заказы"));
    ordersTable->setFixedHeight(35);
    connect(ordersTable, SIGNAL(clicked(bool)), this, SLOT(clickedOrdersTable()));
    QPushButton* purchaseTable = new QPushButton(QString::fromLocal8Bit("Покупки"));
    purchaseTable->setFixedHeight(35);
    connect(purchaseTable, SIGNAL(clicked(bool)), this, SLOT(clickedRurchaseTable()));
    QPushButton* buyersTable = new QPushButton(QString::fromLocal8Bit("Покупатели"));
    buyersTable->setFixedHeight(35);
    connect(buyersTable, SIGNAL(clicked(bool)), this, SLOT(clickedBuyersTable()));
    QPushButton* publishersTable = new QPushButton(QString::fromLocal8Bit("Издатели"));
    publishersTable->setFixedHeight(35);
    connect(publishersTable, SIGNAL(clicked(bool)), this, SLOT(clickedPublishersTable()));
    QPushButton* cartsTable = new QPushButton(QString::fromLocal8Bit("Карты"));
    cartsTable->setFixedHeight(35);
    connect(cartsTable, SIGNAL(clicked(bool)), this, SLOT(clickedCartsTable()));

    QPushButton* notPurchasedRequest = new QPushButton(QString::fromLocal8Bit("Не покупаемые книги"));
    notPurchasedRequest->setFixedHeight(35);
    connect(notPurchasedRequest, SIGNAL(clicked(bool)), this, SLOT(clickedNotPurchasedRequest()));
    QPushButton* numberOfPurchasedRequest = new QPushButton(QString::fromLocal8Bit("По количеству купленных"));
    numberOfPurchasedRequest->setFixedHeight(35);
    connect(numberOfPurchasedRequest, SIGNAL(clicked(bool)), this, SLOT(clickedNumberOfPurchasedRequest()));
    QPushButton* mostOrderedRequest = new QPushButton(QString::fromLocal8Bit("Самые заказываемые книги"));
    mostOrderedRequest->setFixedHeight(35);
    connect(mostOrderedRequest, SIGNAL(clicked(bool)), this, SLOT(clickedMostOrderedRequest()));

    QPushButton* notPurchasedReports = new QPushButton(QString::fromLocal8Bit("Не покупаемые книги"));
    notPurchasedReports->setFixedHeight(35);
    connect(notPurchasedReports, SIGNAL(clicked(bool)), this, SLOT(clickedNotPurchasedReports()));
    QPushButton* numberOfPurchasedReports = new QPushButton(QString::fromLocal8Bit("По количеству купленных"));
    numberOfPurchasedReports->setFixedHeight(35);
    connect(numberOfPurchasedReports, SIGNAL(clicked(bool)), this, SLOT(clickedNumberOfPurchasedReports()));
    QPushButton* mostOrderedReports = new QPushButton(QString::fromLocal8Bit("Самые заказываемые книги"));
    mostOrderedReports->setFixedHeight(35);
    connect(mostOrderedReports, SIGNAL(clicked(bool)), this, SLOT(clickedMostOrderedReports()));
    QPushButton* booksRequest = new QPushButton(QString::fromLocal8Bit("Таблица книги"));
    booksRequest->setFixedHeight(35);
    connect(booksRequest, SIGNAL(clicked(bool)), this, SLOT(clickedBooksReports()));

    QHBoxLayout* namesHBox = new QHBoxLayout;

    QVBoxLayout* tablesVBox = new QVBoxLayout;
    QVBoxLayout* requestsVBox = new QVBoxLayout;
    QVBoxLayout* reportsVBox = new QVBoxLayout;

    tablesVBox->addWidget(tables, Qt::AlignTop);
    tablesVBox->addWidget(booksTable, Qt::AlignTop);
    tablesVBox->addWidget(ordersTable, Qt::AlignTop);
    tablesVBox->addWidget(purchaseTable, Qt::AlignTop);
    tablesVBox->addWidget(buyersTable, Qt::AlignTop);
    tablesVBox->addWidget(publishersTable, Qt::AlignTop);
    tablesVBox->addWidget(cartsTable, Qt::AlignTop);

    requestsVBox->addWidget(requests, Qt::AlignTop);
    requestsVBox->addWidget(notPurchasedRequest, Qt::AlignTop);
    requestsVBox->addWidget(numberOfPurchasedRequest, Qt::AlignTop);
    requestsVBox->addWidget(mostOrderedRequest, Qt::AlignTop);
    requestsVBox->addStretch(2000);

    reportsVBox->addWidget(reports, Qt::AlignTop);
    reportsVBox->addWidget(notPurchasedReports, Qt::AlignTop);
    reportsVBox->addWidget(numberOfPurchasedReports, Qt::AlignTop);
    reportsVBox->addWidget(mostOrderedReports, Qt::AlignTop);
    reportsVBox->addWidget(booksRequest, Qt::AlignTop);
    reportsVBox->addStretch(2000);

    namesHBox->addLayout(tablesVBox);
    namesHBox->addLayout(requestsVBox);
    namesHBox->addLayout(reportsVBox);

    mainBox->addLayout(namesHBox, Qt::AlignTop);

    QLabel* inf = new QLabel(QString::fromLocal8Bit("Выполнил студент группы 14-12 ПОк Ильин Максим"));
    inf->setAlignment(Qt::AlignRight);

    mainBox->addWidget(inf, Qt::AlignTop);

    widget = new Widget;

}

void MainMenu::clickedBooksTable()
{
    widget->show();
    widget->booksShow();
    deleteLater();
}

void MainMenu::clickedCartsTable()
{
    widget->show();
    widget->cartsShow();
    deleteLater();
}

void MainMenu::clickedRurchaseTable()
{
    widget->show();
    widget->purchaseShow();
    deleteLater();
}

void MainMenu::clickedOrdersTable()
{
    widget->show();
    widget->ordersShow();
    deleteLater();
}

void MainMenu::clickedPublishersTable()
{
    widget->show();
    widget->publishersShow();
    deleteLater();
}

void MainMenu::clickedBuyersTable()
{
    widget->show();
    widget->buyersShow();
    deleteLater();
}

void MainMenu::clickedNotPurchasedRequest()
{
    widget->show();
    widget->notPurchasedRequest();
    deleteLater();
}

void MainMenu::clickedNumberOfPurchasedRequest()
{
    CountEntry* c = new CountEntry(this, "NumberOfPurchasedRequest", widget);
    c->show();
}

void MainMenu::clickedMostOrderedRequest()
{
    widget->show();
    widget->mostOrderedRequest();
    deleteLater();
}

void MainMenu::clickedNotPurchasedReports()
{
    widget->notPurchasedReports();
}

void MainMenu::clickedNumberOfPurchasedReports()
{
    CountEntry* c = new CountEntry(this, "NumberOfPurchasedReports", widget);
    c->show();
}

void MainMenu::clickedMostOrderedReports()
{
    widget->mostOrderedReports();
}

void MainMenu::clickedBooksReports()
{
    widget->booksReports();
}

void MainMenu::help()
{
    QMessageBox::information(0, QString::fromLocal8Bit("Справка"),
                             QString::fromLocal8Bit("База данных Книжный магазин предназначена для автоматизации работы магазина"
                             " и для решения задач такие как: "
                             "\n просмотр информаии о книгах "
                             "\n добавление новых книг в базу данных "
                             "\n просмотр данных о покупателях "
                             "\n заполнять ведомость о покупках, заказанных книгах и о постоянных покупателях"));
}
