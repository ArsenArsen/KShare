#include "utils.hpp"

#include <QApplication>
#include <QClipboard>
#include <QPainter>
#include <QPixmap>
#include <QProcess>
#include <QScreen>
#include <logger.hpp>
#include <platformbackend.hpp>
#include <settings.hpp>

QColor utils::invertColor(QColor color) {
    return QColor(255 - color.red(), 255 - color.green(), 255 - color.blue());
}

QPixmap utils::extend(QPixmap img, int extraSize, QColor hl) {
    QPixmap newImg(img.width() + extraSize * 2, img.height() + extraSize * 2);
    newImg.fill(hl);
    QPainter ptr(&newImg);
    ptr.drawPixmap(extraSize, extraSize, img);
    ptr.end();
    return newImg;
}

QPixmap utils::fullscreen(bool cursor) {
    QPixmap image;
    QPainter painter;
    QPoint smallestCoordinate = smallestScreenCoordinate();

// Hack for https://bugreports.qt.io/browse/QTBUG-58110
#ifdef Q_OS_LINUX
    static QStringList qVer = QString(qVersion()).split('.');
    if (qVer.at(0).toInt() == 5 && qVer.at(1).toInt() < 9) {
        image = window(0);
        painter.begin(&image);
    } else {
#endif
        int height = 0, width = 0;
        int ox = smallestCoordinate.x() * -1, oy = smallestCoordinate.y() * -1;
        for (QScreen *screen : QApplication::screens()) {
            QRect geo = screen->geometry();
            width = qMax(ox + geo.left() + geo.width(), width);
            height = qMax(oy + geo.top() + geo.height(), height);
        }
        image = QPixmap(width, height);
        image.fill(Qt::transparent);
        width = 0;
        painter.begin(&image);
        painter.translate(ox, oy);

        for (QScreen *screen : QApplication::screens()) {
            QPixmap currentScreen = window(0, screen);
            QRect geo = screen->geometry();
            painter.drawPixmap(geo.left(), geo.top(), geo.width(), geo.height(), currentScreen);
            width += screen->size().width();
        }
#ifdef Q_OS_LINUX
    }
#endif

#ifdef PLATFORM_CAPABILITY_CURSOR
    if (cursor) {
        auto cursorData = PlatformBackend::inst().getCursor();
        painter.drawPixmap(QCursor::pos() - std::get<0>(cursorData), std::get<1>(cursorData));
    }
#endif
    painter.end();
    return image;
}

QPixmap utils::window(WId wid, QScreen *w) {
    return w->grabWindow(wid);
}

void utils::toClipboard(QString value) {
    QApplication::clipboard()->setText(value);
}

QPixmap utils::fullscreenArea(bool cursor, qreal x, qreal y, qreal w, qreal h) {
    return fullscreen(cursor).copy(x, y, w, h);
}

QPoint utils::smallestScreenCoordinate() {
    QPoint smallestCoordinate;
    for (QScreen *screen : QApplication::screens()) {
        smallestCoordinate.rx() = qMin(smallestCoordinate.x(), screen->geometry().left());
        smallestCoordinate.ry() = qMin(smallestCoordinate.y(), screen->geometry().top());
    }
    return smallestCoordinate;
}

QPixmap utils::renderText(QString toRender, int padding, QColor background, QColor pen, QFont font) {
    QFontMetrics metric(font);
    QStringList lines = toRender.replace("\r", "").split('\n');
    QSize resultingSize(0, padding * 2);
    int lineSpace = metric.leading();
    for (QString line : lines) {
        QRect br = metric.boundingRect(line);
        resultingSize.rheight() += lineSpace + br.height();
        resultingSize.rwidth() = qMax(br.width(), resultingSize.width());
    }
    resultingSize.rwidth() += padding * 2;
    QPixmap renderred(resultingSize);
    renderred.fill(background);
    QPainter painter(&renderred);
    painter.setPen(pen);
    int y = padding;
    for (QString line : lines) {
        QRect br = metric.boundingRect(line);
        painter.drawText(padding, y, br.width(), br.height(), 0, line);
        y += lineSpace + br.height();
    }
    painter.end();
    return renderred;
}

