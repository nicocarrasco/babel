#ifndef CLIENT_H
#define CLIENT_H

#include <QtWidgets/QMainWindow>
#include <QUdpSocket>
#include <QTcpSocket>

class Client : public QObject
{

    Q_OBJECT

    public:
        explicit Client(QString &name, QObject *parent = 0);
        QString getName() const;
        QList<QString>getFriends() const;
        void addAFriend(QString &name);

    private:
        QString _name;
        QList<QString> _friends;
};

#endif // CLIENT_H
