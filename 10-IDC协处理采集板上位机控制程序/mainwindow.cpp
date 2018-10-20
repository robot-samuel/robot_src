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
#include <QClipboard>


//��˸wave

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

    ui->cBox_SetVal_out1->setChecked(false);
    ui->cBox_SetVal_out2->setChecked(false);
    ui->cBox_SetVal_out3->setChecked(false);
    ui->cBox_SetVal_out4->setChecked(false);

	ui->cBox_SetVal_Ral1->setChecked(false);
	ui->cBox_SetVal_Ral2->setChecked(false);


	LED = new QButtonGroup(this);
	LED ->addButton(ui->cBox_B_LED_RED,0);
	LED ->addButton(ui->cBox_B_LED_GREEN,1);
	LED ->addButton(ui->cBox_B_LED_BLUE,2);
	LED ->addButton(ui->cBox_B_LED_REDW,3);
	LED ->addButton(ui->cBox_B_LED_GREENW,4);
	LED ->addButton(ui->cBox_B_LED_BLUEW,5);
	LED ->addButton(ui->cBox_B_LED_CLOSE,6);

	LED1 = new QButtonGroup(this);
	LED1 ->addButton(ui->cBox_B_LED_RED1,0);
	LED1 ->addButton(ui->cBox_B_LED_GREEN1,1);
	LED1 ->addButton(ui->cBox_B_LED_BLUE1,2);
	LED1 ->addButton(ui->cBox_B_LED_REDW1,3);
	LED1 ->addButton(ui->cBox_B_LED_GREENW1,4);
	LED1 ->addButton(ui->cBox_B_LED_BLUEW1,5);
	LED1 ->addButton(ui->cBox_B_LED_CLOSE1,6);

	
	ui->cBox_B_LED_RED1->setChecked(true);
	ui->cBox_B_LED_RED->setChecked(true);


	

    ui->lineEdit_LFspeed->setText("0");
    ui->lineEdit_RFspeed->setText("0");

    ui->lineEdit_Vspeed->setText("0");


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


	ui->lineEdit_TEMA->setText(QString::number(uart->TEMA,10));
	ui->lineEdit_HUMA->setText(QString::number(uart->HUMA,10));
	ui->lineEdit_TEMB->setText(QString::number(uart->TEMB,10));
	ui->lineEdit_HUMB->setText(QString::number(uart->HUMB,10));


	ui->lineEdit_CO->setText(QString::number(uart->CO_vau,'f',2));
	ui->lineEdit_EX->setText(QString::number(uart->EX_vau,'f',2));
	ui->lineEdit_O2->setText(QString::number(uart->O2_vau,'f',2));
	ui->lineEdit_H2S->setText(QString::number(uart->H2S_vau,'f',2));	


	ui->lineEdit_BSOC->setText(QString::number(uart->BOS,10));
	ui->lineEdit_BVOT->setText(QString::number(uart->VOT,'f',2));
	ui->lineEdit_CRROUENT->setText(QString::number(uart->CRROUENT,'f',2));

	ui->lineEdit_obt1_dis->setText(QString::number(uart->Obt1_dis,10));
	ui->lineEdit_obt2_dis->setText(QString::number(uart->Obt2_dis,10));	


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
	
    	//uart->recvbuffer+=strdata;
	recvsize+=size;
	
	uart->comRecvFlag = uart->procss_recv_buffer(strdata);
    	if(uart->comRecvFlag){
		 checksize++;

		 ui->lineEdit_L_Encoder->setText(QString::number(uart->LF_counter,10));
		 ui->lineEdit_R_Encoder->setText(QString::number(uart->RF_counter,10));

		 ui->lineEdit_LMotor_Status->setText(QString::number(uart->LFMotor_Status,10));
		 procosLFMotor_Status();
		 
		  ui->lineEdit_RMotor_Status->setText(QString::number(uart->RFMotor_Status,10));
		  procosRFMotor_Status();

		 ui->lineEdit_LMotor_Error->setText(QString::number(uart->LFMotor_error,10));
		  ui->lineEdit_RMotor_Error->setText(QString::number(uart->RFMotor_error,10));		 

	ui->lineEdit_TEMA->setText(QString::number(uart->TEMA,10));
	ui->lineEdit_HUMA->setText(QString::number(uart->HUMA,10));
	ui->lineEdit_TEMB->setText(QString::number(uart->TEMB,10));
	ui->lineEdit_HUMB->setText(QString::number(uart->HUMB,10));


	ui->lineEdit_CO->setText(QString::number(uart->CO_vau,'f',2));
	ui->lineEdit_EX->setText(QString::number(uart->EX_vau,'f',2));
	ui->lineEdit_O2->setText(QString::number(uart->O2_vau,'f',2));
	ui->lineEdit_H2S->setText(QString::number(uart->H2S_vau,'f',2));	


	ui->lineEdit_BSOC->setText(QString::number(uart->BOS,10));
	ui->lineEdit_BVOT->setText(QString::number(uart->VOT,'f',2));
	ui->lineEdit_CRROUENT->setText(QString::number(uart->CRROUENT,'f',2));

	ui->lineEdit_obt1_dis->setText(QString::number(uart->Obt1_dis,10));
	ui->lineEdit_obt2_dis->setText(QString::number(uart->Obt2_dis,10));	


		  procosCom_Status();
		  procosIO_Status();
		  
        }	


	
    ui->label_recvsize->setText(QString::number(recvsize,10)+":"+QString::number(recvsize/39,10)+":"+QString::number(checksize,10));
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
  char data[13];
  data[0] = 0x66;
  data[1] = 0xAA;


  char ctrl = 0;
  if(ui->cBox_B_LED_CLOSE->isChecked()){
     		ctrl &= (~0X03);
  }
  else if(ui->cBox_B_LED_RED->isChecked()){
      ctrl |= (0X01<<0);
  }
  else if(ui->cBox_B_LED_GREEN->isChecked()){
      ctrl |= (0X02<<0);
  }
  else if(ui->cBox_B_LED_BLUE->isChecked()){
      ctrl |= (0X03<<0);
  }
  else if(ui->cBox_B_LED_REDW->isChecked()){
      ctrl |= (0X04<<0);
  }
  else if(ui->cBox_B_LED_GREENW->isChecked()){
       ctrl |= (0X05<<0);
  }
   else if(ui->cBox_B_LED_BLUEW->isChecked()){
      ctrl |= (0X06<<0);
  }
  else{
      ctrl &= (~0X03);
  }

   if(ui->cBox_B_LED_CLOSE1->isChecked()){
     		ctrl &= (~(0X03<<3));
  }
  else if(ui->cBox_B_LED_RED1->isChecked()){
      ctrl |= (0X01<<3);
  }
  else if(ui->cBox_B_LED_GREEN1->isChecked()){
      ctrl |= (0X02<<3);
  }
  else if(ui->cBox_B_LED_BLUE1->isChecked()){
      ctrl |= (0X03<<3);
  }
  else if(ui->cBox_B_LED_REDW1->isChecked()){
      ctrl |= (0X04<<3);
  }
  else if(ui->cBox_B_LED_GREENW1->isChecked()){
       ctrl |= (0X05<<3);
  }
   else if(ui->cBox_B_LED_BLUEW1->isChecked()){
      ctrl |= (0X06<<3);
  }
  else{
     ctrl &= (~(0X03<<3));
  }

   if(ui->cBox_SetVal_Ral1->isChecked()){
      ctrl |= (0X01<<6);
  }
  else{
      ctrl &= (~(0X01<<6));
  }

  if(ui->cBox_SetVal_Ral2->isChecked()){
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
  if(ui->cBox_SetVal_out1->isChecked()){
      ReCtrl |= (0X01<<0);
  }
  else{
      ReCtrl &= (~(0X01<<0));
  }

  if(ui->cBox_SetVal_out2->isChecked()){
      ReCtrl |= (0X01<<1);
  }
  else{
      ReCtrl &= (~(0X01<<1));
  }

  if(ui->cBox_SetVal_out3->isChecked()){
      ReCtrl |= (0X01<<2);
  }
  else{
      ReCtrl &= (~(0X01<<2));
  }

  if(ui->cBox_SetVal_out4->isChecked()){
      ReCtrl |= (0X01<<3);
  }
  else{
      ReCtrl &= (~(0X01<<3));
  }


  data[9] = ReCtrl;

  data[10] = 0x0A;
  data[11] = 0x77;
  data[12] = 0xCC;

  std::string res;
  res.assign(data,13);
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
  char data[13];
  data[0] = 0x66;
  data[1] = 0xAA;


  char ctrl = 0;
 if(ui->cBox_B_LED_CLOSE->isChecked()){
     		ctrl &= (~0X03);
  }
  else if(ui->cBox_B_LED_RED->isChecked()){
      ctrl |= (0X01<<0);
  }
  else if(ui->cBox_B_LED_GREEN->isChecked()){
      ctrl |= (0X02<<0);
  }
  else if(ui->cBox_B_LED_BLUE->isChecked()){
      ctrl |= (0X03<<0);
  }
  else if(ui->cBox_B_LED_REDW->isChecked()){
      ctrl |= (0X04<<0);
  }
  else if(ui->cBox_B_LED_GREENW->isChecked()){
       ctrl |= (0X05<<0);
  }
   else if(ui->cBox_B_LED_BLUEW->isChecked()){
      ctrl |= (0X06<<0);
  }
  else{
      ctrl &= (~0X03);
  }

   if(ui->cBox_B_LED_CLOSE1->isChecked()){
     		ctrl &= (~(0X03<<3));
  }
  else if(ui->cBox_B_LED_RED1->isChecked()){
      ctrl |= (0X01<<3);
  }
  else if(ui->cBox_B_LED_GREEN1->isChecked()){
      ctrl |= (0X02<<3);
  }
  else if(ui->cBox_B_LED_BLUE1->isChecked()){
      ctrl |= (0X03<<3);
  }
  else if(ui->cBox_B_LED_REDW1->isChecked()){
      ctrl |= (0X04<<3);
  }
  else if(ui->cBox_B_LED_GREENW1->isChecked()){
       ctrl |= (0X05<<3);
  }
   else if(ui->cBox_B_LED_BLUEW1->isChecked()){
      ctrl |= (0X06<<3);
  }
  else{
     ctrl &= (~(0X03<<3));
  }

   if(ui->cBox_SetVal_Ral1->isChecked()){
      ctrl |= (0X01<<6);
  }
  else{
      ctrl &= (~(0X01<<6));
  }

  if(ui->cBox_SetVal_Ral2->isChecked()){
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
 if(ui->cBox_SetVal_out1->isChecked()){
      ReCtrl |= (0X01<<0);
  }
  else{
      ReCtrl &= (~(0X01<<0));
  }

  if(ui->cBox_SetVal_out2->isChecked()){
      ReCtrl |= (0X01<<1);
  }
  else{
      ReCtrl &= (~(0X01<<1));
  }

  if(ui->cBox_SetVal_out3->isChecked()){
      ReCtrl |= (0X01<<2);
  }
  else{
      ReCtrl &= (~(0X01<<2));
  }

  if(ui->cBox_SetVal_out4->isChecked()){
      ReCtrl |= (0X01<<3);
  }
  else{
      ReCtrl &= (~(0X01<<3));
  }


  data[9] = ReCtrl;

  data[10] = 0x0A;
  data[11] = 0x77;
  data[12] = 0xCC;

  std::string res;
  res.assign(data,13);
  return res;
}

std::string MainWindow::packctrlCMD(int flag)
{

   //打包命令
  char data[13];
  data[0] = 0x66;
  data[1] = 0xAA;


  char ctrl = 0;
  if(ui->cBox_B_LED_CLOSE->isChecked()){
     		ctrl &= (~0X03);
  }
  else if(ui->cBox_B_LED_RED->isChecked()){
      ctrl |= (0X01<<0);
  }
  else if(ui->cBox_B_LED_GREEN->isChecked()){
      ctrl |= (0X02<<0);
  }
  else if(ui->cBox_B_LED_BLUE->isChecked()){
      ctrl |= (0X03<<0);
  }
  else if(ui->cBox_B_LED_REDW->isChecked()){
      ctrl |= (0X04<<0);
  }
  else if(ui->cBox_B_LED_GREENW->isChecked()){
       ctrl |= (0X05<<0);
  }
   else if(ui->cBox_B_LED_BLUEW->isChecked()){
      ctrl |= (0X06<<0);
  }
  else{
      ctrl &= (~0X03);
  }

   if(ui->cBox_B_LED_CLOSE1->isChecked()){
     		ctrl &= (~(0X03<<3));
  }
  else if(ui->cBox_B_LED_RED1->isChecked()){
      ctrl |= (0X01<<3);
  }
  else if(ui->cBox_B_LED_GREEN1->isChecked()){
      ctrl |= (0X02<<3);
  }
  else if(ui->cBox_B_LED_BLUE1->isChecked()){
      ctrl |= (0X03<<3);
  }
  else if(ui->cBox_B_LED_REDW1->isChecked()){
      ctrl |= (0X04<<3);
  }
  else if(ui->cBox_B_LED_GREENW1->isChecked()){
       ctrl |= (0X05<<3);
  }
   else if(ui->cBox_B_LED_BLUEW1->isChecked()){
      ctrl |= (0X06<<3);
  }
  else{
     ctrl &= (~(0X03<<3));
  }

   if(ui->cBox_SetVal_Ral1->isChecked()){
      ctrl |= (0X01<<6);
  }
  else{
      ctrl &= (~(0X01<<6));
  }

  if(ui->cBox_SetVal_Ral2->isChecked()){
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



      int speed = ui->lineEdit_Vspeed->text().toInt();


      ui->lineEdit_LFspeed->setText(QString::number(speed));
      ui->lineEdit_RFspeed->setText(QString::number(speed));


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




       int speed = ui->lineEdit_Vspeed->text().toInt();


      ui->lineEdit_LFspeed->setText(QString::number(speed));
      ui->lineEdit_RFspeed->setText(QString::number(speed));

   }

   else {//停车
       ui->rB_LStop->setChecked(true);
       ui->rB_RStop->setChecked(true);
       MStautes &= (~(0X03<<2));
       MStautes &= (~(0X03<<0));
      ui->lineEdit_Vspeed->setText(QString::number(0));

      int speed = ui->lineEdit_Vspeed->text().toInt();


      ui->lineEdit_LFspeed->setText(QString::number(speed));
      ui->lineEdit_RFspeed->setText(QString::number(speed));

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
 if(ui->cBox_SetVal_out1->isChecked()){
      ReCtrl |= (0X01<<0);
  }
  else{
      ReCtrl &= (~(0X01<<0));
  }

  if(ui->cBox_SetVal_out2->isChecked()){
      ReCtrl |= (0X01<<1);
  }
  else{
      ReCtrl &= (~(0X01<<1));
  }

  if(ui->cBox_SetVal_out3->isChecked()){
      ReCtrl |= (0X01<<2);
  }
  else{
      ReCtrl &= (~(0X01<<2));
  }

  if(ui->cBox_SetVal_out4->isChecked()){
      ReCtrl |= (0X01<<3);
  }
  else{
      ReCtrl &= (~(0X01<<3));
  }


  data[9] = ReCtrl;

  data[10] = 0x0A;
  data[11] = 0x77;
  data[12] = 0xCC;

  std::string res;
  res.assign(data,13);
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


    ui->lineEdit_LFspeed->setText(QString::number(speed));
    ui->lineEdit_RFspeed->setText(QString::number(speed));
}

void MainWindow::on_lineEdit_LFspeed_editingFinished()
{
    //修改了值
    
     int speed = (ui->lineEdit_LFspeed->text().toInt()+ui->lineEdit_RFspeed->text().toInt())>>1;
      ui->lineEdit_Vspeed->setText(QString::number(speed));

}

void MainWindow::on_lineEdit_RFspeed_editingFinished()
{
    //修改了值
     int speed = (ui->lineEdit_LFspeed->text().toInt()+ui->lineEdit_RFspeed->text().toInt())>>1;
      ui->lineEdit_Vspeed->setText(QString::number(speed));

}


void MainWindow::procosLFMotor_Status()
{
	if((uart->LFMotor_Status&0x01)){
        ui->cBox_LF_SRDY->setChecked(true);
    }else{ui->cBox_LF_SRDY->setChecked(false);}

    if(((uart->LFMotor_Status>>1)&0x01)){
        ui->cBox_LF_ENA_SRV->setChecked(true);
    }else{ui->cBox_LF_ENA_SRV->setChecked(false);}

    if(((uart->LFMotor_Status>>2)&0x01)){
        ui->cBox_LF_RES1->setChecked(true);
    }else{ui->cBox_LF_RES1->setChecked(false);}

    if(((uart->LFMotor_Status>>3)&0x01)){
        ui->cBox_LF_FAULT->setChecked(true);
    }else{ui->cBox_LF_FAULT->setChecked(false);}

    if(((uart->LFMotor_Status>>4)&0x01)){
        ui->cBox_LF_RES2->setChecked(true);
    }else{ui->cBox_LF_RES2->setChecked(false);}

    if(((uart->LFMotor_Status>>5)&0x01)){
        ui->cBox_LF_EMGING->setChecked(true);
    }else{ui->cBox_LF_EMGING->setChecked(false);}


    if(((uart->LFMotor_Status>>6)&0x01)){
        ui->cBox_LF_DIS_SRV->setChecked(true);
    }else{ui->cBox_LF_DIS_SRV->setChecked(false);}

    if(((uart->LFMotor_Status>>7)&0x01)){
        ui->cBox_LF_WARN->setChecked(true);
    }else{ui->cBox_LF_WARN->setChecked(false);}
	
}
void MainWindow::procosRFMotor_Status()
{
	
	if((uart->RFMotor_Status&0x01)){
        ui->cBox_RF_SRDY->setChecked(true);
    }else{ui->cBox_RF_SRDY->setChecked(false);}

    if(((uart->RFMotor_Status>>1)&0x01)){
        ui->cBox_RF_ENA_SRV->setChecked(true);
    }else{ui->cBox_RF_ENA_SRV->setChecked(false);}

    if(((uart->RFMotor_Status>>2)&0x01)){
        ui->cBox_RF_RES1->setChecked(true);
    }else{ui->cBox_RF_RES1->setChecked(false);}

    if(((uart->RFMotor_Status>>3)&0x01)){
        ui->cBox_RF_FAULT->setChecked(true);
    }else{ui->cBox_RF_FAULT->setChecked(false);}

    if(((uart->RFMotor_Status>>4)&0x01)){
        ui->cBox_RF_RES2->setChecked(true);
    }else{ui->cBox_RF_RES2->setChecked(false);}

    if(((uart->RFMotor_Status>>5)&0x01)){
        ui->cBox_RF_EMGING->setChecked(true);
    }else{ui->cBox_RF_EMGING->setChecked(false);}


    if(((uart->RFMotor_Status>>6)&0x01)){
        ui->cBox_RF_DIS_SRV->setChecked(true);
    }else{ui->cBox_RF_DIS_SRV->setChecked(false);}

    if(((uart->RFMotor_Status>>7)&0x01)){
        ui->cBox_RF_WARN->setChecked(true);
    }else{ui->cBox_RF_WARN->setChecked(false);}
	
}



void MainWindow::procosCom_Status()
{

	if(((uart->Com_Status>>0)&0x01)){
        ui->cBox_Ldriver_Com->setChecked(true);
    }else{ui->cBox_Ldriver_Com->setChecked(false);}
	
	if(((uart->Com_Status>>1)&0x01)){
        ui->cBox_Rdriver_Com->setChecked(true);
    }else{ui->cBox_Rdriver_Com->setChecked(false);}

	if(((uart->Com_Status>>2)&0x01)){
        ui->checkBox_TEAMA->setChecked(true);
    }else{ui->checkBox_TEAMA->setChecked(false);}

	if(((uart->Com_Status>>3)&0x01)){
        ui->checkBox_TEAMB->setChecked(true);
    }else{ui->checkBox_TEAMB->setChecked(false);}

	if(((uart->Com_Status>>4)&0x01)){
        ui->cBox_obt1_Com->setChecked(true);
    }else{ui->cBox_obt1_Com->setChecked(false);}

	if(((uart->Com_Status>>5)&0x01)){
        ui->cBox_obt2_Com->setChecked(true);
    }else{ui->cBox_obt2_Com->setChecked(false);}

	if(((uart->Com_Status>>6)&0x01)){
        ui->cBox_bms_Com->setChecked(true);
    }else{ui->cBox_bms_Com->setChecked(false);}

	if(((uart->Com_Status>>7)&0x01)){
        ui->cBox_air_Com->setChecked(true);
    }else{ui->cBox_air_Com->setChecked(false);}
}

void MainWindow::procosIO_Status()
{

	if(((uart->IO_Status>>0)&0x01)){
        ui->cBox_vau_input1->setChecked(true);
    }else{ui->cBox_vau_input1->setChecked(false);}
	
	if(((uart->IO_Status>>1)&0x01)){
        ui->cBox_vau_input2->setChecked(true);
    }else{ui->cBox_vau_input2->setChecked(false);}

	if(((uart->IO_Status>>2)&0x01)){
        ui->cBox_vau_input3->setChecked(true);
    }else{ui->cBox_vau_input3->setChecked(false);}

	if(((uart->IO_Status>>3)&0x01)){
        ui->cBox_vau_input4->setChecked(true);
    }else{ui->cBox_vau_input4->setChecked(false);}

	if(((uart->IO_Status>>4)&0x01)){
        ui->cBox_vau_out1->setChecked(true);
    }else{ui->cBox_vau_out1->setChecked(false);}

	if(((uart->IO_Status>>5)&0x01)){
        ui->cBox_vau_out2->setChecked(true);
    }else{ui->cBox_vau_out2->setChecked(false);}

	if(((uart->IO_Status>>6)&0x01)){
        ui->cBox_vau_out3->setChecked(true);
    }else{ui->cBox_vau_out3->setChecked(false);}

	if(((uart->IO_Status>>7)&0x01)){
        ui->cBox_vau_out4->setChecked(true);
    }else{ui->cBox_vau_out4->setChecked(false);}
}


void MainWindow::on_listWidget_customContextMenuRequested(const QPoint &pos)
{

    QListWidgetItem* curItem = ui->listWidget->itemAt( pos );
        if( curItem == NULL )
            return;

        QMenu *popMenu = new QMenu( this );
        QAction *deleteSeed = new QAction(tr("Delete"), this);
        QAction *clearSeeds = new QAction(tr("Clear"), this);
        popMenu->addAction( deleteSeed );
        popMenu->addAction( clearSeeds );
        connect( deleteSeed, SIGNAL(triggered() ), this, SLOT( deleteSeedSlot()) );
        connect( clearSeeds, SIGNAL(triggered() ), this, SLOT( clearSeedsSlot()) );
		
        popMenu->exec( QCursor::pos() );
        delete popMenu;
        delete deleteSeed;
        delete clearSeeds;

}

void MainWindow::on_listWidget_return_customContextMenuRequested(const QPoint &pos)
{


    QListWidgetItem* curItem = ui->listWidget_return->itemAt( pos );
        if( curItem == NULL )
            return;

        QMenu *popMenu = new QMenu( this );
        QAction *CopySeed = new QAction(tr("Copy"), this);

        popMenu->addAction( CopySeed );
        connect( CopySeed, SIGNAL(triggered() ), this, SLOT( CopySeedSlot()) );

        popMenu->exec( QCursor::pos() );
        delete popMenu;
        delete CopySeed;
}

void MainWindow::deleteSeedSlot()
{
    int ch = QMessageBox::warning(NULL, "Warning",
                                  "Are you sure to delete seed ?",
                                  QMessageBox::Yes | QMessageBox::No,
                                  QMessageBox::No);

    if ( ch != QMessageBox::Yes )
        return;

    QListWidgetItem * item = ui->listWidget->currentItem();
    if( item == NULL )
        return;

    int curIndex = ui->listWidget->row(item);
    ui->listWidget->takeItem(curIndex);
    delete item;

}

void MainWindow::clearSeedsSlot()
{
    int ch = QMessageBox::warning(NULL, "Warning",
                                  "Are you sure to clear seeds ?",
                                  QMessageBox::Yes | QMessageBox::No,
                                  QMessageBox::No);

    if ( ch != QMessageBox::Yes )
        return;

    QListWidgetItem * item = ui->listWidget->currentItem();
    if( item == NULL )
        return;

    ui->listWidget->clear();
}

void MainWindow::CopySeedSlot()
{
   QListWidgetItem * item = ui->listWidget_return->currentItem();
    if( item == NULL )
        return;

    QString curIndex = ui->listWidget_return->currentItem()->text();
   //
    QClipboard *clipboard = QApplication::clipboard();   //?????????
    QString originalText = clipboard->text();	     //??????????
    
    std::cout<<"originalText="<<originalText.toStdString()<<std::endl;
	
    clipboard->setText(curIndex);		             //???????</span>

  //  delete item;
	
}


