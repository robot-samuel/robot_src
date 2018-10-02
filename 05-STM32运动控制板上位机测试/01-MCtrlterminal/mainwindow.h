#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QButtonGroup>

#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>

#include <sstream>
#include <QString>
#include <QTimer> 


namespace Ui {
class MainWindow;
}

class uart_Mode;
class QLabel;
class QTimer;

class SettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
	void initActionsConnections();
	void showStatusMessage(const QString &message);
	void writeData(const QByteArray &data);
	void writeData(const std::string &str);
	void showsend(QString title,const QByteArray &data);
	void showRecv(QString title,const QByteArray &data);


	
private slots:
    void handleError(QSerialPort::SerialPortError error);
    void openSerialPort();
    void closeSerialPort();
    void about();
	void cleanup();

    void readData();



    void on_pushButton_sned_clicked();

    void on_m_cb_continus_clicked();

     void timerUpDate();

     void on_pushB_forward_clicked();

     void on_pushB_backward_clicked();

     void on_pushB_rightward_clicked();

     void on_pushB_leftward_clicked();

     void on_pushB_stop_clicked();

     std::string packCMD();
     std::string packforwardCMD();
     std::string packctrlCMD(int flag);



     void on_lineEdit_Vspeed_editingFinished();

     void on_lineEdit_LFspeed_editingFinished();

     void on_lineEdit_RFspeed_editingFinished();

     void on_lineEdit_Vangel_editingFinished();

private:
    Ui::MainWindow *ui;
    QLabel *status;
	SettingsDialog *settings;
	QSerialPort *serial;
	bool comalive;
    uart_Mode * uart;
    bool continues;
    QButtonGroup *Lbg;
    QButtonGroup *Rbg;

	 QTimer *timer;

	int sendsize;
	int recvsize;
	int checksize;

    std::string lastcmd;


	
};

#endif // MAINWINDOW_H
