#ifndef CPROTOCOLE_H
#define CPROTOCOLE_H

#include <QObject>
#include <QString>
#include <QList>

#define MAX_CHAR 25

typedef struct sCoureur {
    int id;
    char nom[MAX_CHAR]; // longueur estimée
} T_COUREUR;

typedef struct sTimeRun {
    char time1[MAX_CHAR];
    char wind1[MAX_CHAR];
    char speed1[MAX_CHAR];
    char time2[MAX_CHAR];
    char wind2[MAX_CHAR];
    char speed2[MAX_CHAR];
} T_TIMERUN;

typedef struct sCsv {
    int idRun;
    char name[MAX_CHAR];
    char time[MAX_CHAR];
    char wind[MAX_CHAR];
    char speed[MAX_CHAR];
} T_CSV;

typedef struct sBtnState {
    bool btnSess;
    bool btnPrep;
    bool btnAVM;
    bool btnReady;
    bool btnGo;
} T_BTN_STATE;


class CProtocoleJson : public QObject
{
    Q_OBJECT
public:
    explicit CProtocoleJson(QObject *parent = nullptr);

    // Méthodes pour préparer une trame de protocole
    QString prepareJsonTransferRunner(int idCourse, QList<int> idRunners); // bi-directionnel
    QString prepareJsonTimeRun(int idCourse, T_TIMERUN timeRun); // bi-dir
    QString prepareJsonTransfertCsv(int nbCoureurs, T_CSV *csv); // vers la tablette
    QString prepareJsonAuthCheck(bool res=false);  // vers tablette
    QString prepareJsonBtnState(T_BTN_STATE state); // bi dir
    QString prepareJsonGetControl(); // bi
    // Méthodes pour parser les trames JSON arrivant du terminal
    bool parseJsonGetCsv(QString trame);  // vers le serveur
    bool parseJsonTransferAllRunners(int *nbCoureurs, QList<T_COUREUR>*coureurs); // vers serveur
    bool parseJsonBtnState(QString trame, T_BTN_STATE *state); // bi dir
    bool parseJsonTimeRun(int *idCourse, T_TIMERUN *timeRun); // bi-dir
    bool parseJsonTransferRunner(int *idCourse, QList<QString> *idRunners); // bi-directionnel
    bool parseJsonAuthCheck(QString trame, QString &login, QString &mdp);  // vers tablette

private:
    // méthodes internes pour former une trame JSON
    QString addEnteteJson(QString commande="command", bool suite=true);
    QString addPiedJson(int nb = 1);
    QString addSectionJson(QString nom="section", int dec=2);
    QString addFinSectionJson(int dec=2, int suite=true);
    QString addParamTexteJson(QString nom="param", QString valeur="0", int dec=2, bool suite=true);
    QString addParamIntJson(QString nom="param", int valeur=0, int dec=2, bool suite=true);
    QString addParamTableauJson(QString nom, QList<int> valeur, int dec=2, bool suite=true);
    QString filtreAvantParser(QString trame);

signals:

};

#endif // CPROTOCOLE_H
