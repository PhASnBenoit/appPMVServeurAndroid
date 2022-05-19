#include "cprotocoleJson.h"

CProtocoleJson::CProtocoleJson(QObject *parent) : QObject(parent)
{
} // method

QString CProtocoleJson::prepareJsonTransferRunner(int idCourse, QList<int> idRunners)
{
    QString json;
    json = addEnteteJson("transferRunner");
    json += addSectionJson("data");
    json += addParamIntJson("id", idCourse, 4);
    json += addParamTableauJson("runners", idRunners, 4, false);
    json += addPiedJson(2);
    return json;
} // method

//
//  PARSER
//
bool CProtocoleJson::parseJsonTransferAllRunners(int *nbCoureurs, QList<T_COUREUR> *coureurs)
{
    return true;
}

bool CProtocoleJson::parseJsonBtnState(QString trame, T_BTN_STATE *state)
{
    if (trame.indexOf("btnState")<0)
        return false;
    QStringList elements = trame.split(','); // 6 éléments
    state->btnSess = (elements[1].split(':')[2].indexOf('1')>0?true:false);
    state->btnPrep = (elements[2].split(':')[1].indexOf('1')>0?true:false);
    state->btnAVM = (elements[3].split(':')[1].indexOf('1')>0?true:false);
    state->btnReady = (elements[4].split(':')[1].indexOf('1')>0?true:false);
    state->btnGo = (elements[5].split(':')[1].indexOf('1')>0?true:false);
    return true;
} // method

bool CProtocoleJson::parseJsonTimeRun(int *idCourse, T_TIMERUN *timeRun)
{
    return true;
} // method

bool CProtocoleJson::parseJsonTransferRunner(int *idCourse, QList<QString> *idRunners)
{
    return true;
} // method

bool CProtocoleJson::parseJsonAuthCheck(QString trame, QString &login, QString &mdp)
{
    if (trame.indexOf("authCheck")<0)
        return false;
    QStringList elements = trame.split(',');
    login = elements[1].split(':')[2].split('"')[1];
    mdp = elements[2].split(':')[1].split('"')[1];
    return true;
} // method

bool CProtocoleJson::parseJsonGetCsv(QString trame)
{
    // retourne -1 si pas bon sinon >= 0
    return trame.indexOf("getCsv");
} // method

//
//  PREPARE
//
QString CProtocoleJson::prepareJsonTimeRun(int idCourse, T_TIMERUN timeRun)
{
    QString json;
    json = addEnteteJson("timeRun");
    json += addSectionJson("data");
    json += addParamTexteJson("id", QString::number(idCourse), 4);
    json += addParamTexteJson("time1", timeRun.time1, 4);
    json += addParamTexteJson("wind1", timeRun.wind1, 4);
    json += addParamTexteJson("speed1", timeRun.speed1, 4);
    json += addParamTexteJson("time2", timeRun.time2, 4);
    json += addParamTexteJson("wind2", timeRun.wind2, 4);
    json += addParamTexteJson("speed2", timeRun.speed2, 4, false);
    json += addPiedJson(2);
    return json;
} // method


QString CProtocoleJson::prepareJsonTransfertCsv(int nbCoureurs, T_CSV *csv)
{
    QString json;
    json = addEnteteJson("transferCsv");
    json += addSectionJson("data");
    json += addParamTexteJson("runnersCnt", QString::number(nbCoureurs), 4);
    for (int i=0 ; i<nbCoureurs ; i++) {
        json += addSectionJson("runner"+QString::number(i+1), 4);
          json += addParamTexteJson("idRun", QString::number(csv[i].idRun),6);
          json += addParamTexteJson("name", csv[i].name,6);
          json += addParamTexteJson("time", csv[i].time,6);
          json += addParamTexteJson("wind", csv[i].wind,6);
          json += addParamTexteJson("speed", csv[i].speed,6, false);
        json += addFinSectionJson(4, (i<(nbCoureurs-1)?true:false));
    } // for
    json += addPiedJson(2);
    return json;
} // method

QString CProtocoleJson::prepareJsonAuthCheck(bool res)
{
    QString json;
    json = addEnteteJson("authCheck");
    json += addSectionJson("data");
    json += addParamIntJson("success",(res?'1':'0'), 4,false);
    json += addPiedJson(2);
    return json;
} // method

QString CProtocoleJson::prepareJsonBtnState(T_BTN_STATE state)
{
    QString json;
    json = addEnteteJson("btnState");
    json += addSectionJson("data");
      json += addParamTexteJson("btnSess", QString::number(state.btnSess) , 4);
      json += addParamTexteJson("btnPrep", QString::number(state.btnPrep) ,4);
      json += addParamTexteJson("btnAVM", QString::number(state.btnAVM),4);
      json += addParamTexteJson("btnReady",  QString::number(state.btnReady),4);
      json += addParamTexteJson("btnGo",  QString::number(state.btnGo),4, false);
    json += addPiedJson(2);
    return json;
}

QString CProtocoleJson::prepareJsonGetControl()
{
    QString json;
    json = addEnteteJson("getControl", false);
    json += addPiedJson(1);
    return json;
} // method

//
// Méthodes private pour former une trame JSON
//
QString CProtocoleJson::addEnteteJson(QString commande, bool suite)
{
    QString json;
    json.append("{\"command\": \""+commande+"\"");
    if (suite)
        json.append(",");
    return json;
} // method

QString CProtocoleJson::addPiedJson(int nb)
{
    QString json;
    if (nb == 2)
        json.append("  }");
    json.append("}");
    return json;
} // method

QString CProtocoleJson::addSectionJson(QString nom, int dec)
{
    QString json;
    json.fill(' ', dec);
    json.append("\""+nom+"\": {");
    return json;
} // method

QString CProtocoleJson::addFinSectionJson(int dec, int suite)
{
    QString json;
    json.fill(' ', dec);
    json.append("}");
    if (suite)
        json.append(",");
    return json;
} // method

QString CProtocoleJson::addParamTexteJson(QString nom, QString valeur, int dec, bool suite)
{
    QString json;
    json.fill(' ', dec);
    json.append("\""+nom+"\": \""+valeur+"\"");
    if (suite)
        json.append(",");
    return json;
} // method

QString CProtocoleJson::addParamIntJson(QString nom, int valeur, int dec, bool suite)
{
    QString json;
    json.fill(' ', dec);
    json.append("\""+nom+"\": "+valeur);
    if (suite)
        json.append(",");
    return json;
} // method

QString CProtocoleJson::addParamTableauJson(QString nom, QList<int> valeur, int dec, bool suite)
{
    QString json;
    json.fill(' ', dec);
    json.append("\""+nom+"\": [");
    for (int i=0 ; i<valeur.size() ; i++) {
        json.fill(' ', dec+2);
        json.append(valeur.at(i));
        if (i < valeur.size()-1)
            json.append(",");
    } // for
    json.fill(' ', dec);
    json.append("]");
    if (suite)
        json.append(",");
    return json;
}

QString CProtocoleJson::filtreAvantParser(QString trame)
{
    int pos;
    do {
        pos = trame.indexOf("\r\n");
        if (pos >= 0)
            trame.remove(pos, 2);

    } while(pos >= 0);
    do {
        pos = trame.indexOf("\n");
        if (pos >= 0)
            trame.remove(pos, 1);
    } while(pos >= 0);
    do {
        pos = trame.indexOf("\r");
        if (pos >= 0)
            trame.remove(pos, 1);
    } while(pos >= 0);
    return trame;
} // method
