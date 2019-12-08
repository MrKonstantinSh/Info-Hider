#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QtGui>
#include <QBitArray>
#include <QByteArray>
#include <QString>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Model/imagecontainer.h"
#include "Model/lsbsteganography.h"
#include "Model/texttohide.h"
#include "Model/arc4.h"

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

QVector<unsigned char> toQVector(const QByteArray arr)
{
    QString temp = QString::fromUtf8(arr);
    QVector<unsigned char> vector(temp.length());

    for (int i = 0; i < temp.length(); i++)
    {
        QChar currentChar = temp.at(i);
        vector[i] = currentChar.toLatin1();
    }

    return vector;
}

QByteArray toQByteArray(const QVector<unsigned char> vector)
{
    QString temp = "";
    for (int i = 0; i < vector.size(); i++)
        temp += QChar(vector[i]);

    QByteArray arr = temp.toUtf8();

    return  arr;
}

void MainWindow::on_pbUploadImage_clicked()
{
    QString pathToImgContainer = QFileDialog::getOpenFileName(this,
        "Select the file in which you want to hide the information",
        QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
        "Images (*.png *.bmp)");

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

void MainWindow::on_pbHideInfo_clicked()
{
    LsbSteganography lsb = LsbSteganography();
    QImage imgContainer = QImage(ui->lePathToContainer->text());

    QImage resultImg;
    if (ui->cbUseCryptography->isChecked()) {
        QString sessionKey = ui->leSecretKey->text();
        ARC4 arc4 = ARC4(toQVector(sessionKey.toUtf8()));
        QVector<unsigned char> dataToEncrypt = toQVector(ui->teTextToHide->toPlainText().toUtf8());
        QString encryptedMessage = QString::fromUtf8(toQByteArray(arc4.encrypt(dataToEncrypt)));
        resultImg = lsb.inject(imgContainer, encryptedMessage);
    } else {
        resultImg = lsb.inject(imgContainer, ui->teTextToHide->toPlainText().toUtf8());
    }

    QString pathToResultImg = QFileDialog::getSaveFileName(this,
        "Save the resulting image",
        QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
        "Images (*.png .*bmp)");
    resultImg.save(pathToResultImg);
}

void MainWindow::on_pbUploadImageWithData_clicked()
{
    QString pathToImgContainer = QFileDialog::getOpenFileName(this,
        "Select the file in which you want to hide the information",
        QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
        "Images (*.png *.bmp)");

    if (pathToImgContainer.size() > 0) {
        ui->lePathToContainerWithData->setText(pathToImgContainer);
        ImageContainer imgContainer = ImageContainer(pathToImgContainer);
        ui->lblImgContainerWithData->setPixmap(imgContainer.getImgContainer().scaled(
            ui->lblImgContainer->size().width(),
            ui->lblImgContainer->size().height(),
            Qt::KeepAspectRatio));
        ui->lblImgResolutionWithData->setText("Image resolution: "
            + imgContainer.getImgContainerResolution());
    }
}

void MainWindow::on_pbExtractData_clicked()
{
    LsbSteganography lsb = LsbSteganography();
    QImage imgContainer = QImage(ui->lePathToContainerWithData->text());
    QString message = lsb.extract(imgContainer);

    if (ui->cbUseCryptoToDecrypt->isChecked()) {
        QString sessionKey = ui->leSecretKeyToDecrypt->text();
        ARC4 arc4 = ARC4(toQVector(sessionKey.toUtf8()));
        QVector<unsigned char> dataToDecrypt = toQVector(message.toUtf8());
        QVector<unsigned char> resultMessage = arc4.decrypt(dataToDecrypt);
        ui->teExtractedData->setText(QString::fromUtf8(toQByteArray(resultMessage)));
    } else {
        ui->teExtractedData->setText(message);
    }
}
