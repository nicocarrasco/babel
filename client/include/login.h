#ifndef LOGIN_H
#define LOGIN_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>
#include "../include/mainwindow.h"
#include "../include/client.h"
#include "../include/tcpNetwork.h"
#include <QKeyEvent>
#include <QDebug>
#include <string>
#include <iostream>

namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT

public:
    explicit Login(QString &port, QString &ip, QWidget *parent = nullptr);
    ~Login();
    void onLogin(QString &usrname, QString &password);
    void onSignUp(QString &usrname, QString &password, QString &confirmPassword);

private slots:
    void on_pushButtonLogin_clicked();

    void on_pushButtonSignUp_clicked();

protected:
    void keyPressEvent(QKeyEvent *);

private:
    Ui::Login *ui;
    MainWindow *mainWindow;
    QString port;
    QString ip;
};

#endif // LOGIN_H
