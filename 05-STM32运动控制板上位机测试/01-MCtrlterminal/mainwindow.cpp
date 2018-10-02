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



#include "uart_Mode.hpp"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	

	serial = new QSerialPort(this);
	settings = new SettingsDialog;

	comalive = false;
	uart = new uart_Mode;
	
	status = new QLabel;
	ui->statusBar->addWidget(status);

    connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &MainWindow::handleError);

    initActionsConnections();

    //! [2]
        connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);
	
    //QValidator *validator1=new QIntValidator(0,12000,this);
    //ui->lineEdit_speed->setValidator(validator1);
   // ui->lineEdit_speed->setText("0");

    //QValidator *validator2=new QIntValidator(0,8000,this);
   // ui->lineEdit_angel->setValidator(validator2);
    //ui->lineEdit_angel->setText("0");
    ui->cBox_F_Crash_En->setChecked(true);
    ui->cBox_B_Crash_En->setChecked(true);

    ui->lineEdit_LFspeed->setText("0");
    ui->lineEdit_RFspeed->setText("0");

    ui->lineEdit_Vspeed->setText("0");
    ui->lineEdit_Vangel->setText("0");

    ui->lineEdit_Fobtdis->setText("30");
    ui->lineEdit_Bobtdis->setText("30");

    Lbg = new QButtonGroup(this);
    Lbg ->addButton(ui->rB_LStop,0);
    Lbg ->addButton(ui->rB_LRun,1);
    Lbg ->addButton(ui->rB_LRealse,2);

    ui->rB_LStop->setChecked(true);


    Rbg = new QButtonGroup(this);
    Rbg ->addButton(ui->rB_RStop,0);
    Rbg ->addButton(ui->rB_RRun,1);
    Rbg ->addButton(ui->rB_RRealse,2);
    ui->rB_RStop->setChecked(true);


    continues = false;
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpDate()));
    //timer->start(50);
    ui->lineEdit_timeout->setText("50");

	
    recvsize=0;
    sendsize=0;
	checksize=0;
    ui->label_recvsize->setText(QString::number(recvsize,10));
    ui->label_sendsize->setText(QString::number(sendsize,10));
	 
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
    // lastcmd = str;

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
    	uart->recvbuffer+=strdata;
	recvsize+=size;

    	uart->comRecvFlag = uart->procss_recv_buffer(strdata);
    	if(uart->comRecvFlag){
		 checksize++;

		 ui->lineEdit_L_Encoder->setText(QString::number(uart->LF_counter,10));
		 ui->lineEdit_R_Encoder->setText(QString::number(uart->RF_counter,10));

		 ui->lineEdit_LMotor_Status->setText(QString::number(uart->LFMotor_Status,10));
		  ui->lineEdit_RMotor_Status->setText(QString::number(uart->RFMotor_Status,10));

		 ui->lineEdit_LMotor_Error->setText(QString::number(uart->LFMotor_error,10));
		  ui->lineEdit_RMotor_Error->setText(QString::number(uart->RFMotor_error,10));		 

		 ui->lineEdit_obt1_dis->setText(QString::number(uart->Obt1_dis,10));
		  ui->lineEdit_obt2_dis->setText(QString::number(uart->Obt2_dis,10));	
		 ui->lineEdit_obt3_dis->setText(QString::number(uart->Obt3_dis,10));
		  ui->lineEdit_obt4_dis->setText(QString::number(uart->Obt4_dis,10));		  
        }
	
	ui->label_recvsize->setText(QString::number(recvsize,10)+":"+QString::number(recvsize/25,10)+":"+QString::number(checksize,10));
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

