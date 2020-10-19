#include "../include/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Client *cl, TcpNetwork *net, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    client(cl),
    tcp(net)
{
    QString clName = client->getName();
    std::vector<std::string> contacts = tcp->getContact(clName);
    for (int i = 0; i < contacts.size(); i++) {
        QString contact = QString::fromStdString(contacts[i]);
        client->addAFriend(contact);
    }
    ui->setupUi(this);
    this->setWindowTitle("Babel");
    this->isCalling = false;
    ui->labelFriendCall->setText("You are not calling anyone.");
    ui->labelFriendCall->setWordWrap(true);
    ui->labelFriendCall->setAlignment(Qt::AlignCenter);
    QIcon icon(":/ressources/images/phone-hang-up.png");
    ui->pushButtonHangUp->setIcon(icon);
    QIcon icon2(":/ressources/images/phone-call.png");
    ui->pushButtonCallDeny->setIcon(icon2);
    ui->pushButtonCallAccept->setIcon(icon2);
    ui->pushButtonHangUp->setIconSize(QSize(40, 40));
    ui->pushButtonHangUp->hide();
    ui->pushButtonCallAccept->hide();
    ui->pushButtonCallDeny->hide();
    ui->pushButtonTalk->hide();
    ui->label_talk->hide();
    udp = new UdpNetwork(this);
    for (int i = 0; i < client->getFriends().size(); i++) {
        QString name = client->getFriends().value(i);
        addAFriend(name);
    }
    tcp->setAcceptCall(true);
    connect(tcp, &TcpNetwork::acceptingCall, this, &MainWindow::beingCalled);
    connect(tcp, &TcpNetwork::hasAcceptedCall, this, &MainWindow::hasRespondCalled);
    connect(udp, &UdpNetwork::stopCall, this, &MainWindow::hasStopCall);
    connect(udp, &UdpNetwork::canTalk, this, &MainWindow::canTalk);
}

MainWindow::~MainWindow()
{
    delete udp;
    delete ui;
}

void MainWindow::hangUp()
{
    int nbFriends = ui->listWidgetFriend->count();

    for (int i = 0; i < nbFriends; i++) {
        QListWidgetItem* item = ui->listWidgetFriend->item(i);
        BabelFriend *babelFriend = qobject_cast<BabelFriend*> (ui->listWidgetFriend->itemWidget(item));
        babelFriend->setCallBtn(true);
    }
    ui->labelFriendCall->setText("You are not calling anyone.");
    ui->pushButtonHangUp->hide();
    this->isCalling = false;
    udp->setCall(false);
    udp->setPortWrite(0);
    ui->pushButtonTalk->hide();
    ui->label_talk->hide();
}

void MainWindow::addAFriend(QString &name)
{
    BabelFriend *newFriend = new BabelFriend(name, this);
    connect(newFriend, &BabelFriend::sendMessage, this, &MainWindow::callSomeone);
    if (this->isCalling)
        newFriend->setCallBtn(false);
    QListWidgetItem *item = new QListWidgetItem;

    item->setSizeHint(QSize(271, 81));
    ui->listWidgetFriend->addItem(item);
    ui->listWidgetFriend->setItemWidget(item, newFriend);
}

void MainWindow::on_pushButtonAddFriend_clicked()
{
    QString name = ui->lineEditAdd->text();
    tcp->setAcceptCall(false);
    if ((tcp->addFriend(name)) == "OK\n") {
        addAFriend(name);
        ui->lineEditAdd->setText("");
    }
    else
        QMessageBox::warning(this, "Add friend", "User doesn't exist.");
    tcp->setAcceptCall(true);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return) {
        if (this->focusWidget()->objectName() == "lineEditAdd") {
            QString name = ui->lineEditAdd->text();
            tcp->setAcceptCall(false);
            if ((tcp->addFriend(name)) == "OK\n") {
                addAFriend(name);
                ui->lineEditAdd->setText("");
            }
            else
                QMessageBox::warning(this, "Add friend", "User doesn't exist.");
    tcp->setAcceptCall(true);
        }
    }
}

void MainWindow::callSomeone(QString &name)
{
    int nbFriends = ui->listWidgetFriend->count();

    for (int i = 0; i < nbFriends; i++) {
        QListWidgetItem* item = ui->listWidgetFriend->item(i);
        BabelFriend *babelFriend = qobject_cast<BabelFriend*> (ui->listWidgetFriend->itemWidget(item));
        babelFriend->setCallBtn(false);
    }
    ui->labelFriendCall->setText(name);
    ui->pushButtonHangUp->show();
    tcp->callFriend(name, udp->getPortListen());
    this->isCalling = true;
}

void MainWindow::on_pushButtonHangUp_clicked()
{
    udp->sendStop();
    hangUp();
}

void MainWindow::on_pushButtonCallAccept_clicked()
{
    int nbFriends = ui->listWidgetFriend->count();

    QString name = ui->labelFriendCall->text();
    tcp->acceptCallFriend(name, true, udp->getPortListen());
    for (int i = 0; i < nbFriends; i++) {
        QListWidgetItem* item = ui->listWidgetFriend->item(i);
        BabelFriend *babelFriend = qobject_cast<BabelFriend*> (ui->listWidgetFriend->itemWidget(item));
        babelFriend->setCallBtn(false);
    }
    ui->pushButtonHangUp->show();
    ui->pushButtonCallAccept->hide();
    ui->pushButtonCallDeny->hide();
    this->isCalling = true;
}

void MainWindow::on_pushButtonCallDeny_clicked()
{
    int nbFriends = ui->listWidgetFriend->count();

    QString name = ui->labelFriendCall->text();
    tcp->acceptCallFriend(name, false, udp->getPortListen());
    for (int i = 0; i < nbFriends; i++) {
        QListWidgetItem* item = ui->listWidgetFriend->item(i);
        BabelFriend *babelFriend = qobject_cast<BabelFriend*> (ui->listWidgetFriend->itemWidget(item));
        babelFriend->setCallBtn(true);
    }
    udp->setCall(false);
    ui->labelFriendCall->setText("You are not calling anyone.");
    ui->pushButtonCallAccept->hide();
    ui->pushButtonCallDeny->hide();
    this->isCalling = false;
}

void MainWindow::beingCalled(QString &usrnameTarget, int port, QString &ip)
{
    int nbFriends = ui->listWidgetFriend->count();

    for (int i = 0; i < nbFriends; i++) {
        QListWidgetItem* item = ui->listWidgetFriend->item(i);
        BabelFriend *babelFriend = qobject_cast<BabelFriend*> (ui->listWidgetFriend->itemWidget(item));
        babelFriend->setCallBtn(false);
    }
    udp->setCall(true);
    udp->setPortWrite(port);
    udp->setIpWrite(ip);
    ui->labelFriendCall->setText(usrnameTarget);
    ui->pushButtonCallAccept->show();
    ui->pushButtonCallDeny->show();
}

void MainWindow::hasRespondCalled(QString &usrnameTarget, bool accept, int port, QString ip)
{
    if (!accept)
        hangUp();
    else {
        udp->setCall(true);
        udp->setPortWrite(port);
        udp->setIpWrite(ip);
        ui->pushButtonTalk->show();
        ui->label_talk->show();
    }
}

void MainWindow::hasStopCall(bool val)
{
    if (val)
        hangUp();
}

void MainWindow::on_pushButtonTalk_clicked()
{
    udp->sendTalk();
    ui->pushButtonTalk->hide();
    ui->label_talk->hide();
}

void MainWindow::canTalk(bool val)
{
    if (val) {
        ui->pushButtonTalk->show();
        ui->label_talk->show();
    }
}