/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#include "proxy.h"
#include "ui_proxy.h"

#include <QDebug>

#define PROXY_SCHEMA              "org.gnome.system.proxy"
#define PROXY_MODE_KEY            "mode"
#define PROXY_AUTOCONFIG_URL_KEY  "autoconfig-url"
#define IGNORE_HOSTS_KEY          "ignore-hosts"

#define HTTP_PROXY_SCHEMA         "org.gnome.system.proxy.http"
#define HTTP_USE_AUTH_KEY         "use-authentication"
#define HTTP_AUTH_USER_KEY        "authentication-user"
#define HTTP_AUTH_PASSWD_KEY      "authentication-password"

#define HTTPS_PROXY_SCHEMA        "org.gnome.system.proxy.https"

#define FTP_PROXY_SCHEMA          "org.gnome.system.proxy.ftp"

#define SOCKS_PROXY_SCHEMA        "org.gnome.system.proxy.socks"

#define PROXY_HOST_KEY       "host"
#define PROXY_PORT_KEY       "port"

Proxy::Proxy()
{
    ui = new Ui::Proxy;
    pluginWidget = new QWidget;
    pluginWidget->setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(pluginWidget);

    pluginName = tr("proxy");
    pluginType = NETWORK;

    pluginWidget->setStyleSheet("background: #ffffff;");

    ui->autoWidget->setStyleSheet("QWidget{background: #F4F4F4; border-radius: 6px;}");
    ui->urlWidget->setStyleSheet("QWidget{background: #F4F4F4; border-radius: 6px;}");
    ui->urlLineEdit->setStyleSheet("QLineEdit{background: #ffffff; border: none; border-radius: 4px; font-size: 14px; color: #000000;}");

    ui->manualWidget->setStyleSheet("QWidget{background: #F4F4F4; border-radius: 6px}");
    ui->httpWidget->setStyleSheet("QWidget{background: #F4F4F4; border-radius: 6px}");
    ui->httpHostLineEdit->setStyleSheet("QLineEdit{background: #ffffff; border: none; border-radius: 4px; font-size: 14px; color: #000000;}");
    ui->httpPortLineEdit->setStyleSheet("QLineEdit{background: #ffffff; border: none; border-radius: 4px; font-size: 14px; color: #000000;}");
    ui->cetificationBtn->setStyleSheet("QPushButton{background: #ffffff; border: none; border-radius: 4px;}");

    ui->httpsWidget->setStyleSheet("QWidget{background: #F4F4F4; border-radius: 6px}");
    ui->httpsHostLineEdit->setStyleSheet("QLineEdit{background: #ffffff; border: none; border-radius: 4px; font-size: 14px; color: #000000;}");
    ui->httpsPortLineEdit->setStyleSheet("QLineEdit{background: #ffffff; border: none; border-radius: 4px; font-size: 14px; color: #000000;}");

    ui->ftpWidget->setStyleSheet("QWidget{background: #F4F4F4; border-radius: 6px}");
    ui->ftpHostLineEdit->setStyleSheet("QLineEdit{background: #ffffff; border: none; border-radius: 4px; font-size: 14px; color: #000000;}");
    ui->ftpPortLineEdit->setStyleSheet("QLineEdit{background: #ffffff; border: none; border-radius: 4px; font-size: 14px; color: #000000;}");

    ui->socksWidget->setStyleSheet("QWidget{background: #F4F4F4; border-radius: 6px}");
    ui->socksHostLineEdit->setStyleSheet("QLineEdit{background: #ffffff; border: none; border-radius: 4px; font-size: 14px; color: #000000;}");
    ui->socksPortLineEdit->setStyleSheet("QLineEdit{background: #ffffff; border: none; border-radius: 4px; font-size: 14px; color: #000000;}");

    ui->ignoreHostsWidget->setStyleSheet("QWidget{background: #F4F4F4; border-radius: 6px}");
    ui->ignoreHostTextEdit->setStyleSheet("QTextEdit{background: #ffffff; border: none; border-radius: 4px; font-size: 14px; color: #000000;}");

    const QByteArray id(PROXY_SCHEMA);
    proxysettings = new QGSettings(id);
    const QByteArray idd(HTTP_PROXY_SCHEMA);
    httpsettings = new QGSettings(idd);
    const QByteArray iddd(HTTPS_PROXY_SCHEMA);
    securesettings = new QGSettings(iddd);
    const QByteArray iid(FTP_PROXY_SCHEMA);
    ftpsettings = new QGSettings(iid);
    const QByteArray iiid(SOCKS_PROXY_SCHEMA);
    sockssettings = new QGSettings(iiid);

    initComponent();

    initProxyModeStatus();
    initAutoProxyStatus();
    initManualProxyStatus();
    initIgnoreHostStatus();
}

