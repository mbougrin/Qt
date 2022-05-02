#ifndef SERVERHTTP_H
#define SERVERHTTP_H

#include <QString>
#include <QDebug>
#include <QTcpServer>
#include <QtNetwork>
#include <QNetworkSession>
#include <QtGlobal>
#include <QJsonDocument>

#define PORT_LISTEN                     4242    // Default Port
#define TIMEOUT_CONNECTION              5000   // ms (5 seconde) Max
#define RESPONSE_ERROR_NAME             "errdesc"
#define CONTENT_TYPE                    "multipart/form-data|application/json"
//Status Response HTTP
#define STATUS_OK                       "200 OK"
#define STATUS_NOT_MODIFIED             "304 Not Modified"
#define STATUS_BAD_REQUEST              "400 Bad Request"
#define STATUS_UNAUTHORIZED             "401 Unauthorized"
#define STATUS_INTERNAL_SERVER_ERROR    "500 Internal Server Error"

class ServerHttp : public QObject
{
    Q_OBJECT
public:
    ServerHttp();
    ~ServerHttp();
    int                     start(void);
    int                     start(const int port);
    int                     stop(void);
signals:
    void                    returnStatutRequestBusy();
    void                    returnStatutRequestOk();
    void                    receivedFreeCredit(int credit);
public slots:
private slots:
    void                    sendResponseBusy();
    void                    sendResponseOk();
    void                    newConnection();
    void                    acceptError(QAbstractSocket::SocketError socketError);
    void                    readyRead();
private:
    QTcpServer              *p_tcpServer;
    QTcpSocket              *p_newClient;
    QMap<QString, QString>  m_data;
    QString                 m_ipAddress;
    QTimer                  m_timer;
    bool                    m_started;
    bool                    m_initLog;
    int                     checkValue(void);
    void                    printData(void);
    int                     parsingDataApplicationJson(QList<QString>split, int *indexParsing);
    void                    parsingDataMultipartFormData(QList<QString>split, int *indexParsing);
    int                     parsingData(QList<QString>split, int *indexParsing);
    void                    parsingHeader(QList<QString>split, int *indexParsing);
    int                     parsingURL(QList<QString>split);
    int                     starting(const int port);
    void                    sendResponse(QString response, QString details);
    void                    log(QString niveau, QString data);
    int                     parseHttpRequest(QString data);
};

#endif // SERVERHTTP_H