#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include "videoplayer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onPlayClicked();
    void onStopClicked();
    void onPauseResumeClicked();
    void onDurationChanged(double duration);
    void onPositionChanged(double position);
    void onPlaybackFinished();
    void onSliderMoved(int position);

private:
    VideoPlayer *videoPlayer;
    QLineEdit *urlEdit;
    QPushButton *playButton;
    QPushButton *stopButton;
    QPushButton *pauseResumeButton;
    QSlider *progressSlider;
    QLabel *timeLabel;
    bool isPaused;

    QString formatTime(double seconds);
};

#endif // MAINWINDOW_H 