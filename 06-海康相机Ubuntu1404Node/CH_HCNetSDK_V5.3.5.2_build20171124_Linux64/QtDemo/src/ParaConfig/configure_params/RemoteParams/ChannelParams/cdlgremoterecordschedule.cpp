
#include <QMessageBox>
#include "cdlgremoterecordschedule.h"
#include "publicfuc.h"


CDlgRemoteRecordSchedule::CDlgRemoteRecordSchedule(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);

	//¼�����ͣ�0-��ʱ¼��1-�ƶ����¼��2-����¼��3-�ƶ����򱨾�¼��4-�ƶ����ͱ���¼��5-�����¼��6-���ܱ���¼��
	QStringList strings;
	strings<<"Schedule"<<"Motion detect"<<"Alarm"<<"Motion or alarm"<<"Motion and alarm"<<"Command trigger"<<"VCA alarm";
	int i;
	for(i=0; i < 8; i++)
	{
		ui.vLayStartHour->addWidget(&(m_editStartHour[i]));
		ui.vLayStartMin->addWidget(&(m_editStartMin[i]));
		ui.vLayEndHour->addWidget(&(m_editEndHour[i]));
		ui.vLayEndMin->addWidget(&(m_editEndMin[i]));
		ui.vLayRecordType->addWidget(&(m_comboRecordType[i]));
		m_comboRecordType[i].addItems(strings);
	}
	ui.comboDayRecordType->addItems(strings);
	strings.clear();
	strings<<"Monday"<<"Tuesday"<<"Wednesday"<<"Thursday"<<"Friday"<<"Saturday"<<"Sunday";
	ui.comboWeekDay->addItems(strings);
	strings<<"Whole week";
	ui.comboCopyRecordDay->addItems(strings);

	m_pstruRecord = NULL;

	connect(ui.comboWeekDay, SIGNAL(activated(int)), this, SLOT(comboWeekDayChanged (int)));
    connect(ui.checkRecordAllDay, SIGNAL(clicked()), this, SLOT(clickRecordAllDay()));
}

CDlgRemoteRecordSchedule::~CDlgRemoteRecordSchedule()
{

}

void CDlgRemoteRecordSchedule::on_btnCopyDay_clicked()
{
	if(m_pstruRecord == NULL)
	{
	    QMessageBox::information(this, tr("Please check!"), tr("the m_pstruRecord is null!"));
	    return;
	}
	int iCopyDay;;
	int i;
	iCopyDay = ui.comboCopyRecordDay->currentIndex();

	if(iCopyDay < 7)//ֻ����ĳһ��
	{
		copyDaySetting(iCopyDay);
	}
	else //���Ƶ�������
	{
		for(i = 0; i < MAX_DAYS; i++)
		{
			copyDaySetting(i);
		}
	}
}

//���水ť�����
void CDlgRemoteRecordSchedule::on_btnSave_clicked()
{
	if(m_pstruRecord != NULL)
	{
		int iWeekDay;
		int i;
		int iTime;
		iWeekDay = ui.comboWeekDay->currentIndex();
		if(ui.checkRecordAllDay->isChecked())//ȫ��¼��
		{
			m_pstruRecord->struRecAllDay[iWeekDay].wAllDayRecord = 1;
			m_pstruRecord->struRecAllDay[iWeekDay].byRecordType = ( BYTE)(ui.comboDayRecordType->currentIndex());
		}
		else//ʱ���¼��
		{
			for(i = 0; i < MAX_TIMESEGMENT; i++)
			{
				//����
				m_pstruRecord->struRecordSched[iWeekDay][i].byRecordType = m_comboRecordType[i].currentIndex();
                //��ʼСʱ
				KIT_editToInt(&(m_editStartHour[i]), &iTime);
				m_pstruRecord->struRecordSched[iWeekDay][i].struRecordTime.byStartHour = iTime;
				//��ʼ����
				KIT_editToInt(&(m_editStartMin[i]), &iTime);
				m_pstruRecord->struRecordSched[iWeekDay][i].struRecordTime.byStartMin = iTime;
				//����Сʱ
				KIT_editToInt(&(m_editEndHour[i]), &iTime);
				m_pstruRecord->struRecordSched[iWeekDay][i].struRecordTime.byStopHour = iTime;
				//��������
				KIT_editToInt(&(m_editEndMin[i]), &iTime);
				m_pstruRecord->struRecordSched[iWeekDay][i].struRecordTime.byStopMin = iTime;
			}
		}
	}
	else
	{
	    QMessageBox::information(this, tr("Please check!"), tr("the m_pstruRecord is null!"));
	}
}

int CDlgRemoteRecordSchedule::comboWeekDayChanged(int iWeekDay)
{
	setEachWidget(iWeekDay);
	return HPR_OK;
}

