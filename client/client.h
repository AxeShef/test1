#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTreeWidget>
#include <QLabel>

class Client : public QMainWindow
{
    Q_OBJECT
public:
    explicit Client(QWidget *parent = nullptr);

private slots:
    void connectToServer();
    void handleConnected();
    void handleDisconnected();
    void handleError(QAbstractSocket::SocketError error);
    void handleReadyRead();

private:
    void setupUi();
    void updateConnectionStatus();

    QTcpSocket *socket;
    QTreeWidget *treeWidget;
    QLabel *statusLabel;
    QString serverAddress;
    int serverPort;
};

#endif // CLIENT_H 