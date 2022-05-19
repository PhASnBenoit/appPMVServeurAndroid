#include "cserveurtcp.h"

CServeurTcp::CServeurTcp(QObject *parent, quint16 noPort) :
    QTcpServer(parent)
{
    _noPort = noPort;

    init();
}

CServeurTcp::~CServeurTcp()
{
    // destruction optionnelle car déjà pris en charge par le serveur
    for (int i=0 ; i<_listeClients.size() ; i++) {
        _listeClients.at(i)->close();
        if (_listeClients.at(i)->isValid()) delete _listeClients.at(i);
    } // for i
}

int CServeurTcp::emettreVersClients(QString mess)
{
    for (int i=0 ; i<_listeClients.size() ; i++) {
        _listeClients.at(i)->write(mess.toStdString().c_str());
        _listeClients.at(i)->write("\r\n");
        _listeClients.at(i)->flush();
        qDebug() << "Envoi vers " << _listeClients.at(i);
    } // for i
    return 1;
}

int CServeurTcp::init()
{
    listen(QHostAddress::Any, _noPort);
    connect(this,&CServeurTcp::newConnection, this, &CServeurTcp::on_newConnectionClient);
    return 1;
}

///////////////////  SLOTs ////////////////////////

void CServeurTcp::on_newConnectionClient()
{
    QString mess="Un client vient de se connecter";
    qDebug() << mess;
    QTcpSocket *newClient = this->nextPendingConnection();
    qDebug() << "Nouvelle connexion : " << newClient;
    if (newClient == NULL)
        emit sig_erreur(QAbstractSocket::ConnectionRefusedError);
    connect(newClient, &QTcpSocket::readyRead, this, &CServeurTcp::on_readyReadClient);
    //connect(newClient, &QAbstractSocket::disconnected, this, &CServeurTcp::deleteLater);
    connect(newClient, &QAbstractSocket::disconnected, this, &CServeurTcp::on_disconnectedClient);
    connect(newClient, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
          [=](QAbstractSocket::SocketError err)
    {
        QString txt;
        switch(err) {
        case QAbstractSocket::ConnectionRefusedError:
            txt="The connection was refused by the peer (or timed out).";
            break;
        case QAbstractSocket::RemoteHostClosedError:
            txt="The remote host closed the connection. Note that the client socket (i.e., this socket) will be closed after the remote close notification has been sent.";
            break;
        case QAbstractSocket::HostNotFoundError:
            txt="The host address was not found.";
            break;
        case QAbstractSocket::SocketAccessError:
            txt="The socket operation failed because the application lacked the required privileges.";
            break;
        case QAbstractSocket::SocketResourceError:
            txt="The local system ran out of resources (e.g., too many sockets).";
            break;
        case QAbstractSocket::SocketTimeoutError:
            txt="The socket operation timed out.";
            break;
        case QAbstractSocket::DatagramTooLargeError:
            txt="The datagram was larger than the operating system's limit (which can be as low as 8192 bytes).";
            break;
        case QAbstractSocket::NetworkError:
            txt="An error occurred with the network (e.g., the network cable was accidentally plugged out).";
            break;
        case QAbstractSocket::AddressInUseError:
            txt="The address specified to QAbstractSocket::bind() is already in use and was set to be exclusive.";
            break;
        case QAbstractSocket::SocketAddressNotAvailableError:
            txt="The address specified to QAbstractSocket::bind() does not belong to the host.";
            break;
        case QAbstractSocket::UnsupportedSocketOperationError:
            txt="The requested socket operation is not supported by the local operating system (e.g., lack of IPv6 support).";
            break;
        case QAbstractSocket::ProxyAuthenticationRequiredError:
            txt="The socket is using a proxy, and the proxy requires authentication.";
            break;
        case QAbstractSocket::SslHandshakeFailedError:
            txt="The SSL/TLS handshake failed, so the connection was closed (only used in QSslSocket)";
            break;
        case QAbstractSocket::UnfinishedSocketOperationError:
            txt="Used by QAbstractSocketEngine only, The last operation attempted has not finished yet (still in progress in the background).";
            break;
        case QAbstractSocket::ProxyConnectionRefusedError:
            txt="Could not contact the proxy server because the connection to that server was denied";
            break;
        case QAbstractSocket::ProxyConnectionClosedError:
            txt="The connection to the proxy server was closed unexpectedly (before the connection to the final peer was established)";
            break;
        case QAbstractSocket::ProxyConnectionTimeoutError:
            txt="The connection to the proxy server timed out or the proxy server stopped responding in the authentication phase.";
            break;
        case QAbstractSocket::ProxyNotFoundError:
            txt="The proxy address set with setProxy() (or the application proxy) was not found.";
            break;
        case QAbstractSocket::ProxyProtocolError:
            txt="The connection negotiation with the proxy server failed, because the response from the proxy server could not be understood.";
            break;
        case QAbstractSocket::OperationError:
            txt="An operation was attempted while the socket was in a state that did not permit it.";
            break;
        case QAbstractSocket::SslInternalError:
            txt="The SSL library being used reported an internal error. This is probably the result of a bad installation or misconfiguration of the library.";
            break;
        case QAbstractSocket::SslInvalidUserDataError:
            txt="Invalid data (certificate, key, cypher, etc.) was provided and its use resulted in an error in the SSL library.";
            break;
        case QAbstractSocket::TemporaryError:
            txt="A temporary error occurred (e.g., operation would block and socket is non-blocking).";
            break;
        case QAbstractSocket::UnknownSocketError:
            txt="An unidentified error occurred.";
            break;
        } // sw
        qDebug() << txt;
    });

    _listeClients.append(newClient);  // sauve l'adresse de l'objet dans la liste
    emit sig_evenementServeur("CON");
    emit sig_adrClient(newClient->peerAddress().toString());
    emit sig_maJClients(_listeClients);  // pour IHM
}

