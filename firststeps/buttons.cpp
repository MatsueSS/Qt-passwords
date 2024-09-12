#include "buttons.h"

#include <QtWidgets>

buttons::buttons(QWidget* pwgt /*=0*/) : QGroupBox("QGroupBox", pwgt)
{
    resize(150,200);
    setCheckable(true);
    setChecked(true);

    red_cmd = new QRadioButton("&Red");
    blue_cmd = new QRadioButton("&Blue");
    green_cmd = new QRadioButton("&Green");
    green_cmd->setChecked(true);
    connect(red_cmd, SIGNAL(clicked()), SLOT(slotClickedButton()));
    connect(blue_cmd, SIGNAL(clicked()), SLOT(slotClickedButton()));
    connect(green_cmd, SIGNAL(clicked()), SLOT(slotClickedButton()));

    m_pchk = new QCheckBox("&Light");
    m_pchk->setChecked(true);
    connect(m_pchk, SIGNAL(clicked()), SLOT(slotClickedButton()));

    QPushButton* xt = new QPushButton("&Exit");
    connect(xt, SIGNAL(clicked()), qApp, SLOT(quit()));

    QVBoxLayout* vbLayout = new QVBoxLayout;
    vbLayout->addWidget(red_cmd);
    vbLayout->addWidget(green_cmd);
    vbLayout->addWidget(blue_cmd);
    vbLayout->addWidget(m_pchk);
    vbLayout->addWidget(xt);
    setLayout(vbLayout);

    slotClickedButton();
}

void buttons::slotClickedButton()
{
    QPalette pal = palette();

    int nLight = m_pchk->isChecked() ? 150 : 80;
    if(red_cmd->isChecked()) pal.setColor(backgroundRole(), QColor(Qt::red).light(nLight));
    else if(green_cmd->isChecked()) pal.setColor(backgroundRole(), QColor(Qt::green).light(nLight));
    else pal.setColor(backgroundRole(), QColor(Qt::blue).light(nLight));

    setPalette(pal);
}
