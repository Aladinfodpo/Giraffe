#pragma once

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_errorRaised(QString in_desc, bool isError);
    void on_fpsCalculated(QString in_desc);

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_clicked();

    void on_actionAbout_Giraffe_triggered();

    void on_horizontalSlider_2_valueChanged(int value);

    void on_snapshotAvailable(QImage& image);

    void on_listWidget_currentRowChanged(int currentRow);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_fragChanged(QString frag);

    void on_actionStash_triggered();

    void on_actionPop_triggered();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
    std::vector<QImage> m_snapshot;
};

