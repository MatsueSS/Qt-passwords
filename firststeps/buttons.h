#ifndef BUTTONS_H
#define BUTTONS_H

#include <QGroupBox>

class QCheckBox;
class QRadioButton;

class buttons : public QGroupBox
{
    Q_OBJECT
private:
    QCheckBox* m_pchk;
    QRadioButton* red_cmd;
    QRadioButton* green_cmd;
    QRadioButton* blue_cmd;

public:
    buttons(QWidget* pwgt = 0);

public slots:
    void slotClickedButton();
};

#endif // BUTTONS_H