Proxy::~Proxy()
{
    delete ui;
    delete proxysettings;
    delete httpsettings;
    delete securesettings;
    delete ftpsettings;
    delete sockssettings;
}

QString Proxy::get_plugin_name(){
    return pluginName;
}

int Proxy::get_plugin_type(){
    return pluginType;
}

QWidget *Proxy::get_plugin_ui(){
    return pluginWidget;
}

void Proxy::plugin_delay_control(){

}

void Proxy::initComponent(){
    //添加自动配置代理开关按钮
    autoSwitchBtn = new SwitchButton(ui->autoWidget);
    autoSwitchBtn->setObjectName("auto");
    ui->autoHorLayout->addWidget(autoSwitchBtn);

    //添加手动配置代理开关按钮
    manualSwitchBtn = new SwitchButton(ui->manualWidget);
    manualSwitchBtn->setObjectName("manual");
    ui->manualHorLayout->addWidget(manualSwitchBtn);

    //QLineEdit 设置数据
    KeyValue * httpHostValue = new KeyValue();
    httpHostValue->schema = HTTP_PROXY_SCHEMA;
    httpHostValue->key = PROXY_HOST_KEY;
    ui->httpHostLineEdit->setUserData(Qt::UserRole, httpHostValue);

    KeyValue * httpsHostValue = new KeyValue();
    httpsHostValue->schema = HTTPS_PROXY_SCHEMA;
    httpsHostValue->key = PROXY_HOST_KEY;
    ui->httpsHostLineEdit->setUserData(Qt::UserRole, httpsHostValue);

    KeyValue * ftpHostValue = new KeyValue();
    ftpHostValue->schema = FTP_PROXY_SCHEMA;
    ftpHostValue->key = PROXY_HOST_KEY;
    ui->ftpHostLineEdit->setUserData(Qt::UserRole, ftpHostValue);

    KeyValue * socksHostValue = new KeyValue();
    socksHostValue->schema = SOCKS_PROXY_SCHEMA;
    socksHostValue->key = PROXY_HOST_KEY;
    ui->socksHostLineEdit->setUserData(Qt::UserRole, socksHostValue);

    KeyValue * httpPortValue = new KeyValue();
    httpPortValue->schema = HTTP_PROXY_SCHEMA;
    httpPortValue->key = PROXY_PORT_KEY;
    ui->httpPortLineEdit->setUserData(Qt::UserRole, httpPortValue);

    KeyValue * httpsPortValue = new KeyValue();
    httpsPortValue->schema = HTTPS_PROXY_SCHEMA;
    httpsPortValue->key = PROXY_PORT_KEY;
    ui->httpsPortLineEdit->setUserData(Qt::UserRole, httpsPortValue);

    KeyValue * ftpPortValue = new KeyValue();
    ftpPortValue->schema = FTP_PROXY_SCHEMA;
    ftpPortValue->key = PROXY_PORT_KEY;
    ui->ftpPortLineEdit->setUserData(Qt::UserRole, ftpPortValue);

    KeyValue * socksPortValue = new KeyValue();
    socksPortValue->schema = SOCKS_PROXY_SCHEMA;
    socksPortValue->key = PROXY_PORT_KEY;
    ui->socksPortLineEdit->setUserData(Qt::UserRole, socksPortValue);


    connect(autoSwitchBtn, SIGNAL(checkedChanged(bool)), this, SLOT(proxyModeChangedSlot(bool)));
    connect(manualSwitchBtn, SIGNAL(checkedChanged(bool)), this, SLOT(proxyModeChangedSlot(bool)));
    connect(ui->urlLineEdit, &QLineEdit::textChanged, this, [=](const QString &txt){proxysettings->set(PROXY_AUTOCONFIG_URL_KEY, QVariant(txt));});

    connect(ui->httpHostLineEdit, &QLineEdit::textChanged, this, [=](const QString &txt){manualProxyTextChanged(txt);});
    connect(ui->httpsHostLineEdit, &QLineEdit::textChanged, this, [=](const QString &txt){manualProxyTextChanged(txt);});
    connect(ui->ftpHostLineEdit, &QLineEdit::textChanged, this, [=](const QString &txt){manualProxyTextChanged(txt);});
    connect(ui->socksHostLineEdit, &QLineEdit::textChanged, this, [=](const QString &txt){manualProxyTextChanged(txt);});
    connect(ui->httpPortLineEdit, &QLineEdit::textChanged, this, [=](const QString &txt){manualProxyTextChanged(txt);});
    connect(ui->httpsPortLineEdit, &QLineEdit::textChanged, this, [=](const QString &txt){manualProxyTextChanged(txt);});
    connect(ui->ftpPortLineEdit, &QLineEdit::textChanged, this, [=](const QString &txt){manualProxyTextChanged(txt);});
    connect(ui->socksPortLineEdit, &QLineEdit::textChanged, this, [=](const QString &txt){manualProxyTextChanged(txt);});

    connect(ui->cetificationBtn, &QPushButton::clicked, [=](bool checked){
        Q_UNUSED(checked)
        showCertificationDialog();
    });
    connect(ui->ignoreHostTextEdit, &QTextEdit::textChanged, this, [=](){
        QString text = ui->ignoreHostTextEdit->toPlainText();
        QStringList hostStringList = text.split(";");
        proxysettings->set(IGNORE_HOSTS_KEY, QVariant(hostStringList));
    });
}

