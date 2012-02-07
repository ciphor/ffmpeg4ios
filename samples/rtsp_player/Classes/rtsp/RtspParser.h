// DhStreamParser.h: interface for the DhStreamParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DHSTREAMPARSER_H__4CB30E13_2FFF_4236_AEE3_E7BD20C8C173__INCLUDED_)
#define AFX_DHSTREAMPARSER_H__4CB30E13_2FFF_4236_AEE3_E7BD20C8C173__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FrameList.h"
#include "rtsp.h"

class RtspParser  
{
public:
	RtspParser();
	virtual ~RtspParser();
	int play();
	void stop();
	int create(const char * url);//,const char *address,int port);
	int create(const char * url ,int type);
	int getstatu();
	// 输入数据.
	//int InputData(unsigned char *pData, unsigned long nDataLength);
	//int InputDataEx(unsigned char *pData, unsigned long nDataLength);
	// 同步输出帧数据.
	MediaData_FRAME * GetNextFrame();
	// 调用这个等同于重找I帧,或者第一次调用的时候,找I帧.
	MediaData_FRAME * GetNextKeyFrame();
	void PutPacketToQueueVodEx(char* pcContent, int iLen);
	int Reset();
	
	MediaData_FRAME * GetNextAudioFrame();
	MediaData_FRAME* m_tmp_Frameinfo;//临时帧信息

	CFrameList m_FrameInfoList ;//帧信息链表
	CFrameList m_auFrameInfoList ;//音频帧信息链表
	MediaData_FRAME *m_FrameInfo;

	int statu;
	/*char * m_szDVRData;	//锐明的DVR_Data
	int  m_nCurLenDVRData; //当前DVR_Data的大小
	char * m_szPacket;	//锐明的DVR_Data
	int  m_nStartPos;	//当前位置*/
	
	source_struct * source;
    
    int GetMediaInfo(TAVPMediaInfo * pInfo);
	
private:
	//通过URL获取服务器地址，端口等信息
	//According to url get address, port
	//iType默认为0, 参考 SST_..
	int parseUrl(const char * url, int iType); 

	char m_address[256];
	int	 m_port;
	
};

#endif // !defined(AFX_DHSTREAMPARSER_H__4CB30E13_2FFF_4236_AEE3_E7BD20C8C173__INCLUDED_)
