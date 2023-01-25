#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->shaderTextEdit->setText(ui->openGLWidget->getFragmentShader().c_str());
    connect(ui->openGLWidget, &MyGLWidget::errorRaised, this, &MainWindow::on_errorRaised);
    connect(ui->checkBox_dumpWarning, &QCheckBox::toggled, ui->openGLWidget, &MyGLWidget::dumpWarnings);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->openGLWidget->sliderChanged(value);
}

void MainWindow::on_errorRaised(QString in_desc, bool isError){
    ui->DebugTextBrowser->setTextColor(QColor(isError ? 255 : 0, 0, 0));
    ui->DebugTextBrowser->append(in_desc + "\n");
}

void MainWindow::on_pushButton_clicked()
{
    ui->openGLWidget->compileFrag(ui->shaderTextEdit->toPlainText());
}

void MainWindow::on_actionAbout_Giraffe_triggered()
{
    QMessageBox qm(QMessageBox::Information, "About Giraffe", "Giraffe Version : 0.0.1\n\n\nSometines longer can mean better");
    qm.exec();
}
