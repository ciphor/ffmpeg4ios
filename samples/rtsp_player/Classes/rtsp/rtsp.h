
#if !defined( US_RTSP_H )
#define US_RTSP_H

#include "StdAfx.h"

const char base[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
const unsigned char amrnb_packed_size[16] = {12, 13, 15, 17, 19, 20, 26, 31, 5, 0, 0, 0, 0, 0, 0, 0};
const unsigned char amrwb_packed_size[16] = {18, 24, 33, 37, 41, 47, 51, 59, 61, 6, 6, 0, 0, 0, 1, 1};

#define SOCKBUFSIZE 65536
#define MAXRTPPAKCRTSIZE 1500
#define MINRTPPACKETSIZE 12

//Streaming Server Type
#define  SST_RTSP_GENERAL	0
#define  SST_RTSP_DARWIN	1
#define  SST_RTSP_HELIX		2
#define  SST_RTSP_DAHUA		3
#define  SST_RTSP_XUNLEI	4
#define  SST_RTSP_HISTREAM	5
#define  SST_RTSP_STARV		6

typedef struct{
	char url[256];
	char ip[256];
	int port;
	int fulltime;
	int	maxtime;
	void (*nl_notify)(int msgId, unsigned int wParam, unsigned int lParam);
}source_param_struct;

typedef struct{
	unsigned char*		iData; 	//帧数据，该数据由rtsp模块来分配，rtsp模块负责释放
	unsigned int		iLen; 	//当前包长度，
	unsigned int		iPTS;  	//Frame timestamp
	unsigned int		iFrameIndex; 	//当前帧序号

	unsigned char		iFrameType; 	//1->audio,2->video
	unsigned char		iIFrame; 		//如果是视频帧，判断当前是否为关键帧,如果音频帧，则无效
	unsigned int		iRealLen;
}TSourceFrame;

typedef struct{
	//AVP video format
	int			wFPS;				//帧率，一般在3-25之间
	int			wBitDepth;			//色深，一般在8，16，24，32
	int			wWidth;				//视频宽

	int			wHeight;			//视频高

	int			dwVideoCompress;	//1->MPEG4,2->H264,3->H263
	int 		dwVideoBitRate;		//视频平均码流,bps
	int			wVideoReserved;		//保留,当为点播时，保存该节目的音频总长，一般情况下和视频总长一致

}TAVPVideoInfo;

typedef struct {
	//	audio format
	int		wAudioFormat;		//参看1->amr-nb,2->amr-wb,3->mp3,4->aac
	int		wChannelNum;		//声道数，mono or stereo
	int		wBlockAlign;		//reserve
	int		wBitsPerSample;		//reserve
	int		wFrameTime;			//reserve
	int		wAudioReserved;		//保存该节目的音频总长，一般情况下和视频总长一致

	int		dwSamplesPerSec;	//reserve
	int		dwBitRate;			//平均码流
}TAVPAudioInfo;

typedef struct DataUnit {
	TSourceFrame* frame;
	int complete;
	DataUnit* next_unit;
}DataUnit;

typedef struct{

	char codec[32];
	char sps[256];
	int sps_len;
	char pps[256];
	int pps_len;

	unsigned int timestamp;
	unsigned int first_timestamp;
	unsigned int timestamp_range;
	unsigned int timestamp_step;
	
	int seq;
	int first_seq;
	int last_seq;
	int seq_lost;  //1:seq lost, if H264 skip all the frame until IDR 

	int lastiPTS;
	int fps;
	
	DataUnit *listHead;
	DataUnit *listEnd;
	
	int frame_num;
	int frame_cnt;

}RTPDepacker_video;

typedef struct{
	
	DataUnit *listHead;
	DataUnit *listEnd;
	int lastiPTS;
	
	int frame_num;
	int frame_cnt;

	unsigned int timestamp;
	unsigned int first_timestamp;
	unsigned int timestamp_step;
	unsigned int timestamp_range;

	int first_seq;
	int last_seq;
	int seq;

}RTPDepacker_audio;

typedef struct{
	TAVPVideoInfo videoInfo;
	TAVPAudioInfo audioInfo;
}TAVPMediaInfo;


typedef struct {
	char *rtp_buf; //store the uncomplete rtp packet
	int rtp_len;   //index the full size of this rtp packet
	int rtp_len_cur; //index the current size of the rtp  packet
	
	char rtpLen[4];  //store the 4 bytes of the Interleaved rtp packet header
	int rtpLen_cur;  //index the current size of the rtpLen 

	//content-base
	char content_base[256];		//Content-Base
	//data
	char session_id[64];  		//asigned by DarwinServer
	int video_flag;  			//1:video 0:no video
	char video_trackid[64]; 	//asigmed by DSS
	char video_codec[32]; 		//"H264"
	char video_info[256]; 		//the base64 coded sps and pps and other info

	unsigned char channel;
	unsigned char is_ready;		//is ready for receive raw audio/video data.  //xunlei need sps to be ready.
	unsigned char reserve2;
	unsigned char reserve3;
	int video_samplerate; 		
	int video_width;
	int video_height;
	int video_bitrate;
	float video_range;

	int audio_flag;
	char audio_trackid[64];
	char audio_codec[32];
	int audio_amrnb;
	char audio_info[256];
	int audio_samplerate;
	int audio_channels;	
	int audio_bitrate;
	float audio_range;

	unsigned int audio_ssrc;
	unsigned int video_ssrc;

	double range;
	double range_start;
	double range_end;

	int   server_type;  //rtsp server 

	RTPDepacker_video * videoData;
	RTPDepacker_audio * audioData;
}RTSPContext;

typedef struct {
	SOCKET sock;
	char *sock_buf;  //store the data from socket 
	int sock_len;    //index the size of the data	
}SocketContext;

typedef struct{
	//param
	char url[256];
	char ip[256];
	int port;

	RTSPContext *rtsp_context;
	SocketContext *socket_context;
	TAVPMediaInfo  mediaInfo;

}source_struct;

int GetStatus( source_struct *source );
int GetRTP( source_struct *source );

TSourceFrame *GetNextVideoFrame( source_struct *source );
TSourceFrame *GetNextAudioPacket( source_struct *source );
void FreeSourceFrame( TSourceFrame* frame );
TAVPMediaInfo *GetMediaInfo( source_struct *source );
void source_destroy( source_struct *source );
source_struct *source_create( source_param_struct * param );

#endif