QString utils::randomString(int length) {
    QString str;
    str.resize(length);
    for (int s = 0; s < length; s++) str[s] = QChar('A' + char(qrand() % ('Z' - 'A')));
    return str;
}

void utils::externalScreenshot(std::function<void(QPixmap)> callback) {
    QString cmd = settings::settings().value("command/fullscreenCommand", "").toString();
    QStringList args = cmd.split(' ');
    QString tempPath;
    for (QString &arg : args) {
        if (arg == "%FILE_PATH") {
            if (tempPath.isEmpty()) tempPath = "KShare-Ext-Screenshot." + randomString(5);
            arg = tempPath;
        }
    }
    QProcess *process = new QProcess;
    QObject::connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                     [callback, process, tempPath](int code, QProcess::ExitStatus) {
                         if (code != 0) {
                             logger::fatal(QObject::tr("Failed to take external screenshot: \n") + process->readAllStandardError());
                         } else {
                             QPixmap pixmap;
                             if (!tempPath.isEmpty())
                                 pixmap.load(tempPath);
                             else
                                 pixmap.loadFromData(process->readAllStandardOutput());
                             callback(pixmap);
                         }
                         if (!tempPath.isEmpty()) QFile(tempPath).remove();
                     });
    QObject::connect(process, &QProcess::errorOccurred, [](QProcess::ProcessError err) {
        if (err == QProcess::FailedToStart) settings::settings().remove("command/fullscreenCommand");
    });
    process->start(args.takeFirst(), args);
}

void utils::externalScreenshotActive(std::function<void(QPixmap)> callback) {
    QString cmd = settings::settings().value("command/activeCommand", "").toString();
    QStringList args = cmd.split(' ');
    QString tempPath;
    for (QString &arg : args) {
        if (arg == "%FILE_PATH") {
            if (tempPath.isEmpty()) tempPath = "KShare-Ext-Screenshot." + randomString(5);
            arg = tempPath;
        }
    }
    QProcess *process = new QProcess;
    QObject::connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                     [callback, process, tempPath](int code, QProcess::ExitStatus) {
                         if (code != 0) {
                             logger::fatal(QObject::tr("Failed to take external screenshot: \n") + process->readAllStandardError());
                         } else {
                             QPixmap pixmap;
                             if (!tempPath.isEmpty())
                                 pixmap.load(tempPath);
                             else
                                 pixmap.loadFromData(process->readAllStandardOutput());
                             callback(pixmap);
                         }
                         if (!tempPath.isEmpty()) QFile(tempPath).remove();
                     });
    QObject::connect(process, &QProcess::errorOccurred, [](QProcess::ProcessError err) {
        if (err == QProcess::FailedToStart) settings::settings().remove("command/activeCommand");
    });
    process->start(args.takeFirst(), args);
}

QIcon defaultIcon() {
    static QIcon icon = QIcon(":/icons/icon.png");
    return icon;
}

QIcon infinity() {
    static QIcon icon = QIcon(":/icons/infinity.png");
    return icon;
}

QIcon utils::getTrayIcon(int num) {
    if (!num) {
        return defaultIcon();
    } else if (num < 100) {
        QPixmap unscaled = utils::renderText(QString::number(num), 0, Qt::lightGray, Qt::black);
        int dim = qMax(unscaled.width(), unscaled.height());
        QPixmap scaled(dim, dim);
        scaled.fill(Qt::lightGray);
        QPainter *painter = new QPainter(&scaled);
        painter->drawPixmap((dim / 2) - (unscaled.width() / 2), 0, unscaled);
        delete painter;
        return scaled;
    } else {
        return infinity();
    }
}
