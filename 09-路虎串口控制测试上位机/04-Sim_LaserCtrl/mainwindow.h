#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QTimer> 

namespace Ui {
class MainWindow;
}

class laser_ctrl_Mode;
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


	void vaule_chang_line_edit_speed();
	void vaule_chang_line_edit_th();
    void chang_horiSlider_speed_line_edit_speed();
    void chang_horiSlider_angel_line_edit_th();

    void on_pushButton_sned_clicked();

    void on_m_cb_continus_clicked();

     void timerUpDate();

     void on_pushButton_stop_clicked();

private:
    Ui::MainWindow *ui;
    QLabel *status;
	SettingsDialog *settings;
	QSerialPort *serial;
	bool comalive;
    laser_ctrl_Mode * Plaser_ctrl_Mode;
    bool continues;

	QTimer *timer;

	int sendsize;
	int recvsize;
	int checksize;
	
};

#endif // MAINWINDOW_H
