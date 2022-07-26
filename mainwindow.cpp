#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <windows.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_Process = new QProcess(this);
    connect(m_Process, SIGNAL(readyReadStandardOutput()), this, SLOT(disp_output_program_data()));
    m_Process->setReadChannel(QProcess::StandardOutput);
    m_Process->setProcessChannelMode(QProcess::MergedChannels);

    m_Process->start("cmd");

    program_init();
}

MainWindow::~MainWindow()
{
    if(m_Process->isOpen()){
        m_Process->close();
        m_Process->waitForFinished();
    }
    m_Process->deleteLater();
    delete ui;
}

void MainWindow::disp_output_program_data()
{
    QByteArray outputData = m_Process->readAllStandardOutput().data();

    ui->statusBrowser->append(QString::fromLocal8Bit(outputData));

    qDebug() << "Progress Data:" << outputData;
}

void MainWindow::program_init()
{
    ui->runBtn->setDisabled(true);

    m_f_fast = false;
    m_f_normal = false;
    m_f_good = false;
    m_f_better = false;

    ui->checkNormal->setChecked(true);
}

void MainWindow::swith_compress_mod()
{
    QFileInfo fileInfo(m_filePath);
    if(m_f_fast){
        m_cCmd = QString("\"add-on/upx.exe\" \"%1\" -1 -o \"%2/%3_Encryption.exe\"")
                        .arg(m_filePath).arg(fileInfo.absolutePath()).arg(fileInfo.fileName()
                        .remove(".exe"));
    }else if(m_f_normal){
        m_cCmd = QString("\"add-on/upx.exe\" \"%1\" -d -o \"%2/%3_Encryption.exe\"")
                        .arg(m_filePath).arg(fileInfo.absolutePath()).arg(fileInfo.fileName()
                        .remove(".exe"));
    }else if(m_f_good){
        m_cCmd = QString("\"add-on/upx.exe\" \"%1\" -9 -o \"%2/%3_Encryption.exe\"")
                        .arg(m_filePath).arg(fileInfo.absolutePath()).arg(fileInfo.fileName()
                        .remove(".exe"));
    }else if(m_f_better){
        m_cCmd = QString("\"add-on/upx.exe\" \"%1\" --best -o \"%2/%3_Encryption.exe\"")
                        .arg(m_filePath).arg(fileInfo.absolutePath()).arg(fileInfo.fileName()
                        .remove(".exe"));
    }

}

void MainWindow::on_openBtn_clicked()
{
    m_filePath = QFileDialog::getOpenFileName(this, tr("Open File"),
                                              nullptr,
                                              tr("Program (*.exe)"));
    if(m_filePath.isEmpty()){
        QMessageBox::warning(this, "WARNING", "Sorry, you did not choose any program file.\n"
                                                  "We could not working correctly.");
        return;
    }
    ui->filePathEdit->setText(m_filePath);
    ui->runBtn->setEnabled(true);
}

void MainWindow::on_runBtn_clicked()
{
    if((m_f_fast == false) && (m_f_normal == false) && (m_f_good == false) && (m_f_better == false)){
        QMessageBox::warning(this, "WARNING", "Please check the compression method!");
        return;
    }

    if(m_filePath.isEmpty()){
        QMessageBox::warning(this, "WARNING", "Program file no found.");
        return;
    }
    swith_compress_mod();
    m_cCmd += QString("\r\n");    /*注意：\r\n代替键盘的Enter，没有则不会执行 */
    QByteArray cmdByteArr;
    cmdByteArr = m_cCmd.toLocal8Bit();
    m_Process->write(cmdByteArr, cmdByteArr.length());

    qDebug() << "CMD:" << m_cCmd;
}

void MainWindow::on_infoBtn_clicked()
{
    QMessageBox msgBox;
    msgBox.setIconPixmap(QPixmap(":/icon/logo.png"));
    msgBox.setWindowTitle("About");
    msgBox.setText("LET UPX Encryption Tool (GUI)  V1.0\n\n"
                   "Hamson@leee.tech\n"
                   "June 6, 2022\n\n"
                   "ICON: ICONS8 (icons8.com)\n\n"
                   "[ Note ]\n"
                   "This is a free open source software. Base on Qt & UPX Encryption.");
    msgBox.exec();
}

void MainWindow::on_checkFast_stateChanged(int arg1)
{
    if(arg1){
        m_f_fast = true;

        ui->checkNormal->setChecked(false);
        ui->checkGood->setChecked(false);
        ui->checkBetter->setChecked(false);

        qDebug() << "Fast Compress";
    }else{
        m_f_fast = false;
        qDebug() << "Fast Compress Disable";
    }

}

void MainWindow::on_checkNormal_stateChanged(int arg1)
{
    if(arg1){
        m_f_normal = true;

        ui->checkFast->setChecked(false);
        ui->checkGood->setChecked(false);
        ui->checkBetter->setChecked(false);

        qDebug() << "Normal Compress";
    }else{
        m_f_normal = false;
        qDebug() << "Normal Compress Disable";
    }

}

void MainWindow::on_checkGood_stateChanged(int arg1)
{
    if(arg1){
        m_f_good = true;

        ui->checkFast->setChecked(false);
        ui->checkNormal->setChecked(false);
        ui->checkBetter->setChecked(false);

        qDebug() << "Good Compress";
    }else{
        m_f_good = false;
        qDebug() << "Good Compress Disable";
    }

}

void MainWindow::on_checkBetter_stateChanged(int arg1)
{
    if(arg1){
        m_f_better = true;

        ui->checkFast->setChecked(false);
        ui->checkNormal->setChecked(false);
        ui->checkGood->setChecked(false);

        qDebug() << "Best Compress";
    }else{
        m_f_better = false;
        qDebug() << "Best Compress Disable";
    }

}

void MainWindow::on_helpBtn_clicked()
{
    QMessageBox msgBox;
    msgBox.setIconPixmap(QPixmap(":/icon/help.png"));
    msgBox.setWindowTitle("Helper");
    msgBox.setText("LET UPX Encryption Tool (GUI)  V1.0\n\n"
                   "1. Check whether the tool directory \"add-on/upx.exe\" file exists.\n"
                   "2. Click \"open\" in the software interface to select the software you need to encrypt.\n"
                   "3. Check the compression mode you need.\n"
                   "4. Run!\n"
                   "5. The tool will generate the \"Encryption\" file in the program file directory.\n\n"
                   "[ Tips ]\n"
                   "This tool is based on the open source software upx and requires the Windows terminal "
                   "console program to run. No matter the users of Linux system environment or windows system, "
                   "in order to ensure your normal use, please do not change any files in the folder!");
    msgBox.exec();
}
