#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , isPaused(false)
{
    // Create central widget and layout
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    setCentralWidget(centralWidget);

    // Create URL input layout
    QHBoxLayout *urlLayout = new QHBoxLayout();
    urlEdit = new QLineEdit(this);
    urlEdit->setPlaceholderText("Enter YouTube URL here");
    playButton = new QPushButton("Play", this);
    urlLayout->addWidget(urlEdit);
    urlLayout->addWidget(playButton);
    mainLayout->addLayout(urlLayout);

    // Create video player
    videoPlayer = new VideoPlayer(this);
    mainLayout->addWidget(videoPlayer);

    // Create control layout
    QHBoxLayout *controlLayout = new QHBoxLayout();
    stopButton = new QPushButton("Stop", this);
    pauseResumeButton = new QPushButton("Pause", this);
    progressSlider = new QSlider(Qt::Horizontal, this);
    timeLabel = new QLabel("0:00 / 0:00", this);

    controlLayout->addWidget(stopButton);
    controlLayout->addWidget(pauseResumeButton);
    controlLayout->addWidget(progressSlider);
    controlLayout->addWidget(timeLabel);
    mainLayout->addLayout(controlLayout);

    // Connect signals and slots
    connect(playButton, &QPushButton::clicked, this, &MainWindow::onPlayClicked);
    connect(stopButton, &QPushButton::clicked, this, &MainWindow::onStopClicked);
    connect(pauseResumeButton, &QPushButton::clicked, this, &MainWindow::onPauseResumeClicked);
    connect(progressSlider, &QSlider::sliderMoved, this, &MainWindow::onSliderMoved);

    connect(videoPlayer, &VideoPlayer::durationChanged, this, &MainWindow::onDurationChanged);
    connect(videoPlayer, &VideoPlayer::positionChanged, this, &MainWindow::onPositionChanged);
    connect(videoPlayer, &VideoPlayer::playbackFinished, this, &MainWindow::onPlaybackFinished);

    // Set window properties
    setWindowTitle("Qt YouTube Player");
    resize(800, 600);
}

MainWindow::~MainWindow()
{
}

void MainWindow::onPlayClicked()
{
    QString url = urlEdit->text();
    if (!url.isEmpty()) {
        videoPlayer->playUrl(url);
        isPaused = false;
        pauseResumeButton->setText("Pause");
    }
}

void MainWindow::onStopClicked()
{
    videoPlayer->stop();
    progressSlider->setValue(0);
    timeLabel->setText("0:00 / 0:00");
}

void MainWindow::onPauseResumeClicked()
{
    if (isPaused) {
        videoPlayer->resume();
        pauseResumeButton->setText("Pause");
    } else {
        videoPlayer->pause();
        pauseResumeButton->setText("Resume");
    }
    isPaused = !isPaused;
}

void MainWindow::onDurationChanged(double duration)
{
    progressSlider->setRange(0, static_cast<int>(duration));
    timeLabel->setText(QString("0:00 / %1").arg(formatTime(duration)));
}

void MainWindow::onPositionChanged(double position)
{
    if (!progressSlider->isSliderDown()) {
        progressSlider->setValue(static_cast<int>(position));
    }
    timeLabel->setText(QString("%1 / %2")
                      .arg(formatTime(position))
                      .arg(formatTime(progressSlider->maximum())));
}

void MainWindow::onPlaybackFinished()
{
    progressSlider->setValue(0);
    timeLabel->setText("0:00 / 0:00");
    isPaused = false;
    pauseResumeButton->setText("Pause");
}

void MainWindow::onSliderMoved(int position)
{
    videoPlayer->seek(position);
}

QString MainWindow::formatTime(double seconds)
{
    int total = static_cast<int>(seconds);
    int hours = total / 3600;
    int minutes = (total % 3600) / 60;
    int secs = total % 60;

    if (hours > 0) {
        return QString("%1:%2:%3")
            .arg(hours)
            .arg(minutes, 2, 10, QChar('0'))
            .arg(secs, 2, 10, QChar('0'));
    } else {
        return QString("%1:%2")
            .arg(minutes)
            .arg(secs, 2, 10, QChar('0'));
    }
} 