#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "settingsdialog.h"

#include <QMessageBox>
#include <QLabel>
#include <QtSerialPort/QSerialPort>
#include <QString>
#include <QStringList>
#include <QValidator>
#include <QTextCodec>
#include<iostream>

#include "laser_ctrl_Mode.hpp"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	

	serial = new QSerialPort(this);
    settings = new SettingsDialog;
	comalive = false;
	Plaser_ctrl_Mode = new laser_ctrl_Mode;
	
    status = new QLabel;
    ui->statusBar->addWidget(status);

    connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &MainWindow::handleError);

    initActionsConnections();

    //! [2]
        connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);
	
    QValidator *validator1=new QIntValidator(-12000,12000,this);
    ui->lineEdit_speed->setValidator(validator1);
    ui->lineEdit_speed->setText("0");

    QValidator *validator2=new QIntValidator(-8000,8000,this);
    ui->lineEdit_angel->setValidator(validator2);
    ui->lineEdit_angel->setText("0");

    QValidator *validator3=new QIntValidator(30,10000,this);
    ui->lineEdit_timeout->setValidator(validator3);
    ui->lineEdit_timeout->setText("50");

	ui->m_horiSlider_speed->setMinimum(-12000);    
	ui->m_horiSlider_speed->setMaximum(12000);
	ui->m_horiSlider_speed->setValue(0);
	
	ui->m_horiSlider_angel->setMinimum(-8000);
	ui->m_horiSlider_angel->setMaximum(8000);
	ui->m_horiSlider_angel->setValue(0);
	
	connect(ui->m_horiSlider_speed,SIGNAL(valueChanged(int)),           
		this,SLOT(vaule_chang_line_edit_speed()) );    
	connect(ui->m_horiSlider_angel,SIGNAL(valueChanged(int)),           
		this,SLOT(vaule_chang_line_edit_th()) );

    connect(ui->lineEdit_speed,SIGNAL(textEdited(QString)),
        this,SLOT(chang_horiSlider_speed_line_edit_speed()) );
    connect(ui->lineEdit_angel,SIGNAL(textEdited(QString)),
        this,SLOT(chang_horiSlider_angel_line_edit_th()) );

    continues = false;
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpDate()));
	
	
    recvsize=0;
    sendsize=0;
    checksize=0;
    ui->label_recvsize->setText(QString::number(recvsize,10));
    ui->label_sendsize->setText(QString::number(sendsize,10));

	ui->lineEdit_DLF->setText("0");
	ui->lineEdit_DRF->setText("0");
	ui->lineEdit_DLB->setText("0");
	ui->lineEdit_DRB->setText("0");
	 
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initActionsConnections()
{

    connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::openSerialPort);
    connect(ui->actionDisconnect, &QAction::triggered, this, &MainWindow::closeSerialPort);
    //connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionConfigure, &QAction::triggered, this->settings, &MainWindow::show);
    connect(ui->actionClear, &QAction::triggered, this, &MainWindow::cleanup);
    connect(ui->actionABOUT, &QAction::triggered, this, &MainWindow::about);
    connect(ui->actionAboutQT, &QAction::triggered, qApp, &QApplication::aboutQt);
}

void MainWindow::showStatusMessage(const QString &message)
{
    status->setText(message);
}
void MainWindow::about()
{
    QMessageBox::about(this, tr("About Simple Terminal"),
                       tr("The <b>Simple Terminal</b> example demonstrates how to "
                          "use the Qt Serial Port module in modern GUI applications "
                          "using Qt, with a menu bar, toolbars, and a status bar."));
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        //closeSerialPort();
    }
}

void MainWindow::openSerialPort()
{
    SettingsDialog::Settings p = settings->settings();
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    if (serial->open(QIODevice::ReadWrite)) {
       // console->setEnabled(true);
       // console->setLocalEchoEnabled(p.localEchoEnabled);
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->actionConfigure->setEnabled(false);
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                          .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
		comalive = true;
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

        showStatusMessage(tr("Open error"));
		comalive = false;
    }
}

void MainWindow::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
    //console->setEnabled(false);
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
    showStatusMessage(tr("Disconnected"));
	comalive = false;
}

void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}

void MainWindow::writeData(const std::string &str)
{
     QByteArray data;
	int size = str.length();
     data.resize(size);
     for(int i=0; i<size; i++) {
         data[i]=str[i]; 
     	}
     
    serial->write(data);
    showsend("send date:",data);
    sendsize+=1;
    ui->label_sendsize->setText(QString::number(sendsize,10));
}

void MainWindow::showsend(QString title,const QByteArray &data)
{
    QString item;
    item += title;
    int size = data.length();
     for(int i =0;i<size;i++){
         unsigned char iTemp = data.at(i);
         //QString str = QString::number(iTemp&0xff,16);
        // QString str(QObject::tr("0x%02x ").arg(iTemp));
         QString str;
         str.sprintf("%02X ",iTemp);
        // if(iTemp<10) str.insert(0,"0");  //注意
         item += str;
     	}
    //item = data.toHex();
    //list << "98" << "99" << "100" << "101" << "102";
    
    ui->listWidget->addItem (item);
    //ui->listWidget->sortItems();


}
void MainWindow::showRecv(QString title,const QByteArray &data)
{
    QString item;
    item += title;
    int size = data.length();
     for(int i =0;i<size;i++){
         unsigned char iTemp = data.at(i);
         //QString str = QString::number(iTemp&0xff,16);
        // QString str(QObject::tr("0x%02x ").arg(iTemp));
         QString str;
         str.sprintf("%02X ",iTemp);
        // if(iTemp<10) str.insert(0,"0");  //注意
         item += str;
     	}
    //item = data.toHex();
    //list << "98" << "99" << "100" << "101" << "102";
    
   // ui->listWidget->addItem (item);
   ui->listWidget_return->addItem (item);
    //ui->listWidget->sortItems();


}


