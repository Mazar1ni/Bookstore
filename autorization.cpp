#include "autorization.h"
#include <QLabel>
#include <qDebug>
#include <QPushButton>
#include <QMessageBox>

Autorization::Autorization(QWidget *parent) : mainWidget(parent)
{
    setFixedSize(300, 100);
    QLabel* l = new QLabel(this);
    l->setText(QString::fromLocal8Bit("Для входа в базу данных введите пароль:"));
    l->move(15, 15);

    pass = new QLineEdit(this);
    pass->setEchoMode(QLineEdit::Password);
    pass->setInputMethodHints(Qt::ImhHiddenText| Qt::ImhNoPredictiveText|Qt::ImhNoAutoUppercase);
    pass->resize(210, 25);
    pass->move(15, 45);

    QPushButton* open = new QPushButton(this);
    open->resize(50, 25);
    open->move(235, 45);
    open->setText(QString::fromLocal8Bit("Войти"));

    connect(open, SIGNAL(clicked(bool)), this, SLOT(checkPassForLogin(bool)));
}

void Autorization::checkPassForLogin(bool)
{
    if(pass->text() == "1234")
    {
        mainWidget->show();
        deleteLater();
    }
    else
    {
        QMessageBox::critical(NULL,QString::fromLocal8Bit("Ошибка"), QString::fromLocal8Bit("Неверный логин"));
        pass->clear();
    }
}

void Autorization::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == 16777220)
    {
        checkPassForLogin(true);
    }
}
