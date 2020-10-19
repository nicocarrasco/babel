/*
** EPITECH PROJECT, 2020
** B-CPP-500-MAR-5-1-babel-thibault.schmitt
** File description:
** tcpNetwork
*/

#include "../include/tcpNetwork.h"

TcpNetwork::TcpNetwork(QString &port, QString &ip, QObject *parent) : QObject(parent)
{
    bool ok;
    int portServer = port.toInt(&ok);
    QHostAddress ipServer(ip);
    socket = new QTcpSocket(this);
    socket->connectToHost(ipServer, portServer);
    socket->waitForConnected(1000);
    connect(socket, &QTcpSocket::readyRead, this, &TcpNetwork::handleResponse);
    acceptCall = false;
}

TcpNetwork::~TcpNetwork()
{

}

std::string TcpNetwork::loginTo(QString &usrname, QString &password)
{
    QByteArray reponse;

    if (socket->state() != QTcpSocket::ConnectedState)
        return ("Error : Can't connect");
    socket->write("login " + usrname.toLocal8Bit() + " " + password.toLocal8Bit() + "\r");
    if (socket->waitForBytesWritten(1000) && socket->waitForReadyRead(3000)) {
        reponse = socket->readAll();
        return (reponse.toStdString());
    }
    else
        return ("Error : Can't connect");
}

std::string TcpNetwork::signUp(QString &usrname, QString &password)
{
    QByteArray reponse;

    if (socket->state() != QTcpSocket::ConnectedState)
        return ("Error : Can't connect");
    socket->write("add " + usrname.toLocal8Bit() + " " + password.toLocal8Bit() + "\r");
    if (socket->waitForBytesWritten(1000) && socket->waitForReadyRead(3000)) {
        reponse = socket->readAll();
        return (reponse.toStdString());
    }
    else
        return ("Error : Can't connect");
}

std::vector<std::string> TcpNetwork::getContact(QString &usrname)
{
    QByteArray reponse;
    std::vector<std::string> contacts;

    socket->write("contacts " + usrname.toLocal8Bit() + "\r");
    if (socket->waitForBytesWritten(1000) && socket->waitForReadyRead(3000)) {
        reponse = socket->readAll();
        std::vector<std::string> words;
        std::string s = reponse.toStdString();
        for (char c: "\n")
            s.erase(std::remove(s.begin(), s.end(), c), s.end());
        boost::split(words, s, boost::is_any_of(";"), boost::token_compress_on);
        std::string toErase = "SECOND_USERNAME=";
        for (int i = 0; i < words.size() && words[i].find(toErase) != std::string::npos; i++) {
            std::string contact = words[i].erase(words[i].find(toErase), toErase.length());
            if (contact != usrname.toStdString()) {
                contacts.push_back(contact);
            }
        }
    }
    return (contacts);
}

std::string TcpNetwork::callFriend(QString &usrnameTarget, int port)
{
    QByteArray reponse;
    QString portStr = QString::number(port);

    socket->write("call " + usrnameTarget.toLocal8Bit() + " " + portStr.toLocal8Bit() + "\r");
    if (socket->waitForBytesWritten(1000) && socket->waitForReadyRead()) {
        reponse = socket->readAll();
        return (reponse.toStdString());
    }
    else
        return ("Error : Can't connect");
}

std::string TcpNetwork::acceptCallFriend(QString &usrname, bool accept, int port)
{
    QByteArray reponse;
    QString portStr = QString::number(port);
    QString choice = accept ? " OK " : " KO ";

        socket->write("resp " + usrname.toLocal8Bit() + choice.toLocal8Bit() + portStr.toLocal8Bit() + "\r");
    if (socket->waitForBytesWritten(1000) && socket->waitForReadyRead()) {
        reponse = socket->readAll();
        return (reponse.toStdString());
    }
    else
        return ("Error : Can't connect");
}

void TcpNetwork::handleResponse()
{
    QByteArray reponse;
    std::string reponseStr;

    if (acceptCall) {
        reponse = socket->readAll();
        reponseStr = reponse.toStdString();
        if (reponseStr.size() >= 5 && reponseStr.substr(0, 5) == "call ") {
            std::vector<std::string> words;
            std::string s = reponse.toStdString();
            for (char c: "\n")
                s.erase(std::remove(s.begin(), s.end(), c), s.end());
            boost::split(words, s, boost::is_any_of(" "), boost::token_compress_on);
            QString usrnameTarget = QString::fromStdString(words[1]);
            QString ip = QString::fromStdString(words[3]);
            emit acceptingCall(usrnameTarget, std::stoi(words[2]), ip);
        }
        else if (reponseStr.size() >= 5 && reponseStr.substr(0, 5) == "resp ") {
            std::vector<std::string> words;
            std::string s = reponse.toStdString();
            for (char c: "\n")
                s.erase(std::remove(s.begin(), s.end(), c), s.end());
            boost::split(words, s, boost::is_any_of(" "), boost::token_compress_on);
            QString usrnameTarget = QString::fromStdString(words[1]);
            bool accept = QString::fromStdString(words[2]) == "OK" ? true : false;
            QString ip = QString::fromStdString(words[4]);
            emit hasAcceptedCall(usrnameTarget, accept, std::stoi(words[3]), ip);
        }
    }
}

std::string TcpNetwork::addFriend(QString &usrnameTarget)
{
    QByteArray reponse;

    socket->write("addContact " + usrnameTarget.toLocal8Bit() + "\r");
    if (socket->waitForBytesWritten(1000) && socket->waitForReadyRead(3000)) {
        reponse = socket->readAll();
        return (reponse.toStdString());
    }
    else
        return ("Error");
}

void TcpNetwork::setAcceptCall(bool value)
{
    acceptCall = value;
}