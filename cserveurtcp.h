#ifndef CSERVEURTCP_H
#define CSERVEURTCP_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include "cprotocoleJson.h"

class CServeurTcp : public QTcpServer
{
    Q_OBJECT
public:
    explicit CServeurTcp(QObject *parent = 0, quint16 noPort = 2222);
    ~CServeurTcp();
    int emettreVersClients(QString mess);

signals:
    void sig_evenementServeur(QString eve);
    void sig_erreur(QAbstractSocket::SocketError err);
    void sig_dataClient(QString adrIpClient, QString data);
    void sig_adrClient(QString adrClient);
    void sig_maJClients(QList<QTcpSocket *> liste);

public slots:
    void on_newConnectionClient();
    void on_disconnectedClient();
    void on_erreurReseau(QAbstractSocket::SocketError err);
    void on_readyReadClient();
    void on_sendJson(QString type, QString param);

private:
    int init();
    quint16 _noPort;
    QList<QTcpSocket *> _listeClients;
    CProtocoleJson _prot;
};

#endif // CSERVEURTCP_H
