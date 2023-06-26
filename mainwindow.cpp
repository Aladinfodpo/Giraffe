#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <shellapi.h>
#include <QSettings>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->openGLWidget, &MyGLWidget::errorRaised, this, &MainWindow::on_errorRaised);
    connect(ui->openGLWidget, &MyGLWidget::fragChanged, this, &MainWindow::on_fragChanged);
    connect(ui->openGLWidget, &MyGLWidget::fpsCalculated, this, &MainWindow::on_fpsCalculated);
    connect(ui->checkBox_dumpWarning, &QCheckBox::toggled, ui->openGLWidget, &MyGLWidget::dumpWarnings);
    connect(ui->pushButton_snapshot, &QPushButton::clicked, ui->openGLWidget, &MyGLWidget::takeScreenShot);
    connect(ui->openGLWidget, &MyGLWidget::snpashotAvailable, this, &MainWindow::on_snapshotAvailable);
    connect(ui->actionPlane, &QAction::triggered, ui->openGLWidget, &MyGLWidget::loadPlane);
    connect(ui->actionSphere, &QAction::triggered, ui->openGLWidget, &MyGLWidget::loadSphere);
    connect(ui->actionSphere, &QAction::triggered, ui->openGLWidget, &MyGLWidget::loadSphere);

    QCoreApplication::setAttribute(Qt::AA_CompressHighFrequencyEvents, true);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_horizontalSlider_valueChanged(int value){
}

void MainWindow::on_errorRaised(QString in_desc, bool isError){
    ui->DebugTextBrowser->setTextColor(QColor(isError ? 255 : 0, 0, 0));
    ui->DebugTextBrowser->append(in_desc + "\n");
}

void MainWindow::on_fpsCalculated(QString in_desc){
    ui->label_fps->setText(in_desc);
}

void MainWindow::on_pushButton_clicked(){
    ui->openGLWidget->compileFrag(ui->shaderTextEdit->toPlainText());
}

void MainWindow::on_actionAbout_Giraffe_triggered(){
    QMessageBox qm(QMessageBox::Information, "About Giraffe", "Giraffe Version : 0.0.4\n\n\nSometines longer can mean better");
    qm.exec();
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value){
    ui->openGLWidget->updateInX(value/100.f);
}

void MainWindow::on_snapshotAvailable(QImage& image){
    static int nImage = 0;
    m_snapshot.push_back(image);
    ui->listWidget->addItem("Snapshot "+QString::number(++nImage));
    ui->listWidget->update();
    ui->listWidget->setCurrentRow(m_snapshot.size()-1);
}

void MainWindow::on_listWidget_currentRowChanged(int currentRow){
    if(currentRow != -1)
        ui->label_3->setPixmap(QPixmap::fromImage(m_snapshot[currentRow].scaled(QSize(100,80), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    else
        ui->label_3->clear();
}

void MainWindow::on_pushButton_2_clicked(){
    const auto row = ui->listWidget->currentRow();
    if(row == -1)
        return;

    delete ui->listWidget->takeItem(row);
    m_snapshot.erase(m_snapshot.begin() + row);
}

void MainWindow::on_pushButton_3_clicked(){
    const auto row = ui->listWidget->currentRow();
    if(row == -1)
        return;

    auto filename = ui->listWidget->currentItem()->text() + ".png";
    m_snapshot[row].save(filename);
    ShellExecuteA(NULL, "open", filename.toStdString().c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void MainWindow::on_fragChanged(QString frag){
    ui->shaderTextEdit->setText(frag);
}

void MainWindow::on_actionStash_triggered()
{
    QSettings settings("Giraffe","Giraffe");
    settings.beginGroup("Stack");
    settings.setValue("last", ui->shaderTextEdit->toPlainText());
}


void MainWindow::on_actionPop_triggered()
{
    QSettings settings("Giraffe","Giraffe");
    settings.beginGroup("Stack");
    auto frag = settings.value("last", "").toString();
    if(!frag.isEmpty()){
        ui->shaderTextEdit->setText(frag);
        ui->openGLWidget->compileFrag(frag);
    }
}


void MainWindow::on_pushButton_4_clicked()
{
    QFile file((ui->lineEdit->text().isEmpty() ? "temp" : ui->lineEdit->text()) + ".glsl");
    if(file.open(QIODevice::WriteOnly))
        QTextStream(&file) << ui->shaderTextEdit->toPlainText();
}