std::string MainWindow::packCMD()
{

   //打包命令
  char data[15];
  data[0] = 0x66;
  data[1] = 0xAA;


  char ctrl = 0;
  if(ui->cBox_F_Crash_En->isChecked()){
      ctrl |= (0X01<<0);
  }
  else{
      ctrl &= (~0X01);
  }
  if(ui->cBox_B_Crash_En->isChecked()){
      ctrl |= (0X01<<1);
  }
  else{
      ctrl &= (~(0X01<<1));
  }

  if(ui->cBox_F_Obt_En->isChecked()){
      ctrl |= (0X01<<2);
  }
  else{
      ctrl &= (~(0X01<<2));
  }

  if(ui->cBox_B_Obt_En->isChecked()){
      ctrl |= (0X01<<3);
  }
  else{
      ctrl &= (~(0X01<<3));
  }

  if(ui->cBox_SetVal_out1->isChecked()){
      ctrl |= (0X01<<4);
  }
  else{
      ctrl &= (~(0X01<<4));
  }

  if(ui->cBox_SetVal_out2->isChecked()){
      ctrl |= (0X01<<5);
  }
  else{
      ctrl &= (~(0X01<<5));
  }

  if(ui->cBox_SetVal_out3->isChecked()){
      ctrl |= (0X01<<6);
  }
  else{
      ctrl &= (~(0X01<<6));
  }

  if(ui->cBox_SetVal_out4->isChecked()){
      ctrl |= (0X01<<7);
  }
  else{
      ctrl &= (~(0X01<<7));
  }

  data[2] = ctrl;

  char MStautes = 0;
  switch(Lbg->checkedId())
     {
     case 0:
         //qDebug() << QString::fromLocal8Bit("抱闸停车");
         MStautes &= (~(0X03<<0));
         break;
     case 1:
         //qDebug() << QString::fromLocal8Bit("运动");
         MStautes |= (0X01<<0);
         break;
     case 2:
        // qDebug() << QString::fromLocal8Bit("释放停车");
         MStautes |= (0X03<<0);
         break;
     }
  switch(Rbg->checkedId())
     {
     case 0:
         //qDebug() << QString::fromLocal8Bit("抱闸停车");
         MStautes &= (~(0X03<<2));
         break;
     case 1:
         //qDebug() << QString::fromLocal8Bit("运动");
         MStautes |= (0X01<<2);
         break;
     case 2:
        // qDebug() << QString::fromLocal8Bit("释放停车");
         MStautes |= (0X03<<2);
         break;
     }

    data[3] = MStautes;
  int LFspeed =  ui->lineEdit_LFspeed->text().toInt();
  if(LFspeed>3500){
      LFspeed = 3500;
  }
  else if (LFspeed<-3500){
       LFspeed = -3500;
  }
  char * ps1  = (char *)(&LFspeed);
  data[4] = * (ps1+1);
  data[5] = * (ps1+0);

  int RFspeed =  ui->lineEdit_RFspeed->text().toInt();
  if(RFspeed>3500){
      RFspeed = 3500;
  }
  else if (RFspeed<-3500){
       RFspeed = -3500;
  }
  char * ps2  = (char *)(&RFspeed);
  data[6] = * (ps2+1);
  data[7] = * (ps2+0);

  data[8] = 0;

  char ReCtrl = 0;
  if(ui->cBox_SetVal_Ral1->isChecked()){
      ReCtrl |= (0X01<<0);
  }
  else{
      ReCtrl &= (~(0X01<<0));
  }

  if(ui->cBox_SetVal_Ral2->isChecked()){
      ReCtrl |= (0X01<<1);
  }
  else{
      ReCtrl &= (~(0X01<<1));
  }

  data[9] = ReCtrl;

  int Fobtdis =   ui->lineEdit_Fobtdis->text().toInt();
  if(Fobtdis>254){
      Fobtdis = 254;
  }
  else if (Fobtdis<20){
       Fobtdis = 20;
  }
   char * ps3  = (char *)(&Fobtdis);
   data[10] = * (ps3+0);


   int Bobtdis =   ui->lineEdit_Bobtdis->text().toInt();
   if(Bobtdis>254){
       Bobtdis = 254;
   }
   else if (Bobtdis<20){
        Bobtdis = 20;
   }
    char * ps4  = (char *)(&Bobtdis);
    data[11] = * (ps4+0);


  data[12] = 0x0C;
  data[13] = 0x77;
  data[14] = 0xCC;

  std::string res;
  res.assign(data,15);
  return res;
}