void Proxy::initProxyModeStatus(){
    int mode = _getCurrentProxyMode();

    autoSwitchBtn->blockSignals(true);
    manualSwitchBtn->blockSignals(true);

    if (mode == AUTO){
        autoSwitchBtn->setChecked(true);
    } else if (mode == MANUAL){
        manualSwitchBtn->setChecked(true);
    } else{
        autoSwitchBtn->setChecked(false);
        manualSwitchBtn->setChecked(false);
    }

    autoSwitchBtn->blockSignals(false);
    manualSwitchBtn->blockSignals(false);

    _setSensitivity();
}

void Proxy::initAutoProxyStatus(){

    ui->urlLineEdit->blockSignals(true);
    //设置当前url
    QString urlString = proxysettings->get(PROXY_AUTOCONFIG_URL_KEY).toString();
    ui->urlLineEdit->setText(urlString);

    ui->urlLineEdit->blockSignals(false);
}

void Proxy::initManualProxyStatus(){
    //信号阻塞
    ui->httpHostLineEdit->blockSignals(true);
    ui->httpsHostLineEdit->blockSignals(true);
    ui->ftpHostLineEdit->blockSignals(true);
    ui->socksHostLineEdit->blockSignals(true);

    ui->httpPortLineEdit->blockSignals(true);
    ui->httpsPortLineEdit->blockSignals(true);
    ui->ftpPortLineEdit->blockSignals(true);
    ui->socksPortLineEdit->blockSignals(true);

    //HTTP
    QString httphost = httpsettings->get(PROXY_HOST_KEY).toString();
    ui->httpHostLineEdit->setText(httphost);
    int httpport = httpsettings->get(PROXY_PORT_KEY).toInt();
    ui->httpPortLineEdit->setText(QString::number(httpport));

    //HTTPS
    QString httpshost = securesettings->get(PROXY_HOST_KEY).toString();
    ui->httpsHostLineEdit->setText(httpshost);
    int httpsport = securesettings->get(PROXY_PORT_KEY).toInt();
    ui->httpsPortLineEdit->setText(QString::number(httpsport));

    //FTP
    QString ftphost = ftpsettings->get(PROXY_HOST_KEY).toString();
    ui->ftpHostLineEdit->setText(ftphost);
    int ftppost = ftpsettings->get(PROXY_PORT_KEY).toInt();
    ui->ftpPortLineEdit->setText(QString::number(ftppost));

    //SOCKS
    QString sockshost = sockssettings->get(PROXY_HOST_KEY).toString();
    ui->socksHostLineEdit->setText(sockshost);
    int socksport = sockssettings->get(PROXY_PORT_KEY).toInt();
    ui->socksPortLineEdit->setText(QString::number(socksport));

    //解除信号阻塞
    ui->httpHostLineEdit->blockSignals(false);
    ui->httpsHostLineEdit->blockSignals(false);
    ui->ftpHostLineEdit->blockSignals(false);
    ui->socksHostLineEdit->blockSignals(false);

    ui->httpPortLineEdit->blockSignals(false);
    ui->httpsPortLineEdit->blockSignals(false);
    ui->ftpPortLineEdit->blockSignals(false);
    ui->socksPortLineEdit->blockSignals(false);
}

