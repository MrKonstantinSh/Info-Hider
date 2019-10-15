#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QtGui>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Model/imagecontainer.h"
#include "Model/lsbsteganography.h"
#include "Model/texttohide.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbUploadImage_clicked()
{
    QString pathToImgContainer = QFileDialog::getOpenFileName(this,
        "Select the file in which you want to hide the information",
        QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
        "Images (*.png *.jpg)");

    if (pathToImgContainer.size() > 0) {
        ui->lePathToContainer->setText(pathToImgContainer);
        ImageContainer imgContainer = ImageContainer(pathToImgContainer);
        ui->lblImgContainer->setPixmap(imgContainer.getImgContainer().scaled(
            ui->lblImgContainer->size().width(),
            ui->lblImgContainer->size().height(),
            Qt::KeepAspectRatio));
        ui->lblImgResolution->setText("Image resolution: "
            + imgContainer.getImgContainerResolution());
        ui->lblCharLeft->setText("Characters left: " +
            QString::number(LsbSteganography::calcMaxNumOfCharToHide(imgContainer.getImgContainer())));
        ui->teTextToHide->setReadOnly(false);

    }
}

void MainWindow::on_teTextToHide_textChanged()
{
    ImageContainer imgContainer = ImageContainer(ui->lePathToContainer->text());
    TextToHide textToHide = TextToHide(imgContainer.getImgContainer());
    ui->lblCharLeft->setText("Characters left: "
        + QString::number(textToHide.getMaxLength() - ui->teTextToHide->toPlainText().length()));

    if (ui->teTextToHide->toPlainText().length() > textToHide.getMaxLength()) {
        ui->teTextToHide->setText(ui->teTextToHide->toPlainText().left(textToHide.getMaxLength()));
        QMessageBox::warning(this, "Warning", "You have exceeded the maximum number of characters for your container!");
    }
}