int CDlgRemoteRecordSchedule::clickRecordAllDay()
{
	int i;
	if(ui.checkRecordAllDay->isChecked())
	{
		ui.comboDayRecordType->setEnabled(TRUE);
		for(i = 0; i <MAX_TIMESEGMENT_V30; i++)
		{
			m_comboRecordType[i].setEnabled(FALSE);
			m_editStartHour[i].setEnabled(FALSE);
			m_editStartMin[i].setEnabled(FALSE);
			m_editEndHour[i].setEnabled(FALSE);
			m_editEndMin[i].setEnabled(FALSE);
		}
	}
	else
	{
		ui.comboDayRecordType->setEnabled(FALSE);
		for(i = 0; i <MAX_TIMESEGMENT_V30; i++)
		{
			m_comboRecordType[i].setEnabled(TRUE);
			m_editStartHour[i].setEnabled(TRUE);
			m_editStartMin[i].setEnabled(TRUE);
			m_editEndHour[i].setEnabled(TRUE);
			m_editEndMin[i].setEnabled(TRUE);
		}
	}

	return HPR_OK;
}


//������ǰ���õ�ĳһ�죬�ڲ��ӿ�Ĭ��m_pstruRecord��Ϊ��
int CDlgRemoteRecordSchedule::copyDaySetting(int iWeekNo)
{
	int i;
	int iTime;

	if(ui.checkRecordAllDay->isChecked())
	{
		m_pstruRecord->struRecAllDay[iWeekNo].wAllDayRecord = 1;
		m_pstruRecord->struRecAllDay[iWeekNo].byRecordType = ( BYTE)(ui.comboDayRecordType->currentIndex());
	}
	else
	{
		for (i = 0; i < MAX_TIMESEGMENT_V30; i++)
		{
			//����
			m_pstruRecord->struRecordSched[iWeekNo][i].byRecordType = m_comboRecordType[i].currentIndex();
			//��ʼСʱ
			KIT_editToInt(&(m_editStartHour[i]), &iTime);
			m_pstruRecord->struRecordSched[iWeekNo][i].struRecordTime.byStartHour = iTime;
			//��ʼ����
			KIT_editToInt(&(m_editStartMin[i]), &iTime);
			m_pstruRecord->struRecordSched[iWeekNo][i].struRecordTime.byStartMin
					= iTime;
			//����Сʱ
			KIT_editToInt(&(m_editEndHour[i]), &iTime);
			m_pstruRecord->struRecordSched[iWeekNo][i].struRecordTime.byStopHour
					= iTime;
			//��������
			KIT_editToInt(&(m_editEndMin[i]), &iTime);
			m_pstruRecord->struRecordSched[iWeekNo][i].struRecordTime.byStopMin = iTime;
		}
	}

     return HPR_ERROR;
}

int CDlgRemoteRecordSchedule::SetStruRecord(NET_DVR_RECORD_V30 *pstruRecord)
{
	m_pstruRecord = pstruRecord;
	return HPR_ERROR;
}

//event
void CDlgRemoteRecordSchedule::showEvent ( QShowEvent * event )
{
	if(event ==NULL)
	{
	}
	setEachWidget(0);
}

int  CDlgRemoteRecordSchedule::setEachWidget(int iWeekDay)
{
	if(m_pstruRecord == NULL)
	{
	    QMessageBox::information(this, tr("Please check!"), tr("the m_pstruRecord is null!"));
	    return HPR_ERROR;
	}
	int iTemp;
	iTemp = m_pstruRecord->struRecAllDay[iWeekDay].byRecordType;
    ui.comboDayRecordType->setCurrentIndex(int(iTemp));
    iTemp = m_pstruRecord->struRecAllDay[iWeekDay].wAllDayRecord;
    if(iTemp == 0)//�Ƿ�ȫ��¼��
    {
    	ui.checkRecordAllDay->setCheckState(Qt::Unchecked);
    }
    else
    {
	   	ui.checkRecordAllDay->setCheckState(Qt::Checked);
	 }
     int i;
	 for(i = 0; i < MAX_TIMESEGMENT_V30; i++)
	 {
	   	//����
	   	iTemp = m_pstruRecord->struRecordSched[iWeekDay][i].byRecordType;
	   	m_comboRecordType[i].setCurrentIndex(iTemp);
	   	//ʱ��
    	iTemp = m_pstruRecord->struRecordSched[iWeekDay][i].struRecordTime.byStartHour;
    	KIT_intToEdit(&(m_editStartHour[i]), iTemp);
    	iTemp = m_pstruRecord->struRecordSched[iWeekDay][i].struRecordTime.byStartMin;
	   	KIT_intToEdit(&(m_editStartMin[i]), iTemp);
	   	iTemp = m_pstruRecord->struRecordSched[iWeekDay][i].struRecordTime.byStopHour;
	    KIT_intToEdit(&(m_editEndHour[i]), iTemp);
        iTemp = m_pstruRecord->struRecordSched[iWeekDay][i].struRecordTime.byStopMin;
        KIT_intToEdit(&(m_editEndMin[i]), iTemp);
	 }

	return HPR_OK;
}


