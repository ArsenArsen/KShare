#ifndef REQUESTLOGGING_HPP
#define REQUESTLOGGING_HPP

#include <QByteArray>
#include <QNetworkReply>

struct RequestContext {
    QByteArray response;
    QString sender;
    QNetworkReply *reply;
};

namespace requestlogging {
void addEntry(RequestContext context);
void addGUIEntry(RequestContext context, QString file);
}

#endif // REQUESTLOGGING_HPP
