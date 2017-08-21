#ifndef REQUESTLOGGING_HPP
#define REQUESTLOGGING_HPP

#include <QByteArray>
#include <QNetworkReply>
#include <settings.hpp>


namespace requestlogging {
struct RequestContext {
    QByteArray response;
    QNetworkReply *reply;
};

class LoggedRequest {
    friend QList<LoggedRequest> getRequests();

public:
    QString getUrl() {
        return url;
    }
    QString getType() {
        return type;
    }
    QString getTime() {
        return time;
    }
    int getResponseCode() {
        return responseCode;
    }
    QByteArray getResponse() {
        return QFile(settings::dir().absoluteFilePath("responses/" + time)).readAll();
    }

private:
    QString url;
    QString type;
    QString time;
    int responseCode;
};

QList<LoggedRequest> getRequests();
void addEntry(RequestContext context);
}

#endif // REQUESTLOGGING_HPP