void Proxy::initIgnoreHostStatus(){
    ui->ignoreHostTextEdit->blockSignals(true);

    //设置当前ignore host
    QStringList ignorehost = proxysettings->get(IGNORE_HOSTS_KEY).toStringList();
    ui->ignoreHostTextEdit->setPlainText(ignorehost.join(";"));

    ui->ignoreHostTextEdit->blockSignals(false);
}

int Proxy::_getCurrentProxyMode(){
    GSettings * proxygsettings;
    proxygsettings = g_settings_new(PROXY_SCHEMA);
    int mode = g_settings_get_enum(proxygsettings, PROXY_MODE_KEY);
    g_object_unref(proxygsettings);

    return mode;
}

void Proxy::_setSensitivity(){
    //自动配置代理界面敏感性
    bool autoChecked = autoSwitchBtn->isChecked();
    ui->urlWidget->setEnabled(autoChecked);

    //手动配置代理界面敏感性
    bool manualChecked = manualSwitchBtn->isChecked();
    ui->httpWidget->setEnabled(manualChecked);
    ui->httpsWidget->setEnabled(manualChecked);
    ui->ftpWidget->setEnabled(manualChecked);
    ui->socksWidget->setEnabled(manualChecked);

}

void Proxy::showCertificationDialog(){
    QDialog * certificationDialog = new CertificationDialog();
    certificationDialog->setAttribute(Qt::WA_DeleteOnClose);
    certificationDialog->show();
}

void Proxy::manualProxyTextChanged(QString txt){
    //获取被修改控件
    QObject * pobject = this->sender();
    QLineEdit * who = dynamic_cast<QLineEdit *>(pobject);

    //获取控件保存的用户数据
    KeyValue * current = (KeyValue *)(who->userData(Qt::UserRole));
    QString schema = current->schema; QString key = current->key;

    //构建临时QGSettings
    const QByteArray id = schema.toUtf8();
    const QByteArray iidd(id.data());
    QGSettings * setting = new QGSettings(iidd);

    setting->set(key, QVariant(txt));

    delete setting;
}

void Proxy::proxyModeChangedSlot(bool checked){
    GSettings * proxygsettings;
    proxygsettings = g_settings_new(PROXY_SCHEMA);

    //两个switchbutton要达到互斥的效果，自定义按钮暂时未支持添加buttongroup
    QObject * object = QObject::sender();
    if (object->objectName() == "auto"){ //区分哪个switchbutton
        if (checked){
            if (manualSwitchBtn->isChecked())
                manualSwitchBtn->setChecked(false);
            g_settings_set_enum(proxygsettings, PROXY_MODE_KEY, AUTO);
        }
        else{
            if (!manualSwitchBtn->isChecked())
                g_settings_set_enum(proxygsettings, PROXY_MODE_KEY, NONE);
        }
    }
    else{
        if (checked){
            if (autoSwitchBtn->isChecked())
                autoSwitchBtn->setChecked(false);
            g_settings_set_enum(proxygsettings, PROXY_MODE_KEY, MANUAL);
        }
        else{
            if (!autoSwitchBtn->isChecked())
                g_settings_set_enum(proxygsettings, PROXY_MODE_KEY, NONE);
        }
    }
    g_object_unref(proxygsettings);

    _setSensitivity();
}
