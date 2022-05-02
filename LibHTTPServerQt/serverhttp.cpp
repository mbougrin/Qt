#include "serverhttp.h"

const char *errorSocket[] = {
    "ConnectionRefusedError",
    "RemoteHostClosedError",
    "HostNotFoundError",
    "SocketAccessError",
    "SocketResourceError",
    "SocketTimeoutError",
    "DatagramTooLargeError",
    "NetworkError",
    "AddressInUseError",
    "SocketAddressNotAvailableError",
    "UnsupportedSocketOperationError",
    "ProxyAuthenticationRequiredError",
    "SslHandshakeFailedError",
    "UnfinishedSocketOperationError",
    "ProxyConnectionRefusedError",
    "ProxyConnectionClosedError",
    "ProxyConnectionTimeoutError",
    "ProxyNotFoundError",
    "ProxyProtocolError",
    "OperationError",
    "SslInternalError",
    "SslInvalidUserDataError",
    "TemporaryError"
};

ServerHttp::ServerHttp()
{
    QFileInfo tmp("/tmp");
    this->p_tcpServer = NULL;
    this->p_newClient = NULL;
    this->m_started = false;
    if (tmp.isDir() == true
            && tmp.isWritable() == true
            && tmp.isReadable() == true) {
        QDir dirLibServerHttp("/tmp/LibServerHttp");
        QDir dirLogs("/tmp/LibServerHttp/logs");
        if (!dirLibServerHttp.exists()) {
            dirLibServerHttp.mkpath(".");
        }
        if (!dirLogs.exists()) {
            dirLogs.mkpath(".");
        }
        this->m_initLog = true;
    } else {
        this->log("CRIT", "Permission denied in folder /tmp/");
        this->m_initLog = false;
    }
}

ServerHttp::~ServerHttp()
{
    if (this->m_started == true)
        this->stop();
}

int     ServerHttp::stop(void)
{
    this->log("INFO", "Stopping ServerHttp");
    this->m_started = false;
    disconnect(this->p_tcpServer, &QTcpServer::newConnection,
            this, &ServerHttp::newConnection);
    disconnect(this->p_tcpServer, &QTcpServer::acceptError,
            this, &ServerHttp::acceptError);
    disconnect(this, &ServerHttp::returnStatutRequestBusy,
            this, &ServerHttp::sendResponseBusy);
    disconnect(this, &ServerHttp::returnStatutRequestOk,
            this, &ServerHttp::sendResponseOk);
    this->p_tcpServer->close();
    delete this->p_tcpServer;
    this->p_tcpServer = NULL;
    if (this->p_newClient != NULL) {
        delete this->p_newClient;
        this->p_newClient = NULL;
    }
    this->log("INFO", "ServerHttp is Stopped");
    return (0);
}

int     ServerHttp::starting(const int port)
{
    QList<QHostAddress> ipAddressesList;
    if (this->m_started == true) {
        this->log("CRIT", "ServerHttp AlReady Started");
        return (-1);
    }
    this->log("INFO", "Starting ServerHttp");
    this->p_tcpServer = new QTcpServer(this);
    if (!this->p_tcpServer->listen(QHostAddress::Any, port)) {
        this->log("CRIT", "tcpServer listen ERROR");
        return (-1);
    }
    ipAddressesList = QNetworkInterface::allAddresses();
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            this->m_ipAddress = ipAddressesList.at(i).toString();
            this->log("INFO", "ipAddress Found" + this->m_ipAddress);
            break;
        }
    }
    if (this->m_ipAddress.isEmpty())
        this->m_ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    if (this->p_tcpServer->isListening() == true) {
        this->log("INFO", "Listen ipAddress:" + this->m_ipAddress
                  + " port:" + QString::number(port));
    } else {
        this->log("CRIT", "ServerHttp Listening Failed");
        return (-1);
    }
    connect(this->p_tcpServer, &QTcpServer::newConnection,
            this, &ServerHttp::newConnection);
    connect(this->p_tcpServer, &QTcpServer::acceptError,
            this, &ServerHttp::acceptError);
    connect(this, &ServerHttp::returnStatutRequestBusy,
            this, &ServerHttp::sendResponseBusy);
    connect(this, &ServerHttp::returnStatutRequestOk,
            this, &ServerHttp::sendResponseOk);
    this->m_started = true;
    this->log("INFO", "ServerHttp is Started");
    return (0);
}

