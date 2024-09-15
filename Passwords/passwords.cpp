#include "passwords.h"

Passwords::Passwords(QWidget* pwgt /*=0*/, int minSize, int maxSize, QString chars) : minSize(minSize), maxSize(maxSize), chars(chars)
{
    srand(time(nullptr));

    QPushButton* ppbGenPas = new QPushButton("&Generate new password");
    connect(ppbGenPas, SIGNAL(clicked()), SLOT(generatePassword()));

    QLabel* plblName = new QLabel("&Write site or name:");
    plndName = new QLineEdit("Defaul Name");
    plblName->setBuddy(plndName);

    plblPsw = new QLineEdit;
    plblPsw->setText(password.getMsg());
    plblPsw->setReadOnly(true);

    QPushButton* ppbtnExit = new QPushButton("&Exit");
    connect(ppbtnExit, SIGNAL(clicked()), qApp, SLOT(quit()));

    QMenuBar* pmnBar = new QMenuBar(this);
    QMenu* pfileBar = pmnBar->addMenu("&File");
    /*write funcs for MenuBar*/

    QAction* pwrtFile = new QAction("File write", this);
    pfileBar->addAction(pwrtFile);
    connect(pwrtFile, SIGNAL(triggered()), this, SLOT(writeFile()));

    QAction* prdFile = new QAction("File read", this);
    pfileBar->addAction(prdFile);
    connect(prdFile, SIGNAL(triggered()), this, SLOT(readFile()));

    connect(this, SIGNAL(newPassword(QString)), plblPsw, SLOT(setText(QString)));
    connect(plblPsw, SIGNAL(textChanged(const QString&)), plblPsw, SLOT(selectAll()));
    connect(plblPsw, SIGNAL(textChanged(const QString&)), plblPsw, SLOT(copy()));

    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(plblName);
    pvbxLayout->addWidget(plndName);
    pvbxLayout->setMenuBar(pmnBar);
    pvbxLayout->addWidget(plblPsw);
    pvbxLayout->addWidget(ppbGenPas);
    pvbxLayout->addWidget(ppbtnExit);

    setLayout(pvbxLayout);
}

void Passwords::generatePassword()
{
    QString psw = "";
    int roll = rand()%(maxSize-minSize)+minSize;
    for(int i = 0; i < roll; i++)
    {
        psw += chars[rand()%chars.size()];
    }
    password.setMsg(psw);
    emit newPassword(password.getMsg());
}

void Passwords::writeFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Take choice");
    if(!fileName.isEmpty())
    {
        QFile file(fileName);
        if(file.open(QIODevice::Append | QIODevice::Text))
        {
            file.write(plndName->text().toUtf8());
            file.write(" ");
            file.write(password.getEncrptMsg().toUtf8());
            file.write("\n");
        }
        file.close();
    }
}

void Passwords::readFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Take choice");
    if(!fileName.isEmpty())
    {
        QFile file(fileName);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text));
        {
            QWidget* pnwWgt = new QWidget;

            QTableWidget* ptblWgt = new QTableWidget(pnwWgt);
            ptblWgt->setColumnCount(2);
            int countRow = 0;

            QTextStream out(&file);
            while(!out.atEnd())
            {
                QString line = out.readLine();
                QStringList lstLine = line.split(" ");
                if(lstLine.size() == 2)
                {
                    ptblWgt->insertRow(countRow);

                    QTableWidgetItem* ptblWgtItName = new QTableWidgetItem;
                    QTableWidgetItem* ptblWgtItPsw = new QTableWidgetItem;

                    ptblWgtItName->setText(lstLine.at(0));
                    ptblWgtItPsw->setText(Encrypt(lstLine.at(1)).getMsg());

                    ptblWgt->setItem(countRow, 0, ptblWgtItName);
                    ptblWgt->setItem(countRow++, 1, ptblWgtItPsw);
                }
            }
            QVBoxLayout* pvbLayout = new QVBoxLayout(pnwWgt);
            pvbLayout->addWidget(ptblWgt);
            pvbLayout->setMargin(10);
            pnwWgt->setLayout(pvbLayout);
            pnwWgt->show();
            pnwWgt->resize(300, 300);

        }
        file.close();
    }
}

Encrypt::Encrypt(QString msg /*=""*/) : msg(msg) { _encryption(); }

QString Encrypt::getMsg()
{
    _decryption();
    QString replyMsg = msg;
    _encryption();
    return replyMsg;
}

void Encrypt::setMsg(const QString& newMsg)
{
    msg = newMsg;
    _encryption();
}

void Encrypt::_encryption()
{
    for(int i = 0; i < msg.size(); i++) {
        msg[i] = QChar(msg[i].unicode() + fault_num);
        if(msg[i].unicode() > 126) msg[i] = QChar(32 + (msg[i].unicode() - 127));
    }
    flagCrypt = 1;
}

void Encrypt::_decryption()
{
    for(int i = 0; i < msg.size(); i++){
        msg[i] = QChar(msg[i].unicode() - fault_num);;
        if(msg[i].unicode() < 32) msg[i] = QChar(127 - (32 - msg[i].unicode()));
    }
    flagCrypt = 0;
}

QString Encrypt::getEncrptMsg()
{
    return msg;
}
