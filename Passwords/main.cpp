#include "mainwindow.h"
#include "passwords.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Passwords psw;
    psw.show();
    return a.exec();
}
