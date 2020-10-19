#include "../include/login.h"
#include "ui_login.h"

Login::Login(QString &port, QString &ip, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login),
    port(port),
    ip(ip)
{
    ui->setupUi(this);
    this->setWindowTitle("Babel");
    setFocusPolicy (Qt::StrongFocus);
}

Login::~Login()
{
    delete ui;
}

void Login::onLogin(QString &usrname, QString &password)
{
    TcpNetwork *net = new TcpNetwork(port, ip, this);
    std::string response= "Error : No password or user.";

    if (password != "" && usrname != "" &&
    (response = net->loginTo(usrname, password)) == "OK\n") {
        hide();
        Client *client = new Client(usrname, this);
        mainWindow = new MainWindow(client, net, this);
        mainWindow->show();
      }
      else {
        QMessageBox::warning(this, "Login", QString::fromStdString(response));
        delete net;
    }
}

void Login::onSignUp(QString &usrname, QString &password, QString &confirmPassword)
{
    TcpNetwork *net = new TcpNetwork(port, ip, this);
    std::string response = "Error : Not the same password.";

    if (password != "" && password == confirmPassword && usrname != "" &&
    (response = net->signUp(usrname, password)) == "OK\n" &&
    (response = net->loginTo(usrname, password)) == "OK\n") {
        hide();
        Client *client = new Client(usrname, this);
        mainWindow = new MainWindow(client, net, this);
        mainWindow->show();
      }
      else {
        QMessageBox::warning(this, "Sign up", QString::fromStdString(response));
        delete net;
    }
}

void Login::on_pushButtonLogin_clicked()
{
    QString usrname = ui->lineEditUsrName->text();
    QString password = ui->lineEditPassword->text();

    onLogin(usrname, password);
}

void Login::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return) {
        if ((this->focusWidget()->objectName()).indexOf("SignUp") == -1) {
            QString usrname = ui->lineEditUsrName->text();
            QString password = ui->lineEditPassword->text();
            onLogin(usrname, password);
        }
        else {
            QString usrname = ui->lineEditSignUpUsrName->text();
            QString password = ui->lineEditSignUpPassword->text();
            QString confirmPassword = ui->lineEditSignUpConfirmPassword->text();
            onSignUp(usrname, password, confirmPassword);
        }
    }
}

void Login::on_pushButtonSignUp_clicked()
{
    QString usrname = ui->lineEditSignUpUsrName->text();
    QString password = ui->lineEditSignUpPassword->text();
    QString confirmPassword = ui->lineEditSignUpConfirmPassword->text();

    onSignUp(usrname, password, confirmPassword);
}