void MainWindow::on_pushButton_sned_clicked()
{
    //发送按钮
    //组装命令
    //  float speed = (ui->lineEdit_speed->text().toFloat()/10000.);
  //  float th = (ui->lineEdit_angel->text().toFloat()/10000.);

  std::string cmd= packCMD();
	lastcmd = cmd;
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
        int timeout = ui->lineEdit_timeout->text().toInt();
    		timer->start(timeout);
            continues =true;
    		//ui->lineEdit_timeout->setText("50");
    }
    else{
        continues =false;
        timer->stop();
        ui->lineEdit_timeout->setText("0");
    }
}

void MainWindow::timerUpDate()
{   
    if(continues){
   
    if(comalive == true){
              writeData(lastcmd);
        }
    else{
            showStatusMessage(tr("please connect a com port first,Disconected now"));
            //showStatusMessage(tr("Disconnected"));
        }
    }
}  

std::string MainWindow::packforwardCMD()
{

   //打包命令
  char data[15];
  data[0] = 0x66;
  data[1] = 0xAA;


  char ctrl = 0;
  if(ui->cBox_F_Crash_En->isChecked()){
      ctrl |= (0X01<<0);
  }
  else{
      ctrl &= (~0X01);
  }
  if(ui->cBox_B_Crash_En->isChecked()){
      ctrl |= (0X01<<1);
  }
  else{
      ctrl &= (~(0X01<<1));
  }

  if(ui->cBox_F_Obt_En->isChecked()){
      ctrl |= (0X01<<2);
  }
  else{
      ctrl &= (~(0X01<<2));
  }

  if(ui->cBox_B_Obt_En->isChecked()){
      ctrl |= (0X01<<3);
  }
  else{
      ctrl &= (~(0X01<<3));
  }

  if(ui->cBox_SetVal_out1->isChecked()){
      ctrl |= (0X01<<4);
  }
  else{
      ctrl &= (~(0X01<<4));
  }

  if(ui->cBox_SetVal_out2->isChecked()){
      ctrl |= (0X01<<5);
  }
  else{
      ctrl &= (~(0X01<<5));
  }

  if(ui->cBox_SetVal_out3->isChecked()){
      ctrl |= (0X01<<6);
  }
  else{
      ctrl &= (~(0X01<<6));
  }

  if(ui->cBox_SetVal_out4->isChecked()){
      ctrl |= (0X01<<7);
  }
  else{
      ctrl &= (~(0X01<<7));
  }

  data[2] = ctrl;

  char MStautes = 0;
  MStautes |= (0X01<<0);
  MStautes |= (0X01<<2);

  ui->rB_LRun->setChecked(true);
  ui->rB_RRun->setChecked(true);

    data[3] = MStautes;
  int LFspeed =  ui->lineEdit_LFspeed->text().toInt();
  if(LFspeed>3500){
      LFspeed = 3500;
  }
  else if (LFspeed<-3500){
       LFspeed = -3500;
  }
  char * ps1  = (char *)(&LFspeed);
  data[4] = * (ps1+1);
  data[5] = * (ps1+0);

  int RFspeed =  ui->lineEdit_RFspeed->text().toInt();
  if(RFspeed>3500){
      RFspeed = 3500;
  }
  else if (RFspeed<-3500){
       RFspeed = -3500;
  }
  char * ps2  = (char *)(&RFspeed);
  data[6] = * (ps2+1);
  data[7] = * (ps2+0);

  data[8] = 0;

  char ReCtrl = 0;
  if(ui->cBox_SetVal_Ral1->isChecked()){
      ReCtrl |= (0X01<<0);
  }
  else{
      ReCtrl &= (~(0X01<<0));
  }

  if(ui->cBox_SetVal_Ral2->isChecked()){
      ReCtrl |= (0X01<<1);
  }
  else{
      ReCtrl &= (~(0X01<<1));
  }

  data[9] = ReCtrl;

  int Fobtdis =   ui->lineEdit_Fobtdis->text().toInt();
  if(Fobtdis>254){
      Fobtdis = 254;
  }
  else if (Fobtdis<20){
       Fobtdis = 20;
  }
   char * ps3  = (char *)(&Fobtdis);
   data[10] = * (ps3+0);


   int Bobtdis =   ui->lineEdit_Bobtdis->text().toInt();
   if(Bobtdis>254){
       Bobtdis = 254;
   }
   else if (Bobtdis<20){
        Bobtdis = 20;
   }
    char * ps4  = (char *)(&Bobtdis);
    data[11] = * (ps4+0);


  data[12] = 0x0C;
  data[13] = 0x77;
  data[14] = 0xCC;

  std::string res;
  res.assign(data,15);
  return res;
}

