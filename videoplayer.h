#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QWidget>
#include <mpv/client.h>
#include <mpv/render_gl.h>
#include <QOpenGLWidget>
#include <QTimer>

class VideoPlayer : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit VideoPlayer(QWidget *parent = nullptr);
    ~VideoPlayer();

    void playUrl(const QString &url);
    void stop();
    void pause();
    void resume();
    void seek(double position);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private slots:
    void onMpvEvents();
    void onFrameSwapped();

private:
    mpv_handle *mpv;
    mpv_render_context *mpv_gl;
    QTimer *timer;
    bool initialized;

    void handleMpvEvent(mpv_event *event);
    static void on_update(void *ctx);

signals:
    void durationChanged(double duration);
    void positionChanged(double position);
    void playbackFinished();
};

#endif // VIDEOPLAYER_H 