void CServeurTcp::on_disconnectedClient()
{
    QTcpSocket *client = (QTcpSocket *)sender(); // Déterminer quel client ?
    _listeClients.removeOne(client);
    //delete client;
    emit sig_evenementServeur("Déconnexion du client");
    emit sig_maJClients(_listeClients);
}

void CServeurTcp::on_erreurReseau(QAbstractSocket::SocketError err)
{
    qDebug() << "Erreur réseau !";
    emit sig_erreur(err);
}

void CServeurTcp::on_readyReadClient()
{
    QByteArray ba;
    QString trame;
    QString command;
    // Déterminer quel client ?
    QTcpSocket *client = (QTcpSocket *)sender();

    ba=client->readAll();

    if ( (ba.indexOf(':')>=0) && (ba.indexOf('"')>=0) )
        command = ba.split(':')[1].split('"')[1];
    else
        return;

    if (command == "authCheck") {
        QString login, mdp;
        if (_prot.parseJsonAuthCheck(QString(ba), login, mdp)) {
            trame = _prot.prepareJsonAuthCheck(true); // pas d'authentification
            client->write(trame.toStdString().c_str()); //réponse
            client->write("\r\n"); //réponse
            emit sig_dataClient("toto", trame);
        } // if
    } // if


    if (command == "getCsv") {
        if (_prot.parseJsonGetCsv(QString(ba))) {
            T_CSV csv[40] = {
                {1, "coureur1", "00:03:01.230", "5.6 km/h", "4.9 km/h"},
                {2, "coureur2", "00:03:01.230", "5.6 km/h", "4.9 km/h"},
                {3, "coureur3", "00:03:01.230", "5.6 km/h", "4.9 km/h"},
                {4, "coureur4", "00:03:01.230", "5.6 km/h", "4.9 km/h"},
                {5, "coureur5", "00:03:01.230", "5.6 km/h", "4.9 km/h"},
                {6, "coure ur6", "00:03:01.230", "5.6 km/h", "4.9 km/h"},
                {7, "coureur7", "00:03:01.230", "5.6 km/h", "4.9 km/h"},
                {8, "coureur8", "00:03:01.230", "5.6 km/h", "4.9 km/h"},
                {9, "coureur9", "00:03:01.230", "5.6 km/h", "4.9 km/h"},
                {10, "coureur10", "00:03:01.230", "5.6 km/h", "4.9 km/h"},
                {10, "coureur11", "00:03:01.230", "5.6 km/h", "4.9 km/h"},
                {12, "coureur12", "00:03:01.230", "5.6 km/h", "4.9 km/h"},
                {13, "coureur13", "00:03:01.230", "5.6 km/h", "4.9 km/h"},
                {14, "coureur14", "00:03:01.230", "5.6 km/h", "4.9 km/h"}
            };
            trame = _prot.prepareJsonTransfertCsv(14, csv);
            client->write(trame.toStdString().c_str()); //réponse
            client->write("\r\n"); //réponse
        } // if
    } // if

    if (command == "btnState") {
        T_BTN_STATE state;
        if (_prot.parseJsonBtnState(QString(ba), &state)) {
            // TODO
        } // if
    } // if

    if (command == "getControl") {
        // TODO
        emit sig_dataClient(client->peerAddress().toString(), "La tablette reprend le contrôle !");
    } // if

    qDebug() << "Client : " << client << ba;
    emit sig_dataClient(client->peerAddress().toString(), QString(ba));
}

void CServeurTcp::on_sendJson(QString type, QString param)
{
    QString trame;

    if (type == "getControl") {
        trame = _prot.prepareJsonGetControl();
        emettreVersClients(trame);
    } // getControl

    if (type == "btnState") {
        T_BTN_STATE state;
        state.btnSess = param.toUInt()&0x01;
        state.btnPrep = param.toUInt()&0x02;
        state.btnAVM = param.toUInt()&0x04;
        state.btnReady = param.toUInt()&0x08;
        state.btnGo = param.toUInt()&0x10;
        trame = _prot.prepareJsonBtnState(state);
        emettreVersClients(trame);
    } // if btnState

} // method