std::string MainWindow::packctrlCMD(int flag)
{

   //打包命令
  char data[15];
  data[0] = 0x66;
  data[1] = 0xAA;


  char ctrl = 0;
  if(ui->cBox_F_Crash_En->isChecked()){
      ctrl |= (0X01<<0);
  }
  else{
      ctrl &= (~0X01);
  }
  if(ui->cBox_B_Crash_En->isChecked()){
      ctrl |= (0X01<<1);
  }
  else{
      ctrl &= (~(0X01<<1));
  }

  if(ui->cBox_F_Obt_En->isChecked()){
      ctrl |= (0X01<<2);
  }
  else{
      ctrl &= (~(0X01<<2));
  }

  if(ui->cBox_B_Obt_En->isChecked()){
      ctrl |= (0X01<<3);
  }
  else{
      ctrl &= (~(0X01<<3));
  }

  if(ui->cBox_SetVal_out1->isChecked()){
      ctrl |= (0X01<<4);
  }
  else{
      ctrl &= (~(0X01<<4));
  }

  if(ui->cBox_SetVal_out2->isChecked()){
      ctrl |= (0X01<<5);
  }
  else{
      ctrl &= (~(0X01<<5));
  }

  if(ui->cBox_SetVal_out3->isChecked()){
      ctrl |= (0X01<<6);
  }
  else{
      ctrl &= (~(0X01<<6));
  }

  if(ui->cBox_SetVal_out4->isChecked()){
      ctrl |= (0X01<<7);
  }
  else{
      ctrl &= (~(0X01<<7));
  }

  data[2] = ctrl;

  char MStautes = 0;

   if(flag == 1){  //前进
      ui->rB_LRun->setChecked(true);
      ui->rB_RRun->setChecked(true);
      MStautes |= (0X01<<0);
      MStautes |= (0X01<<2);
      data[3] = MStautes;
      int vspeed =  ui->lineEdit_Vspeed->text().toInt();
      if(abs(vspeed)>3500){
          vspeed = 3500;
      }
      vspeed = abs(vspeed);
      ui->lineEdit_Vspeed->setText(QString::number(vspeed));

      ui->lineEdit_Vangel->setText(QString::number(0));

      int speed = ui->lineEdit_Vspeed->text().toInt();
      int angle = ui->lineEdit_Vangel->text().toInt();

      ui->lineEdit_LFspeed->setText(QString::number(speed+(angle>>1)));
      ui->lineEdit_RFspeed->setText(QString::number(speed-(angle>>1)));


   }
   else if(flag ==2){//后退
       ui->rB_LRun->setChecked(true);
       ui->rB_RRun->setChecked(true);

       MStautes |= (0X01<<0);
       MStautes |= (0X01<<2);
       int vspeed =  ui->lineEdit_Vspeed->text().toInt();
       if(abs(vspeed)>3500){
           vspeed = 3500;
       }
       vspeed = 0-abs(vspeed);
       ui->lineEdit_Vspeed->setText(QString::number(vspeed));


       ui->lineEdit_Vangel->setText(QString::number(0));

       int speed = ui->lineEdit_Vspeed->text().toInt();
       int angle = ui->lineEdit_Vangel->text().toInt();

       ui->lineEdit_LFspeed->setText(QString::number(speed+(angle>>1)));
       ui->lineEdit_RFspeed->setText(QString::number(speed-(angle>>1)));

   }
   else if(flag ==3){//左转
       ui->rB_LRun->setChecked(true);
       ui->rB_RRun->setChecked(true);
       MStautes |= (0X01<<0);
       MStautes |= (0X01<<2);

       ui->lineEdit_Vspeed->setText(QString::number(0));

       int vangle =  ui->lineEdit_Vangel->text().toInt();
       if(abs(vangle)>2000){
           vangle = 2000;
       }
       vangle = abs(vangle);
       ui->lineEdit_Vangel->setText(QString::number(vangle));

       int speed = ui->lineEdit_Vspeed->text().toInt();
       int angle = ui->lineEdit_Vangel->text().toInt();

       ui->lineEdit_LFspeed->setText(QString::number(speed+(angle>>1)));
       ui->lineEdit_RFspeed->setText(QString::number(speed-(angle>>1)));

   }
   else if(flag ==4){//右转
       ui->rB_LRun->setChecked(true);
       ui->rB_RRun->setChecked(true);
       MStautes |= (0X01<<0);
       MStautes |= (0X01<<2);


       ui->lineEdit_Vspeed->setText(QString::number(0));

       int vangle =  ui->lineEdit_Vangel->text().toInt();
       if(abs(vangle)>2000){
           vangle = 2000;
       }
       vangle =0-abs(vangle);
       ui->lineEdit_Vangel->setText(QString::number(vangle));

       int speed = ui->lineEdit_Vspeed->text().toInt();
       int angle = ui->lineEdit_Vangel->text().toInt();

       ui->lineEdit_LFspeed->setText(QString::number(speed+(angle>>1)));
       ui->lineEdit_RFspeed->setText(QString::number(speed-(angle>>1)));

   }
   else {//停车
       ui->rB_LStop->setChecked(true);
       ui->rB_RStop->setChecked(true);
       MStautes &= (~(0X03<<2));
       MStautes &= (~(0X03<<0));
       ui->lineEdit_Vangel->setText(QString::number(0));
      ui->lineEdit_Vspeed->setText(QString::number(0));

      int speed = ui->lineEdit_Vspeed->text().toInt();
      int angle = ui->lineEdit_Vangel->text().toInt();

      ui->lineEdit_LFspeed->setText(QString::number(speed+(angle>>1)));
      ui->lineEdit_RFspeed->setText(QString::number(speed-(angle>>1)));

   }

    data[3] = MStautes;

   int LFspeed =  ui->lineEdit_LFspeed->text().toInt();
   if(LFspeed>3500){
       LFspeed = 3500;
   }
   else if (LFspeed<-3500){
        LFspeed = -3500;
   }
   char * ps1  = (char *)(&LFspeed);
   data[4] = * (ps1+1);
   data[5] = * (ps1+0);

   int RFspeed =  ui->lineEdit_RFspeed->text().toInt();
   if(RFspeed>3500){
       RFspeed = 3500;
   }
   else if (RFspeed<-3500){
        RFspeed = -3500;
   }
   char * ps2  = (char *)(&RFspeed);
   data[6] = * (ps2+1);
   data[7] = * (ps2+0);

  data[8] = 0;



  char ReCtrl = 0;
  if(ui->cBox_SetVal_Ral1->isChecked()){
      ReCtrl |= (0X01<<0);
  }
  else{
      ReCtrl &= (~(0X01<<0));
  }

  if(ui->cBox_SetVal_Ral2->isChecked()){
      ReCtrl |= (0X01<<1);
  }
  else{
      ReCtrl &= (~(0X01<<1));
  }

  data[9] = ReCtrl;

  int Fobtdis =   ui->lineEdit_Fobtdis->text().toInt();
  if(Fobtdis>254){
      Fobtdis = 254;
  }
  else if (Fobtdis<20){
       Fobtdis = 20;
  }
   char * ps3  = (char *)(&Fobtdis);
   data[10] = * (ps3+0);


   int Bobtdis =   ui->lineEdit_Bobtdis->text().toInt();
   if(Bobtdis>254){
       Bobtdis = 254;
   }
   else if (Bobtdis<20){
        Bobtdis = 20;
   }
    char * ps4  = (char *)(&Bobtdis);
    data[11] = * (ps4+0);


  data[12] = 0x0C;
  data[13] = 0x77;
  data[14] = 0xCC;

  std::string res;
  res.assign(data,15);
  return res;
}

