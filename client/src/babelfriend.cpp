#include "../include/babelfriend.h"
#include "ui_babelfriend.h"

BabelFriend::BabelFriend(QString &friendName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BabelFriend),
    friendName(friendName)
{
    ui->setupUi(this);
    ui->labelFriendName->setText(friendName);
    QIcon icon(":/ressources/images/phone-call.png");
    ui->pushButtonCall->setIcon(icon);
}

BabelFriend::~BabelFriend()
{
    delete ui;
}

void BabelFriend::on_pushButtonCall_clicked()
{
    emit sendMessage(friendName);
}

void BabelFriend::setCallBtn(bool value)
{
    if (value) {
        ui->pushButtonCall->setEnabled(true);
    }
    else {
       ui->pushButtonCall->setEnabled(false);
    }
}
