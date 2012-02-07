/*************************************************
Copyright (C), 1998-2009
File name:      	CVideoDecode.h
Author:         	dongchaomissyou
Version:        	V1.0
Date:				2009-05-04
Description:   		视频解码类,支持常用的解码算法
					如MPEG4、MPEG2、MJPEG、H264等
*************************************************/
#if !defined(_VIDEO_DEC_H)
#define _VIDEO_DEC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#ifdef __cplusplus
}
#endif	


typedef struct CVideoDecode
{
	
	AVCodec*			m_pAVCodec;							// ffmpeg定义的结构
	AVCodecContext*		m_pAVCodecContext;					// ffmpeg定义的结构
	struct SwsContext*			m_pSwsContext;
	AVFrame*			m_pPicture;							// ffmpeg定义的结构
	AVFrame*			m_pFrameYUV;						// ffmpeg定义的结构
	AVPacket			avpkt;                              //add by wayde
	uint8_t*			m_pVideoBuffer;						// 解码器内部存放解码后数据的内存区地址
	int					m_nGetPicture;						// 解码一帧成功后此值为1，失败此值为0
	int					m_nVideoFormat;						// 解码后视频格式
	int				m_bSupportIFrame;					// 是否支持I帧输出
	
	int				m_bInitSuccess;						// 初始化成功标志
	
}CVIDEO_DECODE, *PCVIDEO_DECODE;

#ifdef __cplusplus
extern "C" {
#endif
	
// 初始化解码器
int	InitDecode(PCVIDEO_DECODE pvideo, int nVideoWidth,int nVideoHeight,enum CodecID emDecCodeID,int nVideoFormat );

// 反初始化解码器
int		UnInitDecode(PCVIDEO_DECODE pvideo);

// 解码函数
int AVDecode(PCVIDEO_DECODE pvideo, unsigned char *pInputBuffer,int nInputSize,unsigned char **ppOutputBuffer,int* bKeyFrame);
void SetSupportIFrame(PCVIDEO_DECODE pvideo, int bSupportIFrame);

void InitRgbYuv();
	void Yuv12ToRgb32(int iWidth, int iHeight, unsigned char * pYuv, unsigned char * pRgb);
	void Yuv12ToRgb24(int iWidth, int iHeight, unsigned char * pYuv, unsigned char * pRgb);	
	void ConvertYUVtoRGB(unsigned char *src0,unsigned char *src1,unsigned char *src2,unsigned char *dst_ori,int width, int height);
	void InitConvertTable();
	
	void YUV420_RGB32(unsigned char * pRGB, unsigned char* pYUV, int Width, int Height);
	void InitColorSpace();
	
#ifdef __cplusplus
}
#endif

#endif
