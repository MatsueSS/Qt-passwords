#include "passwords.h"

#include <algorithm>
#include <QFile>

Encrypt::Encrypt(string msg) : msg(msg) { _encryption(); }
void Encrypt::_encryption(){
    for(int i = 0; i < msg.size(); i++){
        msg[i] = QChar(msg[i].unicode() + fault_num);
        if(msg[i].unicode() > 126) msg[i] = QChar(32 + (msg[i].unicode() - 127));
    }
    flag_crypt = 1;
}
Encrypt::Encrypt(const Encrypt& obj) : msg(obj.msg), flag_crypt(obj.flag_crypt) {}
void Encrypt::_decryption(){
    for(int i = 0; i < msg.size(); i++){
        msg[i] = QChar(msg[i].unicode() - fault_num);
        if(msg[i].unicode() < 32) msg[i] = QChar(127 - (32 - msg[i].unicode()));
    }
    flag_crypt = 0;
}
void Encrypt::getMsg(string& msg) { _decryption(); msg = this->msg; _encryption(); }
void Encrypt::setMsg(string newMsg) { msg = newMsg; _encryption(); }
string& Encrypt::showMsg() { return msg; }

Password::Password(string name, int min_l, int max_l, string chars) : name(name), min_l(min_l), max_l(max_l), chars(chars) { srand(time(nullptr)); }
void Password::generatePassword(){
    string psw = "";
    int length = rand()%(max_l-min_l)+min_l;
    for(int i = 0; i < length; i++){
        psw += chars[rand()%chars.size()];
    }
    password.setMsg(psw);
}
Password::Password(Encrypt msg) : password(msg) {}

string Password::printPassword() { string psw; password.getMsg(psw); return psw; }

string& Password::showPassword() { return password.showMsg(); }

string& Password::showName() { return name; }

PasswordFileManager::PasswordFileManager(Password& password) : password(password) {}

Password& PasswordFileManager::getPassword(){
    return password;
}

/*virtual*/void PasswordFileManager::read(const string& filename)
{
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        while(!in.atEnd()){
            QStringList line = in.readLine().split(" ");
            password.showName() = line[0];
            password.showPassword() = line[1];
        }
    }
    else { QErrorMessage err; err.showMessage("File cannot be opened"); }
    file.close();
}

/*virtual*/void PasswordFileManager::write(const string& filename)
{
    QFile file(filename);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
        QTextStream out(&file);
        out << password.showName() << " " << password.showPassword() << "\n";
    }
    else { QErrorMessage err; err.showMessage("File cannot be opened"); }
    file.close();
}

void Password::read(const string& filename){
    PasswordFileManager user(*this);
    user.read(filename);
}
void Password::write(const string& filename){
    PasswordFileManager user(*this);
    user.write(filename);
}

LibPasswords::LibPasswords() {}
void LibPasswords::addPassword(Password psw){
    libPsw.insert(psw.showName(), psw);
}
void LibPasswords::removePassword(string name){
    libPsw.erase(libPsw.find(name));
}
Password LibPasswords::findPassword(const string& name){
    if(libPsw.find(name) != libPsw.end()) return libPsw.find(name).value();
    else { QErrorMessage err; err.showMessage("Password is not find"); }
}

QMap<string, Password>& LibPasswords::getLib(){
    return libPsw;
}

LibPasswordsFileManager::LibPasswordsFileManager(LibPasswords& lib) : lib(lib) {}

/*virtual*/void LibPasswordsFileManager::read(const string& filename){
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        while(!in.atEnd()){
            QStringList line = in.readLine().split(" ");
            Password psw(line[0]);
            psw.showPassword() = line[1];
            lib.addPassword(psw);
        }
    }
    else { QErrorMessage err; err.showMessage("File cannot be opened"); }
    file.close();
}
/*virtual*/void LibPasswordsFileManager::write(const string& filename){
    QFile file(filename);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
        QTextStream out(&file);
        QMap<string, Password>::iterator it = lib.getLib().begin();
        while(it != lib.getLib().end()){
            out << it.key() << " " << it.value().showPassword() << "\n";
        }
    }
    else { QErrorMessage err; err.showMessage("File cannot be opened"); }
    file.close();
}

void LibPasswords::read(const string& filename){
    LibPasswordsFileManager user(*this);
    user.read(filename);
}
void LibPasswords::write(const string& filename){
    LibPasswordsFileManager user(*this);
    user.write(filename);
}
string LibPasswords::operator[](int size){
    if(size >= libPsw.size()) return "Size must be less than size map";
    QMap<string, Password>::iterator it = libPsw.begin();
    for(int i = 0; i < libPsw.size() && i != size; i++) it++;
    return it.key();
}

