#include "countentry.h"
#include <QLabel>
#include <QBoxLayout>
#include <QPushButton>
#include "widget.h"

CountEntry::CountEntry(QWidget *pa, QString s, Widget *parent) : widget(parent), par(pa), metod(s)
{
    QVBoxLayout* mainL = new QVBoxLayout(this);

    QLabel* l = new QLabel(QString::fromLocal8Bit("Введите количество купленных товаров и выберите знак"));

    count = new QLineEdit;

    sign = new QComboBox;
    sign->setFixedHeight(30);
    sign->addItem(">");
    sign->addItem("=");
    sign->addItem("<");

    QPushButton* ok = new QPushButton;
    ok->setText("OK");
    ok->setFixedHeight(30);
    connect(ok, SIGNAL(clicked(bool)), this, SLOT(clickedOk()));

    mainL->addWidget(l);
    mainL->addWidget(count);
    mainL->addWidget(sign);
    mainL->addWidget(ok);
}

void CountEntry::clickedOk()
{
    if(metod == "NumberOfPurchasedReports")
    {
        widget->numberOfPurchasedReports(count->text().toInt(), sign->currentIndex() + 1);
    }
    else
    {
        widget->show();
        widget->numberOfPurchasedRequest(count->text().toInt(), sign->currentIndex() + 1);
        par->deleteLater();
    }
    deleteLater();
}