//���մ�������
void MainWindow::readData()
{
    QByteArray data = serial->readAll();
    //����״̬����������
    showRecv("recv:",data);
    char* str = data.data();
    int size = data.length();
    std::string strdata;
    strdata.assign(str,size);
    //Plaser_ctrl_Mode->recvbuffer+=strdata;
	recvsize+=size;
	 
    Plaser_ctrl_Mode->comRecvFlag = Plaser_ctrl_Mode->procss_recv_buffer(strdata);
    if(Plaser_ctrl_Mode->comRecvFlag){
		checksize++;
		
        ui->lineEdit_LF->setText(QString::number(Plaser_ctrl_Mode->LF_counter,10));
        ui->lineEdit_RF->setText(QString::number(Plaser_ctrl_Mode->RF_counter,10));
        ui->lineEdit_LB->setText(QString::number(Plaser_ctrl_Mode->LB_counter,10));
        ui->lineEdit_RB->setText(QString::number(Plaser_ctrl_Mode->RB_counter,10));

	ui->lineEdit_DLF->setText(QString::number(Plaser_ctrl_Mode->DLF_counter,10));
	ui->lineEdit_DRF->setText(QString::number(Plaser_ctrl_Mode->DRF_counter,10));
	ui->lineEdit_DLB->setText(QString::number(Plaser_ctrl_Mode->DLB_counter,10));
	ui->lineEdit_DRB->setText(QString::number(Plaser_ctrl_Mode->DRB_counter,10));
		
        }
	ui->label_recvsize->setText(QString::number(recvsize,10)+":"+QString::number(recvsize/20,10)+":"+QString::number(checksize,10));
    //strdata.clear();
}

void MainWindow::cleanup()
{
    ui->listWidget->clear();
    ui->listWidget_return->clear();
    sendsize=0;
    ui->label_sendsize->setText(QString::number(sendsize,10));
    recvsize=0;
    ui->label_recvsize->setText(QString::number(recvsize,10));
	checksize=0;
}

void MainWindow::vaule_chang_line_edit_speed()
{    

    ui->lineEdit_speed->setText(QString::number(ui->m_horiSlider_speed->value(), 10));
}
	
void MainWindow::vaule_chang_line_edit_th()
{

    ui->lineEdit_angel->setText(QString::number(ui->m_horiSlider_angel->value(), 10));
}

void MainWindow::chang_horiSlider_speed_line_edit_speed()
{    
    ui->m_horiSlider_speed->setValue(ui->lineEdit_speed->text().toInt());

}
	
void MainWindow::chang_horiSlider_angel_line_edit_th()
{
    ui->m_horiSlider_angel->setValue(ui->lineEdit_angel->text().toInt());
}


void MainWindow::on_pushButton_sned_clicked()
{
    float speed = (ui->lineEdit_speed->text().toFloat()/10000.);
    float th = (ui->lineEdit_angel->text().toFloat()/10000.);

    std::string cmd= Plaser_ctrl_Mode->TC_cmd_package( speed, th);
	if(comalive == true){
    			writeData(cmd);
		}
	else{
			showStatusMessage(tr("please connect a com port first,Disconected now"));
			//showStatusMessage(tr("Disconnected"));
		}
}


void MainWindow::on_m_cb_continus_clicked()
{
    if(ui->m_cb_continus->checkState()){
      continues =true;
    int timeout = ui->lineEdit_timeout->text().toInt();
    timer->start(timeout);
    }
    else{
        continues =false;
         timer->stop();
    }
}

void MainWindow::timerUpDate()
{   
    if(continues){
    float speed = (ui->lineEdit_speed->text().toFloat()/10000.);
    float th = (ui->lineEdit_angel->text().toFloat()/10000.);

    std::string cmd= Plaser_ctrl_Mode->TC_cmd_package( speed, th);
    if(comalive == true){
                writeData(cmd);
        }
    else{
            showStatusMessage(tr("please connect a com port first,Disconected now"));
            //showStatusMessage(tr("Disconnected"));
        }
    }
}  
//单击停车
void MainWindow::on_pushButton_stop_clicked()
{
	continues = false;
	ui->lineEdit_speed->setText("0");
	 ui->lineEdit_angel->setText("0");

	 ui->m_horiSlider_speed->setValue(0);
	 ui->m_horiSlider_angel->setValue(0);

     ui->m_cb_continus->setChecked(false);
     float speed = (ui->lineEdit_speed->text().toFloat()/10000.);
     float th = (ui->lineEdit_angel->text().toFloat()/10000.);

     std::string cmd= Plaser_ctrl_Mode->TC_cmd_package( speed, th);
     if(comalive == true){
                 writeData(cmd);
         }
     else{
             showStatusMessage(tr("please connect a com port first,Disconected now"));
             //showStatusMessage(tr("Disconnected"));
         }
	
}
