#include "autorization.h"
#include "mainmenu.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainMenu* w = new MainMenu;

    Autorization* aut = new Autorization(w);
    aut->show();

    return a.exec();
}
