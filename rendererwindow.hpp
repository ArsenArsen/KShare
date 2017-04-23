#ifndef RENDERERWINDOW_HPP
#define RENDERERWINDOW_HPP

#include <QtGui>

class RendererWindow : public QWindow
{
    Q_OBJECT
public:
    RendererWindow(QRect &size, QWindow *parent = 0);
    virtual void render(QPainter *painter);
public slots:
    void renderLater();
    void renderNow();
protected:
    bool event(QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void exposeEvent(QExposeEvent *event) override;
private:
    QBackingStore *backingStore;
};

#endif // RENDERERWINDOW_HPP