int     ServerHttp::start(void)
{
    return (this->starting(QString::number(PORT_LISTEN).toInt()));
}

int     ServerHttp::start(const int port)
{
    return (this->starting(port));
}

void    ServerHttp::acceptError(QAbstractSocket::SocketError socketError)
{
    this->log("CRIT", __PRETTY_FUNCTION__);
    if (socketError == -1)
        this->log("CRIT", "UnknownSocketError");
    else
        this->log("CRIT", errorSocket[socketError]);
}

int     ServerHttp::parsingURL(QList<QString>split)
{
    QList<QString>split_space = QString(split.at(0)).split(" ");
    if (split_space.isEmpty() == true) {
        this->log("CRIT", "Parsing HTTP Request Error split_space is empty");
        this->sendResponse(QString(STATUS_INTERNAL_SERVER_ERROR),
                           "Parsing HTTP Request Error");
        return (-1);
    }
    m_data["Request"] = split_space.at(0);
    m_data["URL"] = split_space.at(1);
    m_data["HTTP"] = split_space.at(2);
    if (m_data["Request"].isEmpty() == true
            || m_data["URL"].isEmpty() == true
            || m_data["HTTP"].isEmpty() == true) {
        this->log("CRIT", "Request HTTP have empty value (Request|URL|HTTP)");
        this->sendResponse(QString(STATUS_BAD_REQUEST),
                           "Request HTTP have empty value (Request|URL|HTTP)");
        return (-1);
    }
    if (m_data["Request"] != "POST") {
        this->log("CRIT", "Request HTTP is not a POST Request");
        this->sendResponse(QString(STATUS_BAD_REQUEST),
                           "Request HTTP is not a POST Request");
        return (-1);
    }
    if (m_data["HTTP"] != "HTTP/1.1") {
        this->log("CRIT", "Version HTTP is not supported");
        this->sendResponse(QString(STATUS_BAD_REQUEST),
                           "Version HTTP is not supported (Use HTTP/1.1");
        return (-1);
    }
    int len = strlen("/addValue");
    if (m_data["URL"].contains("/addValue") == false
            || len < m_data["URL"].length()) {
        if (m_data["URL"].at(len) != '+') {
            this->log("CRIT", "URL is not valid");
            this->sendResponse(QString(STATUS_BAD_REQUEST),
                               "URL is not valid");
            return (-1);
        }
    }
    return (0);
}

void     ServerHttp::parsingHeader(QList<QString>split, int *indexParsing)
{
    for (int i = indexParsing[0]; i < split.size() ; ++i) {
        if (split.at(i).isEmpty())
            break ;
        QList<QString>key_value = QString(split.at(i)).split(":");
        QString key = key_value.at(0);
        QString value = QString(split.at(i)).mid(key.size() + 2, split.at(i).size() - 4);
        m_data[key] = value;
        indexParsing[0] += 1;
    }
}

void    ServerHttp::parsingDataMultipartFormData(QList<QString>split, int *indexParsing)
{
    this->log("INFO", "Parsing Content-Type:" + m_data["Content-Type"]);
    for (int i = indexParsing[0]; i < split.size() ; ++i) {
        if (!QString(split.at(i)).isEmpty() || split.at(i) != "\r\n\r\n") {
            if (QString(split.at(i)).contains("form-data", Qt::CaseInsensitive)) {
                QList<QString>key_parsing = QString(split.at(i)).split("name");
                QString key = QString(key_parsing.at(1)).mid(1, QString(key_parsing.at(1)).length());
                key.replace('"', "");
                QString value = split.at(i + 2);
                m_data[key] = value;
            }
        }
    }
}

