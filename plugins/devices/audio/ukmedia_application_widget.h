#ifndef UKMEDIAAPPLICATIONWIDGET_H
#define UKMEDIAAPPLICATIONWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include "ukmedia_output_widget.h"
class UkmediaApplicationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UkmediaApplicationWidget(QWidget *parent = nullptr);
    ~UkmediaApplicationWidget();
    friend class UkmediaMainWidget;
Q_SIGNALS:

public Q_SLOTS:

private:
    QLabel *m_pApplicationLabel;
    QLabel *m_pNoAppLabel;
    QWidget *m_pAppWid;
    QWidget *m_pDisplayAppVolumeWidget;
    QStringList *m_pAppVolumeList;
    QLabel *m_pAppLabel;
    QLabel *m_pAppIconLabel;
    QPushButton *m_pAppIconBtn;
    QLabel *m_pAppVolumeLabel;
    AudioSlider *m_pAppSlider;
    QScrollArea *m_pAppArea;
    QVBoxLayout *m_pVLayout;
};

#endif // UKMEDIAAPPLICATIONWIDGET_H