void swap(QMap<string, Password>::iterator a, QMap<string, Password>::iterator b){
    QMap<string, Password>::iterator temp = a;
    a = b;
    b = temp;
}

void QuickSort(LibPasswords& lib, int low, int high, bool(*func)(string, string)){
    if(low > high) return;
    string p = lib[(low+high)/2];
    int i = low, j = high;
    while(i <= j){
        while(func(p, lib[i])) i++;
        while(func(lib[j], p)) j--;
        if(i <= j){
            swap(lib.libPsw.find(lib[i++]), lib.libPsw.find(lib[j--]));
        }
    }
    QuickSort(lib, low, j, func);
    QuickSort(lib, i, high, func);
}

PasswordsWidget::PasswordsWidget(LibPasswords* lib, QWidget* pwgt /*=0*/) : QWidget(pwgt), psw(""){
    setWindowTitle("Генератор паролей");

    QLabel* plblNm = new QLabel("&Напишите название сайта");
    ptxtNm = new QLineEdit("Default name");
    plblNm->setBuddy(ptxtNm);

    QLabel* plblPsw = new QLabel("Н&ажмите клавишу генерации пароля");
    ptxtPsw = new QLineEdit;
    QPushButton* pcmdPsw = new QPushButton("&Сгенерировать пароль");
    plblPsw->setBuddy(pcmdPsw);
    ptxtPsw->setReadOnly(true);
    connect(pcmdPsw, SIGNAL(clicked()), SLOT(slotGeneratePassword()));
    connect(this, SIGNAL(newPassword(QString)), ptxtPsw, SLOT(setText(const QString&)));
    connect(ptxtPsw, SIGNAL(textChanged(const QString&)), ptxtPsw, SLOT(selectAll()));
    connect(ptxtPsw, SIGNAL(textChanged(const QString&)), ptxtPsw, SLOT(copy()));

    QPushButton* pcmdExit = new QPushButton("&Выход");
    connect(pcmdExit, SIGNAL(clicked()), qApp, SLOT(quit()));

    QMenuBar* pmnBar = new QMenuBar(this);
    QMenu* pmn = pmnBar->addMenu("Файлы");

    QAction* pmnWrtFl = new QAction("Запись", this);
    pmnWrtFl->setText("&Write");
    pmnWrtFl->setShortcut(QKeySequence("CTRL+S"));
    pmnWrtFl->setToolTip("Save Document");
    pmnWrtFl->setStatusTip("Save the data in the file");
    pmnWrtFl->setWhatsThis("Save the data in the file");

    connect(pmnWrtFl, SIGNAL(triggered()), SLOT(slotWriteFilePassword()));
//    pmnWrtFl->setEnabled(false);
    QAction* pmnRdFl = new QAction("Чтение файла", this);
    connect(pmnRdFl, SIGNAL(triggered()), SLOT(slotReadFilePassword()));

    pmn->addAction(pmnWrtFl);
    pmn->addAction(pmnRdFl);

    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(plblNm);
    pvbxLayout->addWidget(ptxtNm);
    pvbxLayout->addWidget(plblPsw);
    pvbxLayout->addWidget(ptxtPsw);
    pvbxLayout->addWidget(pcmdPsw);
    pvbxLayout->addWidget(pcmdExit);
    pvbxLayout->setMenuBar(pmnBar);

    setLayout(pvbxLayout);
}

void PasswordsWidget::slotGeneratePassword(){
    psw.generatePassword();
    emit newPassword(psw.printPassword());
}

void PasswordsWidget::slotWriteFilePassword(){
    psw.showName() = ptxtNm->text();
    string filename = QFileDialog::getOpenFileName(this, "Выберите файл для записи пароля");
    if(filename.size() >= 1){
        PasswordFileManager manager(psw);
        manager.write(filename);
    }
}

void PasswordsWidget::slotReadFilePassword(){
    string filename = QFileDialog::getOpenFileName(this, "Выберите файл для чтения пароля");
    if(filename.size() >=1){
        PasswordFileManager manager(psw);
        manager.read(filename);
        psw = manager.getPassword();
        ptxtNm->setText(psw.showName());
        ptxtPsw->setText(psw.printPassword());
    }
}

void PasswordsWidget::slotReadFileLib(){
    string filename = QFileDialog::getOpenFileName(this, "Выберите файл для чтения");
    if(filename.size() >= 1){
        Lib = new LibPasswords;
        LibPasswordsFileManager manager(*Lib);
        manager.read(filename);
    }
}

void PasswordsWidget::slotWriteFileLib(){
    string filename = QFileDialog::getOpenFileName(this, "Выберите файл для записи");
    if(filename.size() >= 1){
        Lib = new LibPasswords;
        LibPasswordsFileManager manager(*Lib);
        manager.write(filename);
    }
}

PasswordsWidget::~PasswordsWidget(){
    delete Lib;
}
