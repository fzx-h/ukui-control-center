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
#include "changepwddialog.h"
#include "ui_changepwddialog.h"

#include <QStyledItemDelegate>

#define PWD_LOW_LENGTH 6

ChangePwdDialog::ChangePwdDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangePwdDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);

    ui->frame->setStyleSheet("QFrame{background: #ffffff; border: none; border-radius: 6px;}");
    ui->closeBtn->setStyleSheet("QPushButton{background: #ffffff; border: none;}");
    ui->pwdLineEdit->setStyleSheet("QLineEdit{background: #F4F4F4; border: none; border-radius: 4px;}");
    ui->pwdsureLineEdit->setStyleSheet("QLineEdit{background: #F4F4F4; border: none; border-radius: 4px;}");


    //构建Combox代理，否则样式不全部生效
    QStyledItemDelegate * itemDelege = new QStyledItemDelegate();
    ui->pwdtypeComboBox->setItemDelegate(itemDelege);
    ui->pwdtypeComboBox->setMaxVisibleItems(5);
    ui->pwdtypeComboBox->setStyleSheet("QComboBox{background: #F4F4F4; border-radius: 4px; font-size:14px;padding-left: 8px; color: black; min-height: 30px; combobox-popup: 0;}"
                                     "QComboBox::down-arrow{image:url(://img/dropArrow/downpx.png)}"
                                     "QComboBox::drop-down{width: 30px; border: none;}"
                                     "");


    ui->closeBtn->setIcon(QIcon("://img/titlebar/close.png"));


    setupComponent();
    setupConnect();
}

ChangePwdDialog::~ChangePwdDialog()
{
    delete ui;
}

void ChangePwdDialog::setupComponent(){
    ui->pwdtypeComboBox->addItem(tr("General Pwd"));

    ui->pwdLineEdit->setEchoMode(QLineEdit::Password);
    ui->pwdsureLineEdit->setEchoMode(QLineEdit::Password);

    ui->pwdLineEdit->setPlaceholderText(tr("New Password"));
    ui->pwdsureLineEdit->setPlaceholderText(tr("New Password Identify"));

    refreshConfirmBtnStatus();
}

void ChangePwdDialog::setupConnect(){
    connect(ui->closeBtn, &QPushButton::clicked, [=]{
        close();
    });

    connect(ui->pwdLineEdit, &QLineEdit::textChanged, [=](QString text){
        if (text.length() < PWD_LOW_LENGTH)
            ui->tipLabel->setText(tr("Password length needs to more than 5 character!"));
        else
            ui->tipLabel->setText("");

        //防止先输入确认密码，再输入密码后pwdsureLabel无法刷新
        if (ui->pwdLineEdit->text() == ui->pwdsureLineEdit->text())
            ui->pwdsureLabel->setText("");

        refreshConfirmBtnStatus();
    });
    connect(ui->pwdsureLineEdit, &QLineEdit::textChanged, [=](QString text){
        if (text != ui->pwdLineEdit->text())
            ui->tipLabel->setText(tr("Inconsistency with pwd"));
        else
            ui->tipLabel->setText("");

        refreshConfirmBtnStatus();
    });

    connect(ui->cancelPushBtn, &QPushButton::clicked, [=]{
        reject();
    });
    connect(ui->confirmPushBtn, &QPushButton::clicked, [=]{
        this->accept();
        emit passwd_send(ui->pwdLineEdit->text(), ui->usernameLabel->text());
    });
}

void ChangePwdDialog::setFace(QString iconfile){
    ui->faceLabel->setPixmap(QPixmap(iconfile).scaled(QSize(80, 80)));

}

void ChangePwdDialog::setUsername(QString username){
    ui->usernameLabel->setText(username);
}

void ChangePwdDialog::setPwdType(QString type){
    ui->pwdtypeComboBox->setCurrentText(type);
}

void ChangePwdDialog::setAccountType(QString aType){
    ui->aTypeLabel->setText(aType);
}

void ChangePwdDialog::refreshConfirmBtnStatus(){
    if (!ui->tipLabel->text().isEmpty() || \
            ui->pwdLineEdit->text().isEmpty() || ui->pwdLineEdit->text() == tr("New Password") || \
            ui->pwdsureLineEdit->text().isEmpty() || ui->pwdsureLineEdit->text() == tr("New Password Identify"))
        ui->confirmPushBtn->setEnabled(false);
    else
        ui->confirmPushBtn->setEnabled(true);
}
