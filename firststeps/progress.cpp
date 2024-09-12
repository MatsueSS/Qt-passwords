#include "progress.h"
#include <QtWidgets>

Progress::Progress(QWidget* pobj /*=0*/) : QWidget(pobj), m_step(0)
{
    p_pb = new QProgressBar;
    p_pb->setAlignment(Qt::AlignCenter);
    p_pb->setRange(0, 5);
    p_pb->setMinimumWidth(200);

    QPushButton* pcmdStep = new QPushButton("&Step");
    QPushButton* pcmdReset = new QPushButton("&Reset");

    connect(pcmdStep, SIGNAL(clicked()), SLOT(slotStep()));
    connect(pcmdReset, SIGNAL(clicked()), SLOT(slotReset()));

    QHBoxLayout* phbLayout = new QHBoxLayout;
    phbLayout->addWidget(p_pb);
    phbLayout->addWidget(pcmdStep);
    phbLayout->addWidget(pcmdReset);
    setLayout(phbLayout);
}

void Progress::slotStep()
{
    p_pb->setValue(++m_step);
}

void Progress::slotReset()
{
    m_step = 0;
    p_pb->reset();
}
