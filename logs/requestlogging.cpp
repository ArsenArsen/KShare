#include "requestlogging.hpp"
#include <QDateTime>
#include <QDebug>
#include <io/ioutils.hpp>
#include <settings.hpp>

QDir responses(settings::dir().absoluteFilePath("response"));
void requestlogging::addEntry(RequestContext context) {
    if (!responses.exists()) responses.mkpath(".");
    QFile responseFile(responses.absoluteFilePath(context.sender + "-" + QDateTime().toString("yyyy-MM-dd HH-mm-ss")));
    if (!responseFile.open(QIODevice::WriteOnly)) {
        qCritical().noquote() << "Could not save response! " + responseFile.errorString();
        return;
    }
    responseFile.write((                                                   //
                       ioutils::methodString(context.reply->operation()) + // write method
                       " " +                                               // space
                       context.reply->url().toString() +                   // write url
                       " " +                                               // space
                       QString::number(context.reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()) //
                       +    // write status
                       "\n" // newline
                       )
                       .toUtf8());
    for (auto header : context.reply->rawHeaderList()) responseFile.write(header + "\n");
    responseFile.write("\n\n" + context.response);
    responseFile.close();

    addGUIEntry(context, QFileInfo(responseFile).absoluteFilePath());
}

void requestlogging::addGUIEntry(RequestContext context, QString file) {
}
