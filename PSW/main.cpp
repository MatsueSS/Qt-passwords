#include "mainwindow.h"
#include "passwords.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    PasswordsWidget w;
    w.show();

    return a.exec();
}
