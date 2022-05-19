#include "cihm.h"
#include "ui_cihm.h"

CIhm::CIhm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CIhm)
{
    ui->setupUi(this);
    ui->pbEnvoyer->setEnabled(false);
}

CIhm::~CIhm()
{
    delete _serv;
    delete ui;
}

void CIhm::on_pbEnvoyer_clicked()
{
    if (!ui->cbSautLigne->isChecked())
        _serv->emettreVersClients(ui->leTexte->text());
    else
        _serv->emettreVersClients(ui->leTexte->text()+"\r\n");
}

void CIhm::on_evenementServeur(QString eve)
{
    if (eve=="DEC") {
        ui->teTexte->append("Déconnexion d'un client.");
    }
    if (eve=="CON") {
        ui->teTexte->append("Connexion d'un client.");
        ui->pbEnvoyer->setEnabled(true);
    }
}

void CIhm::on_dataRecu(QString adrIpClient, QString data)
{
    ui->teTexte->append(adrIpClient+": "+data);
}

void CIhm::on_erreurServeur(QAbstractSocket::SocketError err)
{
    switch (err) {
      case QAbstractSocket::ConnectionRefusedError:
        ui->teTexte->append("Connexion refusée par le serveur !");
      break;
      case QAbstractSocket::NetworkError:
        ui->teTexte->append("Coupure de liaison réseau !");
        break;
      default:
        ui->teTexte->append("Erreur réseau à déterminer !");
      break;
    } // sw
}

void CIhm::on_adrClient(QString adrClient)
{
    ui->teTexte->append(adrClient+" est connecté.");
}

void CIhm::on_listeMaJClients(QList<QTcpSocket *> liste)
{
    ui->cbListe->clear();
    for (int i=0 ; i<liste.size() ; i++) {
        ui->cbListe->addItem(liste.at(i)->peerAddress().toString()+ "->"+QString::number((unsigned long)liste.at(i)));
        qDebug() << "CIhmAppServeurTcp::onListeMaJClients"
                 << liste.at(i)->peerAddress().toString()+ "->"+QString::number((unsigned long)liste.at(i));
    } // for i
    if (liste.size()==0)
        ui->pbEnvoyer->setEnabled(false);
}

void CIhm::on_pbStart_clicked()
{
    ui->pbStart->setEnabled(false);
    _serv = new CServeurTcp(this, ui->lePort->text().toUInt());
    connect(_serv, &CServeurTcp::sig_evenementServeur, this, &CIhm::on_evenementServeur);
    connect(_serv, &CServeurTcp::sig_dataClient, this, &CIhm::on_dataRecu);
    connect(_serv, &CServeurTcp::sig_erreur, this, &CIhm::on_erreurServeur);
    connect(_serv, &CServeurTcp::sig_adrClient, this, &CIhm::on_adrClient);
    connect(_serv, &CServeurTcp::sig_maJClients, this, &CIhm::on_listeMaJClients);
    connect(this, &CIhm::sig_sendJson, _serv, &CServeurTcp::on_sendJson);
}

void CIhm::on_pbBtnCsv_clicked()
{
    emit sig_sendJson("transferCsv", "");
}

void CIhm::on_pbTransferRunner_clicked()
{
    emit sig_sendJson("transferRunner", "");
}


void CIhm::on_pbBtnState_clicked()
{
    int val=0;
    if (ui->rbSess->isChecked()) val+=1;
    if (ui->rbPrep->isChecked()) val+=2;
    if (ui->rbAVM->isChecked()) val+=4;
    if (ui->rbReady->isChecked()) val+=8;
    if (ui->rbGo->isChecked()) val+=16;

    emit sig_sendJson("btnState", QString::number(val));
}

void CIhm::on_pbGetControl_clicked()
{
    emit sig_sendJson("getControl", "");
}
