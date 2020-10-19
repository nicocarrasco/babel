#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "../include/babelfriend.h"
#include "../include/client.h"
#include <QKeyEvent>
#include <QDebug>
#include <QMessageBox>
#include "../include/tcpNetwork.h"
#include "../include/udpNetwork.h"
#include <string>
#include <iostream>
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Client *client, TcpNetwork *net, QWidget *parent = nullptr);
    ~MainWindow();
    void addAFriend(QString &name);
    void hangUp();

private slots:
    void on_pushButtonAddFriend_clicked();
    void callSomeone(QString &name);
    void beingCalled(QString &usrnameTarget, int port, QString &ip);
    void hasRespondCalled(QString &usrnameTarget, bool accept, int port, QString ip);
    void hasStopCall(bool val);
    void canTalk(bool val);

    void on_pushButtonHangUp_clicked();

    void on_pushButtonCallAccept_clicked();

    void on_pushButtonCallDeny_clicked();

    void on_pushButtonTalk_clicked();

private:
    Ui::MainWindow *ui;
    BabelFriend *anewFriend;
    bool isCalling;
    Client *client;
    UdpNetwork *udp;
    TcpNetwork *tcp;

protected:
    void keyPressEvent(QKeyEvent *);
};

#endif // MAINWINDOW_H
