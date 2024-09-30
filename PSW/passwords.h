#ifndef PASSWORDS_H
#define PASSWORDS_H

#include <QtWidgets>
#include <QMap>

using string = QString;

//not recommended edit this value
enum { fault_num = 5 };

class File {
public:
    virtual void read(const string& filename) = 0;
    virtual void write(const string& filename) = 0;
};

class Encrypt {
    string msg; // message for encrypt
    bool flag_crypt = 0;

    void _decryption(); // make decryption
    void _encryption(); // make encryption

public:
    Encrypt(string msg = "");
    Encrypt(const Encrypt& obj);

    void getMsg(string& msg);
    void setMsg(string newMsg);
    string& showMsg();
};

class Password {
    string name; // name of password
    Encrypt password; // password
    string chars; // chars for generate password
    int min_l, max_l; // length password

public:
    Password(string name, int min_l = 20, int max_l = 21, string chars = "qwertuiopasdfghjklxcvbnm0123456789@$%&*!ABCDEFGHIJKLMNOPQRSTUVMXYZ");
    Password(Encrypt msg);

    string& showName();
    void generatePassword();
    string& showPassword();
    string printPassword();

    void read(const string& fileName);
    void write(const string& fileName);
};

class PasswordFileManager : public File {
    Password* password;
public:
    PasswordFileManager(Password* psw);

    virtual void write(const string& filename) override;
    virtual void read(const string& filename) override;
};

class LibPasswords {
    QMap<string, Password> libPsw;

    enum sort { _quick_sort = 0, _choice_sort = 1, _bubble_sort = 2, _pyramid_sort = 3, _insert_sort = 4 };

public:
    LibPasswords();

    void removePassword(string name);
    void addPassword(Password psw);
    void sortPasswords(sort type);
    Password findPassword(const string& name);

    QMap<string, Password>& getLib();

    string operator[](int size);

    void read(const string& fileName);
    void write(const string& fileName);

    friend void QuickSort(LibPasswords& lib, int low, int high, bool(*func)(string, string));
};

class LibPasswordsFileManager : public File {
    LibPasswords* lib {nullptr};
public:
    LibPasswordsFileManager(LibPasswords* lib);


    virtual void write(const string& filename) override;
    virtual void read(const string& filename) override;
};

class PasswordsWidget : public QWidget{
Q_OBJECT
private:
    LibPasswords* Lib {nullptr};
    Password* psw{nullptr};

    QLineEdit* ptxtNm {nullptr};
    QLineEdit* ptxtPsw {nullptr};

public:
    PasswordsWidget(LibPasswords* lib = nullptr, QWidget* pwgt = 0);

    ~PasswordsWidget();

signals:
    void newPassword(QString);

public slots:
    void slotGeneratePassword();
    void slotWriteFilePassword();
    void slotReadFilePassword();
    void slotWriteFileLib();
    void slotReadFileLib();

};

#endif // PASSWORDS_H
