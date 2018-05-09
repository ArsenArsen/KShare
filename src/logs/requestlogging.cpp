#include "requestlogging.hpp"
#include <QDateTime>
#include <mainwindow.hpp>
#include <io/ioutils.hpp>
#include <utils.hpp>

// $type $url $status $time
// $type = GET POST PATCH DELETE etc
// $url = request target
// $status = response code
// $time = time of request, file name for response: $SETTINGS_DIR/responses/$time

QDir responses(settings::dir().absoluteFilePath("responses"));
QString requestPath = settings::dir().absoluteFilePath("history");


void requestlogging::addEntry(RequestContext context) {
    if (!responses.exists()) responses.mkpath(".");
    QString timeNow = QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd HH-mm-ss-zzz");
    QFile responseFile(responses.absoluteFilePath(timeNow));
    QFile requestFile(requestPath);

    if (!responseFile.open(QIODevice::WriteOnly)) {
        qCritical().noquote() << "Could not save response! " + responseFile.errorString();
        return;
    }

    if (!requestFile.open(QIODevice::Append)) {
        qCritical().noquote() << "Could not append request! " + responseFile.errorString();
        return;
    }

    for (auto header : context.reply->rawHeaderList())
        responseFile.write(header + ": " + context.reply->rawHeader(header) + "\n");
    responseFile.write("\n\n" + context.response);
    responseFile.close();

    QTextStream(&requestFile) << ioutils::methodString(context.reply->operation()) << " "   // $type
                              << context.reply->url().toString().replace(" ", "%20") << " " // $url
                              << context.reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() << " " // $status
                              << timeNow.replace(" ", "_") << endl
                              << flush; // $time
    requestFile.close();
}

using requestlogging::LoggedRequest;

QList<LoggedRequest> requestlogging::getRequests() {
    QList<LoggedRequest> ret;

    QFile requestFile(requestPath);
    if (!requestFile.exists() || !requestFile.open(QIODevice::ReadOnly)) return ret;

    QByteArray line;
    while ((line = requestFile.readLine()).size() != 0) {
        LoggedRequest r;
        QTextStream stream(&line);
        stream >> r.type;
        stream >> r.url;
        stream >> r.responseCode;
        stream >> r.time;
        r.time = r.time.replace("_", " ");
        ret.append(r);
    }

    return ret;
}

void requestlogging::indicator::show(int count) {
    MainWindow::inst()->setTrayIcon(utils::getTrayIcon(count));
}
