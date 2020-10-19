/*
** EPITECH PROJECT, 2020
** B-CPP-500-MAR-5-1-babel-thibault.schmitt
** File description:
** tcpNetwork
*/

#ifndef TCPNETWORK_H_
#define TCPNETWORK_H_

#include <QtWidgets/QMainWindow>
#include <QtNetwork/QTcpSocket>
#include <QDebug>
#include <string>
#include <iostream>
#include <vector>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QHostAddress>

class TcpNetwork : public QObject
{

    Q_OBJECT

    public:
        explicit TcpNetwork(QString &port, QString &ip, QObject *parent = 0);
        ~TcpNetwork();
        std::string loginTo(QString &usrname, QString &password);
        std::string signUp(QString &usrname, QString &password);
        std::vector<std::string> getContact(QString &usrname);
        std::string callFriend(QString &usrnameTarget, int port);
        std::string acceptCallFriend(QString &usrname, bool accept, int port);
        std::string addFriend(QString &usrnameTarget);
        void setAcceptCall(bool value);

    signals:
        void acceptingCall(QString &usrnameTarget, int port, QString &ip);
        void hasAcceptedCall(QString &usrnameTarget, bool accept, int port = 0, QString ip = nullptr);

    public slots:
        void handleResponse();

    private:
        QTcpSocket *socket;
        bool acceptCall;
};

#endif /* !TCPNETWORK_H_ */
