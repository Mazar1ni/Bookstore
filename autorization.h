#ifndef AUTORIZATION_H
#define AUTORIZATION_H

#include <QWidget>
#include <QKeyEvent>
#include <QLineEdit>

class Autorization : public QWidget
{
    Q_OBJECT
public:
    explicit Autorization(QWidget *parent);

private slots:
    void checkPassForLogin(bool);

signals:
    void connected();

private:
    void keyPressEvent(QKeyEvent* event);

    QLineEdit* pass;
    QWidget* mainWidget;

};

#endif // AUTORIZATION_H
