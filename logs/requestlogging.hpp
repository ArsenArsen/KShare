#ifndef REQUESTLOGGING_HPP
#define REQUESTLOGGING_HPP

#include <QByteArray>
#include <QNetworkReply>
#include <settings.hpp>

struct RequestContext {
    QByteArray response;
    QString sender;
    QNetworkReply *reply;
};

namespace requestlogging {
void addEntry(RequestContext context);

class LoggedRequest {
    friend QList<LoggedRequest> getRequests();

public:
    QString getUrl() {
        return url;
    }
    QString getType() {
        return type;
    }
    QDateTime getTime() {
        return time;
    }
    int getResponseCode() {
        return responseCode;
    }
    QByteArray getResponse() {
        return QFile(settings::dir().absoluteFilePath("responses/" + time.toString("yyyy-MM-dd HH-mm-ss-zzz"))).readAll();
    }

private:
    QString url;
    QString type;
    QDateTime time;
    int responseCode;
};

QList<LoggedRequest> getRequests();
}

#endif // REQUESTLOGGING_HPP
