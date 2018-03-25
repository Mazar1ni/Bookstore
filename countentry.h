#ifndef COUNTENTRY_H
#define COUNTENTRY_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>

class Widget;

class CountEntry : public QWidget
{
    Q_OBJECT
public:
    explicit CountEntry(QWidget *pa, QString s, Widget *parent = nullptr);

signals:

public slots:
    void clickedOk();

private:
    QComboBox* sign;
    QLineEdit* count;
    Widget* widget;
    QWidget* par;
    QString metod;

};

#endif // COUNTENTRY_H
