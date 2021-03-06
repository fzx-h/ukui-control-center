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
#include "createuserdialog.h"
#include "ui_createuserdialog.h"

#include <QDebug>

#define MOBILE 0
#define PC 1
#define PWD_LOW_LENGTH 6
#define PWD_HIGH_LENGTH 20
#define PIN_LENGTH 6
#define USER_LENGTH 32

CreateUserDialog::CreateUserDialog(QStringList userlist, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateUserDialog),
    usersStringList(userlist)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);

    ui->frame->setStyleSheet("QFrame{background: #ffffff; border: none; border-radius: 6px;}");
    ui->closeBtn->setStyleSheet("QPushButton{background: #ffffff;}");

    ui->usernameLineEdit->setStyleSheet("QLineEdit{background: #F4F4F4; border: none; border-radius: 4px;}");
    ui->pwdLineEdit->setStyleSheet("QLineEdit{background: #F4F4F4; border: none; border-radius: 4px;}");
    ui->pwdsureLineEdit->setStyleSheet("QLineEdit{background: #F4F4F4; border: none; border-radius: 4px;}");

    ui->closeBtn->setIcon(QIcon("://img/titlebar/close.png"));

    ui->pwdTypeComBox->setStyleSheet("QComboBox{background: #F4F4F4; border-radius: 4px; font-size:14px;padding-left: 8px; color: black; min-height: 30px; combobox-popup: 0;}"
                                     "QComboBox::down-arrow{image:url(://img/dropArrow/downpx.png)}"
                                     "QComboBox::drop-down{width: 30px; border: none;}"
                                     "");
    //构建Combox代理，否则样式不全部生效
    itemDelege = new QStyledItemDelegate();
    ui->pwdTypeComBox->setItemDelegate(itemDelege);
    ui->pwdTypeComBox->setMaxVisibleItems(5);


    setupComonpent();
    setupConnect();

}

CreateUserDialog::~CreateUserDialog()
{
    delete ui;
//    delete process;
}

void CreateUserDialog::setupComonpent(){
    ui->pwdLineEdit->setEchoMode(QLineEdit::Password);
    ui->pwdsureLineEdit->setEchoMode(QLineEdit::Password);
//    ui->pinLineEdit->setEchoMode(QLineEdit::Password);
//    ui->pinsureLineEdit->setEchoMode(QLineEdit::Password);

//    QString required = tr("(Required)");
//    QString optional = tr("(Optional)");
//    if (ostype == PC){
//        ui->pwdLabel->setText(required);
//        ui->pwdsurelabel->setText(required);
//        ui->pinLabel->setText(optional);
//        ui->pinsurelabel->setText(optional);
//    }
//    else{
//        ui->pwdLabel->setText(optional);
//        ui->pwdsurelabel->setText(optional);
//        ui->pinLabel->setText(required);
//        ui->pinsurelabel->setText(required);
//    }

    ui->usernameLineEdit->setPlaceholderText(tr("UserName"));
    ui->pwdLineEdit->setPlaceholderText(tr("Password"));
    ui->pwdsureLineEdit->setPlaceholderText(tr("Password Identify"));
//    ui->pinLineEdit->setPlaceholderText(tr("PIN Code"));
//    ui->pinsureLineEdit->setPlaceholderText(tr("PIN Code Identify"));

    ui->pwdTypeComBox->addItem(tr("General Password"));

//    //给radiobtn设置id，id即accoutnType，方便直接返回id值
    ui->buttonGroup->setId(ui->standardRadioBtn, 0);
    ui->buttonGroup->setId(ui->adminRadioBtn, 1);

//    //默认标准用户
    ui->standardRadioBtn->setChecked(true);

    //设置确定按钮
    refreshConfirmBtnStatus();

//    confirm_btn_status_refresh();
}

