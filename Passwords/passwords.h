#ifndef PASSWORDS_H
#define PASSWORDS_H

#include <QtWidgets>

enum { fault_num = 5 };

class QFile;

class Encrypt : public QObject{
    Q_OBJECT
private:
    QString msg;
    bool flagCrypt{1};

    void _decryption();
    void _encryption();
public:
    Encrypt(QString msg = "");

    QString getMsg();
    void setMsg(const QString& newMsg);
    QString getEncrptMsg();
};

class Passwords : public QWidget{
    Q_OBJECT
private:
    Encrypt password;
    QString chars;
    int minSize, maxSize;

    QLineEdit* plndName = 0;
    QLineEdit* plblPsw = 0;
public:
    Passwords(QWidget* pwgt = 0, int minSize = 20, int maxSize = 21, QString chars = "qwertuiopasdfghjklxcvbnm0123456789@$%&*!ABCDEFGHIJKLMNOPQRSTUVMXYZ");

public slots:
    void generatePassword();
    void writeFile();
    void readFile();

signals:
    void newPassword(QString);
};

#endif // PASSWORDS_H
