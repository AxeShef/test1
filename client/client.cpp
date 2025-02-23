#include "client.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

Client::Client(QWidget *parent) : QMainWindow(parent)
{
    serverAddress = "localhost";
    serverPort = 12345;
    
    socket = new QTcpSocket(this);
    
    connect(socket, &QTcpSocket::connected, this, &Client::handleConnected);
    connect(socket, &QTcpSocket::disconnected, this, &Client::handleDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &Client::handleReadyRead);
    connect(socket, &QTcpSocket::errorOccurred, this, &Client::handleError);

    setupUi();
    connectToServer();
}

void Client::setupUi()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    
    treeWidget = new QTreeWidget(this);
    treeWidget->setHeaderLabels({"IP", "Имя", "Описание"});
    
    statusLabel = new QLabel("Статус подключения: Отключено", this);
    
    layout->addWidget(treeWidget);
    layout->addWidget(statusLabel);
    
    resize(800, 600);
}

void Client::connectToServer()
{
    socket->connectToHost(serverAddress, serverPort);
}

void Client::handleConnected()
{
    updateConnectionStatus();
    // Отправляем запрос на получение данных
    socket->write("GET_DATA");
}

void Client::handleDisconnected()
{
    updateConnectionStatus();
}

void Client::handleError(QAbstractSocket::SocketError error)
{
    QString errorStr = "Ошибка подключения: " + socket->errorString();
    statusLabel->setText(errorStr);
    QMessageBox::critical(this, "Ошибка", errorStr);
}

void Client::updateConnectionStatus()
{
    QString status = socket->state() == QAbstractSocket::ConnectedState ?
                    "Подключено" : "Отключено";
    statusLabel->setText("Статус подключения: " + status);
}

void Client::handleReadyRead()
{
    QByteArray jsonData = socket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    
    if (doc.isArray()) {
        QJsonArray array = doc.array();
        treeWidget->clear();
        
        for (const QJsonValue &value : array) {
            QJsonObject obj = value.toObject();
            QTreeWidgetItem *item = new QTreeWidgetItem(treeWidget);
            item->setText(0, obj["ip"].toString());
            item->setText(1, obj["name"].toString());
            item->setText(2, obj["description"].toString());
        }
    }
} 