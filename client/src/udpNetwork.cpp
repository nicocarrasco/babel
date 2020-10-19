/*
** EPITECH PROJECT, 2020
** B-CPP-500-MAR-5-1-babel-thibault.schmitt
** File description:
** udpNetwork
*/

#include "../include/udpNetwork.h"

UdpNetwork::UdpNetwork(QObject *parent) : QObject(parent)
{
    inCall = false;
    socket = new QUdpSocket(this);
    for (const QHostAddress addr: QNetworkInterface::allAddresses()) {
        if (addr.protocol() == QAbstractSocket::IPv4Protocol && addr != QHostAddress(QHostAddress::LocalHost))
            ipListen = addr;
    }
    // if (ipListen == nullptr)
    //     ipListen = QHostAddress::LocalHost;
    std::srand(std::time(nullptr));
    portListen = (std::rand() % (9000 - 2000 + 1)) + 2000;
    while (!(socket->bind(ipListen, portListen)))
        portListen = (std::rand() % (9000 - 2000 + 1)) + 2000;
    codec = new Compressor;
    connect(socket, &QUdpSocket::readyRead, this, &UdpNetwork::readData);
}

UdpNetwork::~UdpNetwork()
{
}

void UdpNetwork::sendStop()
{
    QByteArray data;

    data.append("stopCall");
    socket->writeDatagram(data, ipWrite, portWrite);
}

void UdpNetwork::readData()
{
    QByteArray buff;
    QHostAddress sender;
    quint16 senderPort;
    std::string resp;

    buff.resize(socket->pendingDatagramSize());
    socket->readDatagram(buff.data(), buff.size(), &sender, &senderPort);
    resp = buff.data();
    if (resp.size() >= 8 && resp == "stopCall") {
        emit stopCall(true);
    }
    else if (resp.size() >= 8 && resp == "stopData") {
        audio = new Audio;
        paData data;
        data.idx = 0;
        data.max = audio->getAllFrames();
        data.rec = _data.data();
        audio->startStreamOutput(data);
        audio->isStreaming(100);
        audio->stopStream();
        delete audio;
        _data.clear();
        emit canTalk(true);
    }
    else {
        float decode[480];
        int decBytes = codec->decodeData((unsigned char *)buff.data(), buff.size(), decode);
        for (int i = 0; i < decBytes; i++)
            _data.push_back(decode[i]);
    }
}

int UdpNetwork::getPortListen() const
{
    return portListen;
}

void UdpNetwork::setPortWrite(quint64 port)
{
    portWrite = port;
}

void UdpNetwork::setCall(bool val)
{
    inCall = val;
}

void UdpNetwork::setIpWrite(QString &ip)
{
    QHostAddress adrr(ip);
    ipWrite = adrr;
}

void UdpNetwork::sendTalk()
{
    audio = new Audio;
    audio->startStreamInput();
    audio->isStreaming(1000);
    audio->stopStream();
    int AllFrames = audio->getAllFrames();
    int len = 0;
    for (int i = 0; i < AllFrames; i+= 480) {
        unsigned char buff[480];
        int nbBytes;
        int size = AllFrames - i < 480 ? AllFrames - i : 480;
        nbBytes = codec->encodeData(&(audio->getData().rec)[i], size * 2, buff);
        socket->writeDatagram((const char *)buff, nbBytes, ipWrite, portWrite);
        len += nbBytes;
    }
    delete audio;
    QByteArray stop;

    stop.append("stopData");
    socket->writeDatagram(stop, ipWrite, portWrite);
}