void MainWindow::on_pushB_forward_clicked()
{
    //前进按钮

            std::string cmd= packctrlCMD(1);
	lastcmd = cmd;
              if(comalive == true){
                       writeData(cmd);
                  }
              else{
                      showStatusMessage(tr("please connect a com port first,Disconected now"));
                      //showStatusMessage(tr("Disconnected"));
                  }
}

void MainWindow::on_pushB_backward_clicked()
{
    //后退按钮

    std::string cmd= packctrlCMD(2);
	lastcmd = cmd;
      if(comalive == true){
               writeData(cmd);
          }
      else{
              showStatusMessage(tr("please connect a com port first,Disconected now"));
              //showStatusMessage(tr("Disconnected"));
          }
}

void MainWindow::on_pushB_rightward_clicked()
{
    //右转按钮

    std::string cmd= packctrlCMD(3);
	lastcmd = cmd;
      if(comalive == true){
               writeData(cmd);
          }
      else{
              showStatusMessage(tr("please connect a com port first,Disconected now"));
              //showStatusMessage(tr("Disconnected"));
          }
}

void MainWindow::on_pushB_leftward_clicked()
{
    //左转按钮

   std::string cmd= packctrlCMD(4);
	lastcmd = cmd;
      if(comalive == true){
               writeData(cmd);
          }
      else{
              showStatusMessage(tr("please connect a com port first,Disconected now"));
              //showStatusMessage(tr("Disconnected"));
          }
}

