#include "mainwindow.h"
//#include "calculator.h"
//#include "progress.h"
//#include "buttons.h"
#include "passwords.h"
//#include "intlistmodel.h"
//#include "mouseobserver.h"

#include <QtWidgets>
#include <QApplication>
//#include <QtCore>
//#include <QtAlgorithms>
//#include <QCursor>

class ResizeObserver : public QLabel{
public:
    ResizeObserver(QWidget* pwgt = 0) : QLabel(pwgt)
    {
        setAlignment(Qt::AlignCenter);
    }
protected:
    virtual void resizeEvent(QResizeEvent* pe)
    {
        setText(QString("Resized")
                + "\n width()=" + QString::number(pe->size().width())
                + "\n height()=" + QString::number(pe->size().height())
                    );
    }
};

//class SimpleDelegate : public QStyledItemDelegate{
//public:
//    SimpleDelegate(QObject* pwgt = 0) : QStyledItemDelegate(pwgt) {}

//    void paint(QPainter* qPainter, const QStyleOptionViewItem& option, const QModelIndex& index) const
//    {
//        if(option.state & QStyle::State_MouseOver)
//        {
//            QRect rect = option.rect;
//            QLinearGradient gradient(0, 0, rect.width(), rect.height());
//            gradient.setColorAt(0, Qt::white);
//            gradient.setColorAt(0.5, Qt::blue);
//            gradient.setColorAt(1, Qt::green);
//            qPainter->setBrush(gradient);
//            qPainter->drawRect(rect);
//        }
//        QStyledItemDelegate::paint(qPainter, option, index);
//    }

//    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& /*index*/) const
//    {
//        return QSize(option.rect.width(), 55);
//    }
//};

//void messageToFile(QtMsgType type, const QMessageLogContext& context, const QString& msg)
//{
//    QFile file("protocol.log");
//    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) return;

//    QString strDateTime = QDateTime::currentDateTime().toString("dd.MM.yy-hh:mm");

//    QTextStream out(&file);

//    switch(type){
//        case QtDebugMsg:
//            out << strDateTime << " Debug: " << msg << ", " << context.file << Qt::endl; break;
//        case QtWarningMsg:
//            out << strDateTime << " Warning: " << msg << ", " << context.file << Qt::endl; break;
//        case QtCriticalMsg:
//            out << strDateTime << " Critical: " << msg << ", " << context.file << Qt::endl; break;
//        case QtFatalMsg:
//            out << strDateTime << " Fatal: " << msg << ", " << context.file << Qt::endl; abort();
//    }
//}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Passwords psw;
    psw.show();

//    QFileSystemModel model;
//    QTreeView treeView;
//    QModelIndex index = model.index(QDir::currentPath());
//    model.setRootPath(QDir::rootPath());
//    treeView.setModel(&model);
//    treeView.setRootIndex(index);
//    treeView.show();

//    qInstallMessageHandler(messageToFile);
//    qInstallMessageHandler(0);

    return a.exec();
}