int    ServerHttp::parsingDataApplicationJson(QList<QString>split, int *indexParsing)
{
    this->log("INFO", "Parsing Content-Type:" + m_data["Content-Type"]);
    QByteArray json;
    for (int i = indexParsing[0] + 1; i < split.size() ; ++i) {
        json += split.at(i);
    }
    QJsonParseError jsonError;
    QJsonDocument flowerJson = QJsonDocument::fromJson(json, &jsonError);
    if (jsonError.error != QJsonParseError::NoError){
        this->log("CRIT", "Json Error:" + jsonError.errorString());
        this->sendResponse(QString(STATUS_BAD_REQUEST),
                           "Json Error:" + jsonError.errorString());
        return (-1);
    }
    QList<QVariant> list = flowerJson.toVariant().toList();
    QMap<QString, QVariant> map = list[0].toMap();
    this->m_data["type"] = map["type"].toString();
    this->m_data["value"] = map["value"].toString();
    this->m_data["comment"] = map["comment"].toString();
    return (0);
}

int     ServerHttp::parsingData(QList<QString>split, int *indexParsing)
{
    if (m_data["Content-Type"].contains("multipart/form-data")) {
        this->parsingDataMultipartFormData(split, indexParsing);
    } else if (m_data["Content-Type"].contains("application/json")) {
        if (this->parsingDataApplicationJson(split, indexParsing) != 0)
            return (-1);
    } else {
        this->log("CRIT", "Content-Type not supported - Use this Content-Type:"
                  + QString(CONTENT_TYPE));
        this->sendResponse(QString(STATUS_BAD_REQUEST),
                           "Content-Type not supported - Use this Content-Type:"
                           + QString(CONTENT_TYPE));
        return (-1);
    }
    return (0);
}

void    ServerHttp::printData(void)
{
    for (QMap<QString,QString>::const_iterator it = m_data.constBegin()
         ; it != m_data.constEnd()
         ; it++) {
        this->log("INFO", "key: " + it.key());
        this->log("INFO", "value: " + it.value());
    }
}

int     ServerHttp::checkValue(void)
{
    if (this->m_data["type"].isEmpty() == true) {
        this->log("CRIT", "Data type is empty");
        this->sendResponse(QString(STATUS_BAD_REQUEST),
                           "Data type is empty");
        return (-1);
    } else if (!m_data["type"].contains("euros")) {
        this->log("CRIT", "Data type is bad value");
        this->sendResponse(QString(STATUS_BAD_REQUEST),
                           "Data type is bad value");
        return (-1);
    }
    if (this->m_data["value"].isEmpty() == true) {
        this->log("CRIT", "Data value is empty");
        this->sendResponse(QString(STATUS_BAD_REQUEST),
                           "Data value is empty");
        return (-1);
    } else if (this->m_data["value"].toInt() <= 0) {
        this->log("CRIT", "Data value is not positive integer");
        this->sendResponse(QString(STATUS_BAD_REQUEST),
                           "Data value is not positive integer");
        return (-1);
    }
    return (0);
}

int     ServerHttp::parseHttpRequest(QString data)
{
    this->log("INFO", __PRETTY_FUNCTION__);
    //clear old data
    m_data.clear();
    //Check Request HTTP is not empty
    if (data.isEmpty() || data == "\r\n\r\n") {
        this->log("CRIT", "Request HTTP is empty");
        this->sendResponse(QString(STATUS_BAD_REQUEST),
                           "Request HTTP is empty");
        return (-1);
    }
    //Split Request HTTP \r\n
    this->log("INFO", "Parsing HTTP Request");
    QList<QString>split = data.split(QRegExp("\n|\r\n|\r"));
    if (split.isEmpty() == true) {
        this->log("CRIT", "Parsing HTTP Request Error split is empty");
        this->sendResponse(QString(STATUS_INTERNAL_SERVER_ERROR),
                           "Parsing HTTP Request Error");
        return (-1);
    }
    //Parsing URL HTTP Request
    if (this->parsingURL(split) != 0)
        return (-1);
    int indexParsing = 1;
    //Parsing HTTP Header Data
    this->parsingHeader(split, &indexParsing);
    //TODO Check Authorization
    //Parsing Data Body multipart/form-data | application/json
    if (this->parsingData(split, &indexParsing) != 0)
        return (-1);
    //Printing Map after parsing
    this->printData();
    //Check value Received
    if (this->checkValue() != 0)
        return (-1);
    emit receivedFreeCredit(this->m_data["value"].toInt());
    return (0);
}