void MainWindow::on_pushB_stop_clicked()
{
    //停车按钮

    std::string cmd= packctrlCMD(5);
	lastcmd = cmd;
      if(comalive == true){
               writeData(cmd);
          }
      else{
              showStatusMessage(tr("please connect a com port first,Disconected now"));
              //showStatusMessage(tr("Disconnected"));
          }
}



void MainWindow::on_lineEdit_Vspeed_editingFinished()
{
    //修改了值
    int speed = ui->lineEdit_Vspeed->text().toInt();
    int angle = ui->lineEdit_Vangel->text().toInt();

    ui->lineEdit_LFspeed->setText(QString::number(speed+(angle>>1)));
    ui->lineEdit_RFspeed->setText(QString::number(speed-(angle>>1)));
}

void MainWindow::on_lineEdit_LFspeed_editingFinished()
{
    //修改了值
     int speed = (ui->lineEdit_LFspeed->text().toInt()+ui->lineEdit_RFspeed->text().toInt())>>1;
      ui->lineEdit_Vspeed->setText(QString::number(speed));
      int angle = (ui->lineEdit_LFspeed->text().toInt()-ui->lineEdit_RFspeed->text().toInt())>>1;
      ui->lineEdit_Vangel->setText(QString::number(angle));
}

void MainWindow::on_lineEdit_RFspeed_editingFinished()
{
    //修改了值
     int speed = (ui->lineEdit_LFspeed->text().toInt()+ui->lineEdit_RFspeed->text().toInt())>>1;
      ui->lineEdit_Vspeed->setText(QString::number(speed));
     int angle = (ui->lineEdit_LFspeed->text().toInt()-ui->lineEdit_RFspeed->text().toInt())>>1;
     ui->lineEdit_Vangel->setText(QString::number(angle));
}

void MainWindow::on_lineEdit_Vangel_editingFinished()
{
    //修改了值
    int speed = ui->lineEdit_Vspeed->text().toInt();
    int angle = ui->lineEdit_Vangel->text().toInt();

    ui->lineEdit_LFspeed->setText(QString::number(speed+(angle>>1)));
    ui->lineEdit_RFspeed->setText(QString::number(speed-(angle>>1)));
}
