#include "ukmedia_output_widget.h"
#include <QHBoxLayout>

#include <QDebug>
AudioSlider::AudioSlider(QWidget *parent)
{
    Q_UNUSED(parent);
}

AudioSlider::~AudioSlider()
{

}

UkmediaOutputWidget::UkmediaOutputWidget(QWidget *parent) : QWidget(parent)
{
//    QFILEDEVICE_H

    //加载qss样式文件
    QFile QssFile("://combox.qss");
    QssFile.open(QFile::ReadOnly);

    if (QssFile.isOpen()){
        sliderQss = QLatin1String(QssFile.readAll());
//        qDebug()<<"sliderQSs is----------------->"<<sliderQss<<endl;
        QssFile.close();
    } else {
        qDebug()<<"combox.qss is not found"<<endl;
    }

    m_pOutputWidget = new QWidget(this);
    m_pOutputDeviceWidget = new QWidget(m_pOutputWidget);
    m_pMasterVolumeWidget = new QWidget(m_pOutputWidget);
    m_pChannelBalanceWidget = new QWidget(m_pOutputWidget);
    //设置大小
    m_pOutputWidget->setMinimumSize(550,150);
    m_pOutputWidget->setMaximumSize(960,150);
    m_pOutputDeviceWidget->setMinimumSize(550,50);
    m_pOutputDeviceWidget->setMaximumSize(960,50);
    m_pMasterVolumeWidget->setMinimumSize(550,50);
    m_pMasterVolumeWidget->setMaximumSize(960,50);
    m_pChannelBalanceWidget->setMinimumSize(550,50);
    m_pChannelBalanceWidget->setMaximumSize(960,50);

    m_pOutputLabel = new QLabel(tr("Output"),this);
    m_pOutputDeviceLabel = new QLabel(tr("Select output device"),m_pOutputWidget);
    m_pOutputDeviceCombobox = new QComboBox(m_pOutputDeviceWidget);
    m_pOpVolumeLabel = new QLabel(tr("Master volume"),m_pMasterVolumeWidget);
    m_pOutputIconBtn = new QPushButton(m_pMasterVolumeWidget);
    m_pOpVolumeSlider = new AudioSlider(m_pMasterVolumeWidget);
    m_pOpVolumePercentLabel = new QLabel(m_pMasterVolumeWidget);
    m_pOpBalanceLabel = new QLabel(tr("Channel balance"),m_pChannelBalanceWidget);
    m_pLeftBalanceLabel = new QLabel(tr("Left"),m_pChannelBalanceWidget);
    m_pOpBalanceSlider = new AudioSlider(m_pChannelBalanceWidget);
    m_pRightBalanceLabel = new QLabel(tr("right"),m_pChannelBalanceWidget);

    m_pOpVolumeSlider->setOrientation(Qt::Horizontal);
    m_pOpBalanceSlider->setOrientation(Qt::Horizontal);
    m_pOpVolumeSlider->setRange(0,100);
    m_pOutputIconBtn->setFocusPolicy(Qt::NoFocus);
    //输出设备添加布局
    QHBoxLayout *outputDeviceLayout = new QHBoxLayout(m_pOutputDeviceWidget);
    m_pOutputLabel->setFixedSize(83,18);
    m_pOutputDeviceCombobox->setMinimumSize(50,32);
    m_pOutputDeviceCombobox->setMaximumSize(900,32);

    m_pOutputDeviceLabel->setFixedSize(83,24);
    outputDeviceLayout->addItem(new QSpacerItem(16,20,QSizePolicy::Fixed));
    outputDeviceLayout->addWidget(m_pOutputDeviceLabel);
    outputDeviceLayout->addItem(new QSpacerItem(48,20,QSizePolicy::Fixed));
    outputDeviceLayout->addWidget(m_pOutputDeviceCombobox);
    outputDeviceLayout->addItem(new QSpacerItem(16,20,QSizePolicy::Fixed));
    outputDeviceLayout->setSpacing(0);
    m_pOutputDeviceWidget->setLayout(outputDeviceLayout);
    outputDeviceLayout->layout()->setContentsMargins(0,0,0,0);
    //主音量添加布局
    QHBoxLayout *masterLayout = new QHBoxLayout(m_pMasterVolumeWidget);
    m_pOpVolumeLabel->setFixedSize(83,24);
    m_pOutputIconBtn->setFixedSize(24,24);
    m_pOpVolumeSlider->setFixedHeight(20);
    m_pOpVolumePercentLabel->setFixedSize(36,24);
    masterLayout->addItem(new QSpacerItem(16,20,QSizePolicy::Fixed));
    masterLayout->addWidget(m_pOpVolumeLabel);
    masterLayout->addItem(new QSpacerItem(48,20,QSizePolicy::Fixed));
    masterLayout->addWidget(m_pOutputIconBtn);
    masterLayout->addItem(new QSpacerItem(16,20,QSizePolicy::Fixed));
    masterLayout->addWidget(m_pOpVolumeSlider);
    masterLayout->addItem(new QSpacerItem(16,20,QSizePolicy::Fixed));
    masterLayout->addWidget(m_pOpVolumePercentLabel);
    masterLayout->addItem(new QSpacerItem(16,20,QSizePolicy::Fixed));
    masterLayout->setSpacing(0);
    m_pMasterVolumeWidget->setLayout(masterLayout);
    m_pMasterVolumeWidget->layout()->setContentsMargins(0,0,0,0);
    //声道平衡添加布局
    QHBoxLayout *soundLayout = new QHBoxLayout(m_pChannelBalanceWidget);
    m_pOpBalanceLabel->setFixedSize(83,24);
    m_pLeftBalanceLabel->setFixedSize(24,24);
    m_pOpBalanceSlider->setFixedHeight(20);
    m_pRightBalanceLabel->setFixedSize(36,24);
    soundLayout->addItem(new QSpacerItem(16,20,QSizePolicy::Fixed));
    soundLayout->addWidget(m_pOpBalanceLabel);
    soundLayout->addItem(new QSpacerItem(48,20,QSizePolicy::Fixed));
    soundLayout->addWidget(m_pLeftBalanceLabel);
    soundLayout->addItem(new QSpacerItem(16,20,QSizePolicy::Fixed));
    soundLayout->addWidget(m_pOpBalanceSlider);
    soundLayout->addItem(new QSpacerItem(16,20,QSizePolicy::Fixed));
    soundLayout->addWidget(m_pRightBalanceLabel);
    soundLayout->addItem(new QSpacerItem(16,20,QSizePolicy::Fixed));
    soundLayout->setSpacing(0);
    m_pChannelBalanceWidget->setLayout(soundLayout);
    m_pChannelBalanceWidget->layout()->setContentsMargins(0,0,0,0);
    //进行整体布局
    QVBoxLayout *vLayout = new QVBoxLayout(m_pOutputWidget);
    vLayout->addWidget(m_pOutputDeviceWidget);
    vLayout->addWidget(m_pMasterVolumeWidget);
    vLayout->addWidget(m_pChannelBalanceWidget);
    m_pOutputWidget->setLayout(vLayout);
    vLayout->setSpacing(0);
    m_pOutputWidget->layout()->setContentsMargins(0,0,0,0);

    QVBoxLayout *vLayout1 = new QVBoxLayout(this);
    vLayout1->addWidget(m_pOutputLabel);
    vLayout1->addItem(new QSpacerItem(16,20,QSizePolicy::Fixed));
    vLayout1->addWidget(m_pOutputWidget);
    this->setLayout(vLayout1);
    this->layout()->setContentsMargins(0,0,0,0);

    m_pOutputDeviceWidget->setObjectName("outputDeviceWidget");
    m_pMasterVolumeWidget->setObjectName("masterVolumeWidget");
    //设置样式
    m_pOutputLabel->setObjectName("m_pOutputLabel");
//    this->setStyleSheet("QWidget{background-color:rgba(0,0,0,0);}");
    m_pOutputLabel->setStyleSheet("QLabel#m_pOutputLabel{font-size:18px;"
                                  "font-family:Noto Sans S Chinese;"
                                  "color:rgba(0,0,0,1);"
                                  "background:transparent;"
                                  "font-weight:500;"
                                  "line-height:40px;}");
    m_pOutputDeviceLabel->setStyleSheet("QLabel{font-size:14px;"
                                        "font-family:Noto Sans S Chinese;"
                                        "font-weight:400;"
                                        "color:rgba(67,67,69,1);"
                                        "line-height:40px;}");
    m_pOpVolumeLabel->setStyleSheet("QLabel{font-size:14px;"
                                    "font-family:Noto Sans S Chinese;"
                                    "font-weight:400;"
                                    "color:rgba(67,67,69,1);"
                                    "line-height:40px;}");
    m_pOpVolumePercentLabel->setStyleSheet("QLabel{font-size:14px;"
                                           "font-family:Noto Sans S Chinese;"
                                           "font-weight:400;"
                                           "color:rgba(67,67,69,1);"
                                           "line-height:40px;}");
    m_pOpBalanceLabel->setStyleSheet("QLabel{font-size:14px;"
                                     "font-family:Noto Sans S Chinese;"
                                     "font-weight:400;"
                                     "color:rgba(67,67,69,1);"
                                     "line-height:40px;}");
    m_pLeftBalanceLabel->setStyleSheet("QLabel{font-size:14px;"
                                       "font-family:Noto Sans S Chinese;"
                                       "font-weight:400;"
                                       "color:rgba(67,67,69,1);"
                                       "line-height:40px;}");
    m_pRightBalanceLabel->setStyleSheet("QLabel{font-size:14px;"
                                        "font-family:Noto Sans S Chinese;"
                                        "font-weight:400;"
                                        "color:rgba(67,67,69,1);"
                                        "line-height:40px;}");

    m_pOpVolumeSlider->setStyleSheet("QSlider::groove:horizontal {"
                                  "border: 0px solid #bbb; }"
                                  "QSlider::sub-page:horizontal {"
                                  "background: #3D6BE5;border-radius: 2px;"
                                  "margin-top:8px;margin-bottom:9px;}"
                                  "QSlider::add-page:horizontal {"
                                  "background:  rgba(52,70,80,90%);"
                                  "border: 0px solid #777;"
                                  "border-radius: 2px;"
                                  "margin-top:8px;"
                                  "margin-bottom:9px;}"
                                  "QSlider::handle:horizontal {"
                                  "width: 20px;"
                                  "height: 20px;"
                                  "background: #3D6BE5;"
                                  "border-radius:10px;}");
    m_pOpBalanceSlider->setStyleSheet("QSlider::groove:horizontal {"
                                   "border: 0px solid #bbb; }"
                                   "QSlider::sub-page:horizontal {"
                                   "background: #3D6BE5;border-radius: 2px;"
                                   "margin-top:8px;margin-bottom:9px;}"
                                   "QSlider::add-page:horizontal {"
                                   "background:  rgba(52,70,80,90%);"
                                   "border: 0px solid #777;"
                                   "border-radius: 2px;"
                                   "margin-top:8px;"
                                   "margin-bottom:9px;}"
                                   "QSlider::handle:horizontal {"
                                   "width: 20px;"
                                   "height: 20px;"
                                   "background: rgb(61,107,229);"
                                   "border-radius:10px;}");
    m_pOutputDeviceCombobox->setStyleSheet("QComboBox {width:140px;height:30px;background:rgba(248,248,248,1);"
                                        "border:2px solid rgba(218, 227, 250, 1);border-radius:4px;}"
                                        "QComboBox QAbstractItemView{"
                                        "font-size: 14px;height: 80px;width: 140px;}"
                                        "QComboBox QAbstractItemView::item {"
                                        "height: 30px;}"
                                        "QComboBox QAbstractItemView::item:hover {"
                                        "background: rgba(218,227,250,0.9);"
                                        "border-radius: 2px;"
                                        "font-color: rgb(225, 0, 0);}"
                                        "QComboBox::drop-down{background-color:rgba(248,248,248,1);}"
                                        "QComboBox:on{background:linear-gradient(0deg,rgba(248,248,248,1) 0%,rgba(248,248,248,0.9) 100%);"
                                        "border-radius:4px;}"
                                        "QComboBox::down-arrow:hover{"
                                        "width:184px;height:30px;"
                                        "background:linear-gradient(180deg,rgba(218,227,250,0.9),rgba(218,227,250,1));"
                                        "border-radius:2px;}"
                                        "QComboBox::down-arrow {"
                                        "image: url(/usr/share/ukui-media/img/dowm.png);"
                                        "height:6px;width:11px;}"
                                        "QComboBox::down-arrow:hover{"
                                        "width:184px;height:30px;"
                                        "background:linear-gradient(180deg,rgba(218,227,250,0.9),rgba(218,227,250,1));"
                                        "border-radius:2px;}");
    m_pOutputIconBtn->setStyleSheet("QPushButton{background:transparent;border:0px;padding-left:0px;}");

    m_pOutputWidget->setStyleSheet("QWidget{width: 552px;height: 150px;"
                        "background: rgba(244,244,244,1);"
                        "border-radius: 4px;}");

    m_pOutputDeviceWidget->setStyleSheet("QWidget#outputDeviceWidget{"
                                         "border-bottom:1px solid white;"
                                         "border-radius:0px;}");
    m_pMasterVolumeWidget->setStyleSheet("QWidget#masterVolumeWidget{border-bottom:1px solid white;"
                                         "border-radius:0px;}");
}

UkmediaOutputWidget::~UkmediaOutputWidget()
{

}
