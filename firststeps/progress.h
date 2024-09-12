#ifndef PROGRESS_H
#define PROGRESS_H

#include <QWidget>

class QProgressBar;

class Progress : public QWidget
{
    Q_OBJECT
private:
    QProgressBar* p_pb;
    int m_step;

public:
    Progress(QWidget* p_obj = 0);

public slots:
    void slotStep();
    void slotReset();
};

#endif // PROGRESS_H