void    ServerHttp::readyRead(void)
{
    this->log("INFO", __PRETTY_FUNCTION__);
    QByteArray data = this->p_newClient->readAll();
    this->log("INFO", QString("Data Received:\n") + QString(data));
    if (this->parseHttpRequest(QString(data)) == -1) {
        this->log("CRIT", "parseHttpRequest ERROR");
    }
}

void    ServerHttp::sendResponseOk()
{
    if (this->p_newClient == NULL) {
       this->log("CRIT", "return an response without a request");
    } else {
        this->log("INFO", __PRETTY_FUNCTION__);
        this->sendResponse(QString(STATUS_OK), QString());
    }
}

void    ServerHttp::sendResponseBusy()
{
    if (this->p_newClient == NULL) {
       this->log("CRIT", "return an response without a request");
    } else {
        this->log("INFO", __PRETTY_FUNCTION__);
        this->sendResponse(QString(STATUS_NOT_MODIFIED),
                           QString("Serveur is Busy"));
    }
}

void    ServerHttp::sendResponse(QString response, QString details)
{
    QString paquet;
    QString log;
    if (details.isEmpty() == true) {
        paquet = QString(m_data["HTTP"] + QString(" ")
                            + response + QString("\r\n\r\n"));
        this->log("INFO", "Request Response:" + QString(m_data["HTTP"]
                  + QString(" ") + response));
    } else {
        paquet = QString(m_data["HTTP"] + QString(" ")
                            + response + QString("\r\n")
                            + QString(RESPONSE_ERROR_NAME)
                            + ":" + details + QString("\r\n\r\n"));
        this->log("INFO", "Request Response:" + QString(m_data["HTTP"]
                  + QString(" ") + response
                  + " " + QString(RESPONSE_ERROR_NAME)
                  + ":" + details));
    }
    this->m_timer.stop();
    this->p_newClient->write(paquet.toStdString().c_str());
    this->p_newClient->flush();
    this->p_newClient->disconnectFromHost();
}

void    ServerHttp::newConnection()
{
    this->log("INFO", __PRETTY_FUNCTION__);
    QEventLoop q;
    this->p_newClient = this->p_tcpServer->nextPendingConnection();
    connect(this->p_newClient, &QTcpSocket::readyRead,
            this, &ServerHttp::readyRead);
    connect(this->p_newClient, &QAbstractSocket::disconnected,
            this->p_newClient, &QObject::deleteLater);
    //Timeout Request HTTP
    connect(&m_timer, SIGNAL(timeout()), &q, SLOT(quit()));
    this->m_timer.start(TIMEOUT_CONNECTION);
    q.exec();
    this->log("INFO", "Timeout Disconnect From Host");
    this->sendResponse(QString(STATUS_INTERNAL_SERVER_ERROR),
                       "Timeout LibServerHttp");
}

void    ServerHttp::log(QString niveau, QString data)
{
    if (niveau == "INFO") {
        qInfo() << data;
    } else if (niveau == "CRIT") {
        qCritical() << data;
    } else {
        return ;
    }
    if (this->m_initLog == true) {
        QFile file("/tmp/LibServerHttp/logs/http_server.log");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append) == false) {
            qCritical() << "Failed to open /tmp/LibServerHttp/logs/http_server.log";
            return ;
        }
        QTextStream out(&file);
        QDateTime date = QDateTime::currentDateTime();
        out << "[" << date.toString("dd/MM/yyyy-hh:mm:ss")
            << "][" << niveau << "] - " << data << endl;
        file.close();
    }
}