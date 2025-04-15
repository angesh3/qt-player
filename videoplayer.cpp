#include "videoplayer.h"
#include <QOpenGLContext>
#include <stdexcept>
#include <QProcess>
#include <locale.h>
#include <QDebug>

VideoPlayer::VideoPlayer(QWidget *parent)
    : QOpenGLWidget(parent)
    , mpv(nullptr)
    , mpv_gl(nullptr)
    , timer(nullptr)
    , initialized(false)
{
    // Set C locale for MPV
    setlocale(LC_NUMERIC, "C");
    
    setAttribute(Qt::WA_TranslucentBackground);
    mpv = mpv_create();
    if (!mpv) {
        qDebug() << "Failed to create MPV instance";
        throw std::runtime_error("Could not create MPV instance");
    }

    // Enable default key bindings
    if (mpv_set_option_string(mpv, "input-default-bindings", "yes") < 0)
        qDebug() << "Failed to enable input bindings";
        
    if (mpv_set_option_string(mpv, "input-vo-keyboard", "yes") < 0)
        qDebug() << "Failed to enable keyboard input";
        
    if (mpv_set_option_string(mpv, "ytdl", "yes") < 0)  // Enable youtube-dl/yt-dlp support
        qDebug() << "Failed to enable youtube-dl support";

    if (mpv_initialize(mpv) < 0) {
        qDebug() << "Failed to initialize MPV";
        throw std::runtime_error("Could not initialize MPV");
    }

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &VideoPlayer::onMpvEvents);
    timer->start(20);

    connect(this, &VideoPlayer::frameSwapped, this, &VideoPlayer::onFrameSwapped);
}

VideoPlayer::~VideoPlayer()
{
    makeCurrent();
    if (mpv_gl) {
        mpv_render_context_free(mpv_gl);
    }
    if (mpv) {
        mpv_terminate_destroy(mpv);
    }
}

void VideoPlayer::playUrl(const QString &url)
{
    if (!mpv)
        return;

    const QByteArray ba = url.toUtf8();
    const char *args[] = {"loadfile", ba.constData(), NULL};
    mpv_command_async(mpv, 0, args);
}

void VideoPlayer::stop()
{
    if (!mpv)
        return;

    const char *args[] = {"stop", NULL};
    mpv_command_async(mpv, 0, args);
}

void VideoPlayer::pause()
{
    if (!mpv)
        return;

    const char *args[] = {"set", "pause", "yes", NULL};
    mpv_command_async(mpv, 0, args);
}

void VideoPlayer::resume()
{
    if (!mpv)
        return;

    const char *args[] = {"set", "pause", "no", NULL};
    mpv_command_async(mpv, 0, args);
}

void VideoPlayer::seek(double position)
{
    if (!mpv)
        return;

    const char *args[] = {"seek", QString::number(position).toUtf8().constData(), "absolute", NULL};
    mpv_command_async(mpv, 0, args);
}

void VideoPlayer::initializeGL()
{
    if (initialized)
        return;

    mpv_opengl_init_params gl_init_params{
        [](void *ctx, const char *name) -> void* {
            QOpenGLContext *glctx = QOpenGLContext::currentContext();
            if (!glctx)
                return nullptr;
            return reinterpret_cast<void*>(glctx->getProcAddress(QByteArray(name)));
        },
        nullptr
    };

    mpv_render_param params[]{
        {MPV_RENDER_PARAM_API_TYPE, const_cast<char *>(MPV_RENDER_API_TYPE_OPENGL)},
        {MPV_RENDER_PARAM_OPENGL_INIT_PARAMS, &gl_init_params},
        {MPV_RENDER_PARAM_INVALID, nullptr}
    };

    if (mpv_render_context_create(&mpv_gl, mpv, params) < 0)
        throw std::runtime_error("Failed to initialize MPV GL context");

    mpv_render_context_set_update_callback(mpv_gl, VideoPlayer::on_update, reinterpret_cast<void*>(this));

    initialized = true;
}

void VideoPlayer::paintGL()
{
    if (!initialized)
        return;

    mpv_opengl_fbo mpfbo{
        static_cast<int>(defaultFramebufferObject()),
        width(),
        height(),
        0
    };
    int flip_y{1};

    mpv_render_param params[] = {
        {MPV_RENDER_PARAM_OPENGL_FBO, &mpfbo},
        {MPV_RENDER_PARAM_FLIP_Y, &flip_y},
        {MPV_RENDER_PARAM_INVALID, nullptr}
    };

    mpv_render_context_render(mpv_gl, params);
}

void VideoPlayer::resizeGL(int w, int h)
{
    QOpenGLWidget::resizeGL(w, h);
}

void VideoPlayer::onMpvEvents()
{
    while (mpv) {
        mpv_event *event = mpv_wait_event(mpv, 0);
        if (event->event_id == MPV_EVENT_NONE)
            break;
        handleMpvEvent(event);
    }
}

void VideoPlayer::handleMpvEvent(mpv_event *event)
{
    switch (event->event_id) {
    case MPV_EVENT_PROPERTY_CHANGE: {
        mpv_event_property *prop = static_cast<mpv_event_property*>(event->data);
        if (strcmp(prop->name, "time-pos") == 0) {
            if (prop->format == MPV_FORMAT_DOUBLE) {
                double time = *static_cast<double*>(prop->data);
                emit positionChanged(time);
            }
        }
        else if (strcmp(prop->name, "duration") == 0) {
            if (prop->format == MPV_FORMAT_DOUBLE) {
                double time = *static_cast<double*>(prop->data);
                emit durationChanged(time);
            }
        }
        break;
    }
    case MPV_EVENT_END_FILE:
        emit playbackFinished();
        break;
    default:
        break;
    }
}

void VideoPlayer::onFrameSwapped()
{
    if (mpv_gl)
        mpv_render_context_report_swap(mpv_gl);
}

void VideoPlayer::on_update(void *ctx)
{
    QMetaObject::invokeMethod(static_cast<VideoPlayer*>(ctx), "update", Qt::QueuedConnection);
} 