void CreateUserDialog::setupConnect(){

    connect(ui->closeBtn, &QPushButton::clicked, [=](bool checked){
        Q_UNUSED(checked)
        close();
    });

    connect(ui->usernameLineEdit, &QLineEdit::textChanged, [=](QString text){
        nameLegalityCheck(text);
    });

    connect(ui->pwdLineEdit, &QLineEdit::textChanged, [=](QString text){
        pwdLegalityCheck(text);
    });

    connect(ui->pwdsureLineEdit, &QLineEdit::textChanged, [=](QString text){
        if (text != ui->pwdLineEdit->text())
            ui->tipLabel->setText(tr("Inconsistency with pwd"));
        else
            ui->tipLabel->setText("");

        refreshConfirmBtnStatus();
    });

    //    connect(ui->confirmPushBtn, SIGNAL(clicked(bool)), this, SLOT(confirm_btn_clicked_slot()));
    //    connect(ui->usernameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(name_legality_check_slot(QString)));
    //    connect(ui->pwdLineEdit, SIGNAL(textChanged(QString)), this, SLOT(pwd_legality_check_slot(QString)));
    //    connect(ui->pwdsureLineEdit, SIGNAL(textChanged(QString)), this, SLOT(pwdsure_legality_check_slot(QString)));

    connect(ui->confirmBtn, &QPushButton::clicked, [=](bool checked){
        Q_UNUSED(checked)
        this->accept();
        QString uName, pwd, pin;

        uName = ui->usernameLineEdit->text();
        if (ui->pwdTypeComBox->currentIndex() == 0){
            pwd = ui->pwdLineEdit->text();
            pin = "";
        } else {
            pwd = "";
            pin = ui->pwdLineEdit->text();
        }
        emit newUserWillCreate(uName, pwd, pin, ui->buttonGroup->checkedId());

    });

    connect(ui->cancelBtn, &QPushButton::clicked, [=](bool checked){
        Q_UNUSED(checked)
        reject();
    });
}

//void CreateUserDialog::set_face_label(QString iconfile){
//    ui->faceLabel->setPixmap(QPixmap(iconfile).scaled(QSize(80, 80)));
//}

//void CreateUserDialog::tiplabel_status_refresh(){
//    if (ui->usernametipLabel->text().isEmpty())
//        ui->usernametipLabel->hide();
//    else
//        ui->usernametipLabel->show();

//    if (ui->pwdtipLabel->text().isEmpty())
//        ui->pwdtipLabel->hide();
//    else
//        ui->pwdtipLabel->show();

//    if (ui->pwdsuretipLabel->text().isEmpty())
//        ui->pwdsuretipLabel->hide();
//    else
//        ui->pwdsuretipLabel->show();

//    if (ui->pintipLabel->text().isEmpty())
//        ui->pintipLabel->hide();
//    else
//        ui->pintipLabel->show();

//    if (ui->pinsuretipLabel->text().isEmpty())
//        ui->pinsuretipLabel->hide();
//    else
//        ui->pinsuretipLabel->show();
//}

void CreateUserDialog::refreshConfirmBtnStatus(){
    if (ui->usernameLineEdit->text().isEmpty() ||
            ui->pwdLineEdit->text().isEmpty() ||
            ui->pwdsureLineEdit->text().isEmpty() ||
            !ui->tipLabel->text().isEmpty())
        ui->confirmBtn->setEnabled(false);
    else
        ui->confirmBtn->setEnabled(true);
}


void CreateUserDialog::pwdLegalityCheck(QString pwd){
    if (pwd.length() < PWD_LOW_LENGTH)
        ui->tipLabel->setText(tr("Password length needs to more than %1 character!").arg(PWD_LOW_LENGTH - 1));
    else if (pwd.length() > PWD_HIGH_LENGTH)
        ui->tipLabel->setText(tr("Password length needs to less than %1 character!").arg(PWD_HIGH_LENGTH + 1));
    else
        ui->tipLabel->setText("");

    //防止先输入确认密码，再输入密码后pwdsuretipLabel无法刷新
    if (ui->pwdLineEdit->text() == ui->pwdsureLineEdit->text())
        ui->tipLabel->setText("");

    refreshConfirmBtnStatus();
}

void CreateUserDialog::nameLegalityCheck(QString username){
    if (username.isEmpty())
        ui->tipLabel->setText(tr("The user name cannot be empty"));
    else if (username.startsWith("_") || username.left(1).contains((QRegExp("[0-9]")))){
        ui->tipLabel->setText(tr("The first character must be lowercase letters!"));
    }
    else if (username.contains(QRegExp("[A-Z]"))){
        ui->tipLabel->setText(tr("User name can not contain capital letters!"));
    }
    else if (username.contains(QRegExp("[a-z]")) || username.contains(QRegExp("[0-9]")) || username.contains("_"))
        if (username.length() > 0 && username.length() < USER_LENGTH){
            /*
             * 此处代码需要优化
             */
//            back = false;
//            QString cmd = QString("getent group %1").arg(username);
//            process = new QProcess(this);
//            connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(name_conflict_group_slot()));
//            process->start(cmd);

            if (usersStringList.contains(username)){
                ui->tipLabel->setText(tr("The user name is already in use, please use a different one."));
            }
            else
                ui->tipLabel->setText("");
        }
        else
            ui->tipLabel->setText(tr("User name length need to less than %1 letters!").arg(USER_LENGTH));
    else
        ui->tipLabel->setText(tr("The user name can only be composed of letters, numbers and underline!"));

    refreshConfirmBtnStatus();
}
