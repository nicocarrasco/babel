/*
** EPITECH PROJECT, 2020
** B-CPP-500-MAR-5-1-babel-thibault.schmitt
** File description:
** udpNetwork
*/

#ifndef UDPNETWORK_H_
#define UDPNETWORK_H_

#include <QtWidgets/QMainWindow>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QNetworkInterface>
#include <QDebug>
#include <QtNetwork/QHostAddress>
#include "Audio.hpp"
#include "Compressor.hpp"
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>
#include <vector>

class UdpNetwork : public QObject
{

    Q_OBJECT

    public:
        explicit UdpNetwork(QObject *parent = 0);
        ~UdpNetwork();
        void sendStop();
        int getPortListen() const;
        void setPortWrite(quint64);
        void setCall(bool);
        void setIpWrite(QString &);
        void sendTalk();

    signals:
        void stopCall(bool);
        void canTalk(bool);

    public slots:
        void readData();

    private:
        QUdpSocket *socket;
        quint64 portWrite;
        quint64 portListen;
        QHostAddress ipListen;
        QHostAddress ipWrite;
        bool inCall;
        Audio *audio;
        Compressor *codec;
        std::vector<float> _data;
};

#endif /* !UDPNETWORK_H_ */