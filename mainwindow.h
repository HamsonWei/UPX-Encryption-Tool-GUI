#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QLocale>
#include <QProcess>
#include <QTextCodec>
#include <QDebug>

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
    void disp_output_program_data();

private slots:
    void program_init();

    void swith_compress_mod();

    void on_openBtn_clicked();

    void on_runBtn_clicked();

    void on_infoBtn_clicked();

    void on_checkFast_stateChanged(int arg1);

    void on_checkNormal_stateChanged(int arg1);

    void on_checkGood_stateChanged(int arg1);

    void on_checkBetter_stateChanged(int arg1);

    void on_helpBtn_clicked();

private:
    Ui::MainWindow *ui;
    QProcess *m_Process;
    QTextCodec *m_textCode;

    QString m_filePath;
    QLocale m_myLocal;

    QString m_cCmd;

    bool m_f_fast;
    bool m_f_normal;
    bool m_f_good;
    bool m_f_better;
};
#endif // MAINWINDOW_H
