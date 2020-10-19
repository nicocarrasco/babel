#ifndef BABELFRIEND_H
#define BABELFRIEND_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>
#include <QPixmap>
#include <QDebug>

namespace Ui {
class BabelFriend;
}

class BabelFriend : public QWidget
{
    Q_OBJECT

public:
    explicit BabelFriend(QString &friendName, QWidget *parent = nullptr);
    ~BabelFriend();
    void setCallBtn(bool);

signals:
    void sendMessage(QString &name);

private slots:
    void on_pushButtonCall_clicked();

private:
    Ui::BabelFriend *ui;
    QString friendName;
};

#endif // BABELFRIEND_H
