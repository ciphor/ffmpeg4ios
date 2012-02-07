// RtspParser.h: interface for the DhStreamParser class.
// Parser是多线程工作，一方面取数据，一方面对外提供数据
// 需要进行同步处理
//
//////////////////////////////////////////////////////////////////////

#if !defined(US_PARSER_RAW264__INCLUDED_)
#define US_PARSER_RAW264__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
//#include "us_kode.h"
//#include "us_core_parser.h"
//#include "us_parser.h"
#include "FrameList.h"

class Raw264Parser //: public IUSParser 
{
public:
	Raw264Parser();
	virtual ~Raw264Parser();
	// 输入数据.
	//int InputData(unsigned char *pData, unsigned long nDataLength);
	//int InputDataEx(unsigned char *pData, unsigned long nDataLength);
	void PutPacketToQueueVodEx(char* pcContent, int iLen);

public:
	//IUSParser

	//Start Play
	int		Play();

	//Stop
	void	Stop();

	//Create source
	int Prepare(const char * Tempip, int Tempport,const char * Tempusername,const char * Temppassword,int Tmepchannelid,int TempStreamType)
	{
		return 0;
	}
	
	int Prepare(const char * Tempuri, int Temptype);

	//Get status
	int	 GetStatus();
	void SetStatus(int Playstatu)
	{
		return;
	}

	//Gettype
	int		gettype();

	//finish
	int	Reset();

	//PTZ控制 PTZ命令 PTZ数据 通道
	int DoPtzControl(int PtzCmd,int PtzData,int ChannelID) 
	{
		return 0;
	}
	
	void SetCompanyIdentity(char * TempcompanyIdentity)
	{
		return;
	}

	//返回0设置失败 返回1设置成功
	//int SetDeviceChanelInfo(ChannelInfoSeting ChannelInfo)
	//{
	//	return 0;
	//}

	//	
	// 同步输出帧数据.
	MediaData_FRAME * GetNextFrame();
	MediaData_FRAME * GetNextAudioFrame();
	
	// 调用这个等同于重找I帧,或者第一次调用的时候,找I帧.
	MediaData_FRAME * GetNextKeyFrame();

	//************点播相关***************
	//virtual int Play(unsigned char * VideoFilename) = 0;//videofilename需要播放的录像文件
	//需要播放的录像结构体
//	int Play(Video_File VideoFile)
//	{
//		return 0;
//	}
	
	//SeekTime需要seek的时间
//	virtual int Seek(Date_Time SeekTime)
//	{
//		return 0;
//	}

	//暂停
	virtual int Pause()
	{
		return 0;
	}

	virtual int Resume()
	{
		return 0;
	}
	
	//搜索录像文件channel 从0开始 FileType查询的录像类型
	//1：常规录像(定时录像) 2：报警录像(移动报警录像)，3 ：所有录像 4、手动录像 5、探头报警录像
	//这里面需要做发送搜索数据，同时接受完成搜索的列表 之后才能返回 返回收到的数据长度 如果失败返回-1
	//virtual int SearchVideoFile(Date_Time StartTime,Date_Time EndTime,int channel,int FileType)
	//{
	//	return 0;
	//};
	//************点播相关***************

	//获取下一个录像文件
	//Video_File * GetNextVideoFile()
	//{
	//	return NULL;
	//}
	
	//获取下一个报警记录
	//Alarm_Struct * GetNextAlarm()
	//{
	//	return NULL;
	//}

public:
	int statu;
	CFrameList m_FrameInfoList ;//帧信息链表
	MediaData_FRAME *m_FrameInfo;
	CFrameList m_auFrameInfoList ;//音频帧信息链表

	char m_url[512];
	int	 m_Type; 
	


protected:
	//////////////////////////////////////////////////////////////////////////
	//

	MediaData_FRAME* m_tmp_Frameinfo;//临时帧信息


	/*char * m_szDVRData;	//锐明的DVR_Data
	int  m_nCurLenDVRData; //当前DVR_Data的大小
	char * m_szPacket;	//锐明的DVR_Data
	int  m_nStartPos;	//当前位置*/
private:
	//通过URL获取服务器地址，端口等信息
	//According to url get address, port
	//iType默认为0, 参考 SST_..
	int parseUrl(const char * url, int iType); 

	char m_address[256];
	int	 m_port;	

	//US_THREAD_T		m_Thread;
};

#endif // !defined(AFX_DHSTREAMPARSER_H__4CB30E13_2FFF_4236_AEE3_E7BD20C8C173__INCLUDED_)
