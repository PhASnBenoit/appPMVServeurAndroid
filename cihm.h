#ifndef CIHM_H
#define CIHM_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include "cserveurtcp.h"

#define PORT 2222

namespace Ui {
class CIhm;
}

class CIhm : public QMainWindow
{
    Q_OBJECT

public:
    explicit CIhm(QWidget *parent = 0);
    ~CIhm();

private slots:
    void on_pbEnvoyer_clicked();
    void on_evenementServeur(QString eve);
    void on_dataRecu(QString adrIpClient, QString data);
    void on_erreurServeur(QAbstractSocket::SocketError err);
    void on_adrClient(QString adrClient);
    void on_listeMaJClients(QList<QTcpSocket *> liste);
    void on_pbStart_clicked();
    void on_pbBtnCsv_clicked();
    void on_pbTransferRunner_clicked();
    void on_pbBtnState_clicked();

    void on_pbGetControl_clicked();

signals:
    void sig_sendJson(QString type, QString param);

private:
    Ui::CIhm *ui;
    CServeurTcp *_serv;
};

#endif // CIHM_H
