#ifndef RECORDINGPREVIEW_HPP
#define RECORDINGPREVIEW_HPP

#include <QLabel>
#include <QObject>
#include <QRect>
#include <QWidget>

class RecordingPreview : public QWidget {
    Q_OBJECT
public:
    explicit RecordingPreview(QRect recordingArea, QWidget *parent = 0);
    ~RecordingPreview();
    void setPixmap(QPixmap map);
    void setTime(QString time, int frame);

private:
    QLabel *label;
    QLabel *hintLabel;
    QSize size;
    QRect recordingArea;
};

#endif // RECORDINGPREVIEW_HPP
