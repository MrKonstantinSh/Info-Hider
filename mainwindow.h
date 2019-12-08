#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Model/imagecontainer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pbUploadImage_clicked();
    void on_teTextToHide_textChanged();

    void on_pbHideInfo_clicked();

    void on_pbUploadImageWithData_clicked();

    void on_pbExtractData_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
