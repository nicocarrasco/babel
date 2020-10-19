#include "../include/client.h"

Client::Client(QString &name, QObject *parent) : QObject(parent), _name(name)
{

}

QString Client::getName() const
{
    return _name;
}

QList<QString> Client::getFriends() const
{
    return _friends;
}

void Client::addAFriend(QString &name)
{
    _friends.append(name);
}