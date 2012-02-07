// VideoDecode.cpp: implementation of the CVideoDecode class.
//
//////////////////////////////////////////////////////////////////////

#include "VideoDecode.h"
	

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*************************************************
Function:		CVideoDecode    
Description:	ππ‘Ï∫Ø ˝
Parameters:		ø’
Return:			ø’
use:			CFilePlayer
Call:  
Called By:
IO:      
Last Modified:	2009-04-21       
Others:       
*************************************************/
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)					{if(p != NULL) { delete (p);     (p) = NULL; } }   //Delete object by New create 
#endif

#ifndef SAFE_DELETEA	
#define SAFE_DELETEA(p)					{if(p != NULL) { delete[] (p);   (p) = NULL; } }   //Delete Arrary
#endif

#ifndef CheckPointer
#define CheckPointer(pointer,hr)		{if(pointer == NULL) return hr;};
#endif

#define TRUE    1
#define FALSE   0


void SetSupportIFrame(PCVIDEO_DECODE pvideo, int bSupportIFrame)
{
	pvideo->m_bSupportIFrame = bSupportIFrame;
}

/*************************************************
Function:		UnInitDecode    
Description:	∑¥≥ı ºªØ ”∆µΩ‚¬Î∆˜     
Parameters:		ø’
Return:			int   «∑Òµ˜”√≥…π¶£¨TRUE±Ì æ≥…π¶,∆‰À¸ ß∞‹ 
use:
				m_pFilePlayer    u
Call:
Called By:		~CVideoDecode()
IO:      
Last Modified:	2009-04-21
Others:       
*************************************************/
int UnInitDecode(PCVIDEO_DECODE pvideo, int videoFormat)
{    
	   pvideo->m_nVideoFormat		= videoFormat;//PIX_FMT_YUV420P;
		pvideo->m_pAVCodecContext	= NULL;
		pvideo->m_pSwsContext		= NULL;
	  pvideo->m_pPicture			= NULL;
		pvideo->m_pFrameYUV			= NULL;
		pvideo->m_pAVCodec			= NULL;
		pvideo->m_pVideoBuffer		= NULL;
		pvideo->m_bInitSuccess		= 0;
		pvideo->m_bSupportIFrame	= 1;
		pvideo->m_bInitSuccess = 0;
		
#if 0    
	SAFE_DELETEA(pvideo->m_pVideoBuffer);
	if (pvideo->m_pAVCodecContext)
	{
		avcodec_close(pvideo->m_pAVCodecContext);
		av_free(pvideo->m_pAVCodecContext);
		pvideo->m_pAVCodecContext = NULL;
	}
	if(pvideo->m_pPicture)
	{
		av_free(pvideo->m_pPicture);
		pvideo->m_pPicture = NULL;
	}
#else
		SAFE_DELETEA(pvideo->m_pVideoBuffer);
		
		if(pvideo->m_pAVCodecContext)
		{
			avcodec_close(pvideo->m_pAVCodecContext);
			av_free(pvideo->m_pAVCodecContext);
			pvideo->m_pAVCodecContext = NULL;
		}

		if(pvideo->m_pSwsContext)
		{
			sws_freeContext(pvideo->m_pSwsContext);
			pvideo->m_pSwsContext = NULL;
		}
		
		if(pvideo->m_pPicture)
		{
			av_free(pvideo->m_pPicture);
			pvideo->m_pPicture = NULL;
		}
		
		if(pvideo->m_pFrameYUV)
		{
			av_free(pvideo->m_pFrameYUV);
			pvideo->m_pFrameYUV = NULL;
		}
#endif
	return TRUE;
}

/*************************************************
Function:		InitDecode    
Description:	≥ı ºªØ ”∆µΩ‚¬Î∆˜     
Parameters:		nVideoWidth		in	 ”∆µÕºœÒøÌ∂»
				nVideoHeight	in	 ”∆µÕºœÒ∏ﬂ∂»
				nDecCodeID		in	Ω‚¬Î∆˜¿‡–Õ
				nVideoFormat	in	 ”∆µÕºœÒ∏Ò Ω

Return:			int µ˜”√ «∑Ò≥…π¶. RUE±Ì æ≥…π¶,∆‰À¸ ß∞‹ 
use:
				m_pFilePlayer    u
Call:
Called By:		
IO:      
Last Modified:	2009-04-21
Others:       
*************************************************/
int InitDecode(PCVIDEO_DECODE pvideo, int nVideoWidth,int nVideoHeight,enum CodecID emDecCodeID,int nVideoFormat)
{
     
	UnInitDecode (pvideo, nVideoFormat);
	
#if 0
		avcodec_init();
	avcodec_register_all();
	
	av_init_packet(&(pvideo->avpkt));
	pvideo->m_pAVCodec = avcodec_find_decoder(emDecCodeID);
	if(!(pvideo->m_pAVCodec))
	{
		fprintf(stderr, "Codec not found.\n");
		return -1;
	}
	
	pvideo->m_pAVCodecContext = avcodec_alloc_context();
	pvideo->m_pPicture = avcodec_alloc_frame();
	pvideo->m_pAVCodecContext->coded_height = nVideoWidth; 
	pvideo->m_pAVCodecContext->coded_width = nVideoHeight;
	pvideo->m_pAVCodecContext->width = nVideoWidth; 
	pvideo->m_pAVCodecContext->height = nVideoHeight; 
	if(pvideo->m_pAVCodec->capabilities&CODEC_CAP_TRUNCATED)
		pvideo->m_pAVCodecContext->flags|= CODEC_FLAG_TRUNCATED;
	
	if (avcodec_open(pvideo->m_pAVCodecContext, pvideo->m_pAVCodec) < 0) {
		fprintf(stderr, "could not open codec\n");
		return FALSE;
	}

	if (pvideo->m_pVideoBuffer == NULL)
		pvideo->m_pVideoBuffer = new uint8_t[nVideoWidth * nVideoHeight * 3 / 2];
	if(pvideo->m_pVideoBuffer == NULL)
	{
		return FALSE;
	}
	
// 	if(avpicture_fill((AVPicture *)m_pFrameYUV,m_pVideoBuffer, (PixelFormat)nVideoFormat,nVideoWidth, nVideoHeight) < 0)
// 	{
// 		return FALSE;
// 	}
		
	pvideo->m_bInitSuccess = TRUE;

	return TRUE;
#else
	pvideo->m_nVideoFormat		= nVideoFormat;
	pvideo->m_pAVCodecContext	= NULL;

	avcodec_init();				// ≥ı ºªØcodec
	avcodec_register_all();		// ◊¢≤·codec

	// CODEC_ID_H264°¢CODEC_ID_MPEG4°¢CODEC_ID_MPEG2VIDEO°¢CODEC_ID_MJPEGµ»
	pvideo->m_pAVCodec			= avcodec_find_decoder(emDecCodeID);
	if (pvideo->m_pAVCodec == NULL) 
	{
		return FALSE;
	}
	
	pvideo->m_pAVCodecContext	= avcodec_alloc_context();

	pvideo->m_pPicture			= avcodec_alloc_frame();
	pvideo->m_pFrameYUV			= avcodec_alloc_frame();

	if(pvideo->m_pAVCodecContext == NULL)
	{
		return FALSE;
	}

	if(pvideo->m_pFrameYUV == NULL || pvideo->m_pPicture == NULL)
	{
		return FALSE;
	}

	// ƒ¨»œ≈‰÷√H264 CODEC CONTEXT–≈œ¢
	pvideo->m_pAVCodecContext->codec_type	= AVMEDIA_TYPE_VIDEO;
	pvideo->m_pAVCodecContext->codec_id		= emDecCodeID;
	pvideo->m_pAVCodecContext->width		= nVideoWidth;
	pvideo->m_pAVCodecContext->height		= nVideoHeight;
	pvideo->m_pAVCodecContext->pix_fmt		= (PixelFormat)nVideoFormat;
    
	int nRes = 0;
	if ((nRes = avcodec_open(pvideo->m_pAVCodecContext, pvideo->m_pAVCodec)) < 0) 
	{
		pvideo->m_pAVCodecContext = NULL;
		
		return FALSE;
    }
	
	if (pvideo->m_pVideoBuffer == NULL)
		pvideo->m_pVideoBuffer = new uint8_t[nVideoWidth * nVideoHeight * 3 / 2];
	if(pvideo->m_pVideoBuffer == NULL)
	{
		return FALSE;
	}

	if(avpicture_fill((AVPicture *)pvideo->m_pFrameYUV,pvideo->m_pVideoBuffer, (PixelFormat)nVideoFormat,nVideoWidth, nVideoHeight) < 0)
	{
		return FALSE;
	}
		pvideo->m_bInitSuccess = TRUE;
#endif
	return TRUE; 
}

/*************************************************
Function:		AVDecode    
Description:	Ω‚¬Î—πÀıµƒ ”∆µ ˝æ›     
Parameters:		pInputBuffer		in	–Ë“™Ω‚¬Îµƒ ”∆µ—πÀı ˝æ›
				nInputSize			in	 ‰»ÎµƒΩ‚¬Î ˝æ›µƒ≥§∂»
				ppOutputBuffer		out	±£¥ÊΩ‚¬Î ‰≥ˆµƒ‘≠ º ”∆µ ˝æ›µƒª∫¥Ê
				bKeyFrame			out	 «∑Ò «πÿº¸÷°
				
Return:			int µ˜”√ «∑Ò≥…π¶. RUE±Ì æ≥…π¶,∆‰À¸ ß∞‹ 
use:
				m_pFilePlayer    u
Call:
Called By:		
IO:      
Last Modified:	2009-04-21
Others:       
*************************************************/
int AVDecode(PCVIDEO_DECODE pvideo, unsigned char *pInputBuffer,int nInputSize,unsigned char **ppOutputBuffer,int* bKeyFrame)
{    
	// –£—È—πÀı ˝æ›
	CheckPointer(pInputBuffer,FALSE);
	if(nInputSize <= 0)
		return FALSE;

	// Œ¥≥ı ºªØΩ· ¯,≤ª‘ –ÌΩ‚¬Î
	if(!(pvideo->m_bInitSuccess))
		return FALSE;
		
#if 0
	int got_picture;
	int ret;
	int i, j;
	pvideo->avpkt.size = nInputSize;
	pvideo->avpkt.data = pInputBuffer;
	int width = pvideo->m_pAVCodecContext->width;
	int height = pvideo->m_pAVCodecContext->height;
	while(pvideo->avpkt.size > 0)
	{
		int len = avcodec_decode_video2(pvideo->m_pAVCodecContext, pvideo->m_pPicture, &got_picture, &(pvideo->avpkt));
		if (len <= 0) { 
			return FALSE;
		}
		if (got_picture) {
			uint8_t* pp = pvideo->m_pPicture->data[0] + 0*(width+32) + 16;
			uint8_t* pu = pvideo->m_pPicture->data[1] + 0*(width/2+16) + 8;
			uint8_t* pv = pvideo->m_pPicture->data[2] + 0*(width/2+16) + 8;
//			printf("%d %d %d\n", pvideo->m_pPicture->linesize[0], pvideo->m_pPicture->linesize[1], pvideo->m_pPicture->linesize[2]);
			unsigned char *outBuf = new unsigned char[width*height*4];
			for (j =0; j<height; j++)
			{				
				memcpy(outBuf+j*width, pp+j*(width+32), width);
				if (!(j&1))
				{
					memcpy(outBuf+width*height+(j/2)*(width/2), pu+(j/2)*(width/2+16), width/2);
					memcpy(outBuf+width*height*5/4+(j/2)*(width/2), pv+(j/2)*(width/2+16), width/2);
				}
			}	    
			//memcpy(*ppOutputBuffer, outBuf, width*height*3/2);
			//delete [] outBuf;
			*ppOutputBuffer = outBuf;       
		}     
		pvideo->avpkt.size -= len;
		pvideo->avpkt.data += len;
	}
	if(pvideo->m_bSupportIFrame)
	{
		// πÿº¸÷°
		*bKeyFrame = pvideo->m_pAVCodecContext->coded_frame->key_frame;
	}
	else
	{
		*bKeyFrame = TRUE;
	}
				
	return TRUE;
#else

	int nRes = 0;
	pvideo->avpkt.size = nInputSize;
	pvideo->avpkt.data = pInputBuffer;

	if ((nRes = avcodec_decode_video2(pvideo->m_pAVCodecContext, pvideo->m_pPicture, &(pvideo->m_nGetPicture),&(pvideo->avpkt))) <= 0) 
	{
		return FALSE;
	}		
	if (pvideo->m_nGetPicture)
	{
		if(pvideo->m_pSwsContext == NULL)
		{
			pvideo->m_pSwsContext = sws_getContext(  pvideo->m_pAVCodecContext->width,
				pvideo->m_pAVCodecContext->height,
				pvideo->m_pAVCodecContext->pix_fmt,			
				pvideo->m_pAVCodecContext->width,
				pvideo->m_pAVCodecContext->height,			
				(PixelFormat)pvideo->m_nVideoFormat,
				/*SWS_FAST_BILINEAR*/SWS_BICUBIC,
				NULL, NULL, NULL  );
			CheckPointer(pvideo->m_pSwsContext,FALSE);
		}
		
		sws_scale(pvideo->m_pSwsContext,pvideo->m_pPicture->data,	
			pvideo->m_pPicture->linesize,
			0,
			pvideo->m_pAVCodecContext->height,
			pvideo->m_pFrameYUV->data,
			pvideo->m_pFrameYUV->linesize);
		
		*ppOutputBuffer = pvideo->m_pFrameYUV->data[0];
	   	
		
		//AVPicture outpicture;
		//outpicture.data[0] = * ppOutputBuffer;
		//outpicture.linesize[0] = ((AVCodecContext *) (pvideo->m_pAVCodecContext))->width*4;
		//YUV2RGBFUNC(yuv2rgb_c_32, long, 0);
					
		if(pvideo->m_bSupportIFrame)
		{
			// πÿº¸÷°
			*bKeyFrame = 1;//pvideo->m_pAVCodecContext->coded_frame->key_frame;
		}
		else
		{
			*bKeyFrame = TRUE;
		}
				
		return TRUE;
	}
	
	
#endif
	
	return FALSE;
}

// Convert from RGB to YUV420
int RGB2YUV_YR[256], RGB2YUV_YG[256], RGB2YUV_YB[256];
int RGB2YUV_UR[256], RGB2YUV_UG[256], RGB2YUV_UBVR[256];
int RGB2YUV_VG[256], RGB2YUV_VB[256];
// Conversion from YUV420 to RGB24
static long int crv_tab[256];
static long int cbu_tab[256];
static long int cgu_tab[256];
static long int cgv_tab[256];
static long int tab_76309[256];
static unsigned char clp[1024];   //for clip in CCIR601

//
// Table used for RGB to YUV420 conversion
//
void InitLookupTable()
{
	int i;
	for (i = 0; i < 256; i++) RGB2YUV_YR[i] = (float)65.481 * (i<<8);
	for (i = 0; i < 256; i++) RGB2YUV_YG[i] = (float)128.553 * (i<<8);
	for (i = 0; i < 256; i++) RGB2YUV_YB[i] = (float)24.966 * (i<<8);
	for (i = 0; i < 256; i++) RGB2YUV_UR[i] = (float)37.797 * (i<<8);
	for (i = 0; i < 256; i++) RGB2YUV_UG[i] = (float)74.203 * (i<<8);
	for (i = 0; i < 256; i++) RGB2YUV_VG[i] = (float)93.786 * (i<<8);
	for (i = 0; i < 256; i++) RGB2YUV_VB[i] = (float)18.214 * (i<<8);
	for (i = 0; i < 256; i++) RGB2YUV_UBVR[i] = (float)112 * (i<<8);
}

//
//Initialize conversion table for YUV420 to RGB
//
void InitConvertTable()
{
	long int crv,cbu,cgu,cgv;
	int i,ind;   
	
	crv = 104597; cbu = 132201;  /* fra matrise i global.h */
	cgu = 25675;  cgv = 53279;
	
	for (i = 0; i < 256; i++) {
		crv_tab[i] = (i-128) * crv;
		cbu_tab[i] = (i-128) * cbu;
		cgu_tab[i] = (i-128) * cgu;
		cgv_tab[i] = (i-128) * cgv;
		tab_76309[i] = 76309*(i-16);
	}
	
	for (i=0; i<384; i++)
		clp[i] =0;
	ind=384;
	for (i=0;i<256; i++)
		clp[ind++]=i;
	ind=640;
	for (i=0;i<384;i++)
		clp[ind++]=255;
}



void ConvertYUVtoRGB(unsigned char *src0,unsigned char *src1,unsigned char *src2,unsigned char *dst_ori,int width, int height)
{
	extern long int crv_tab[];
	extern long int cbu_tab[];
	extern long int cgu_tab[]; 
	extern long int cgv_tab[];
	extern long int tab_76309[]; 
	int y11, y21;
	int y12, y22;
	int y13, y23;
	int y14, y24;
	int u, v;
	int i, j;
	int c11, c21, c31, c41;
	int c12, c22, c32, c42;
	unsigned int DW;
	unsigned int *id1, *id2;
	unsigned char *py1, *py2, *pu, *pv;
	unsigned char *d1, *d2;
	
	d1 = dst_ori;
	d1 += width * height * 3 - width * 3;
	d2 = d1 - width * 3;
	
	py1 = src0;
	pu = src1;
	pv = src2;
	py2 = py1 + width;
	
	id1 = (unsigned int *) d1;
	id2 = (unsigned int *) d2; 
	for (j = 0; j < height; j += 2)
		
		
	{
		/* line j + 0 */
		for (i = 0; i < width; i += 4)
		{
			u = *pu++;
			v = *pv++;
			c11 = crv_tab[v];
			c21 = cgu_tab[u];
			c31 = cgv_tab[v];
			c41 = cbu_tab[u];
			u = *pu++;
			v = *pv++;
			c12 = crv_tab[v];
			c22 = cgu_tab[u];
			c32 = cgv_tab[v];
			c42 = cbu_tab[u];
			
			y11 = tab_76309[*py1++];  /* (255/219)*65536 */
			y12 = tab_76309[*py1++];
			y13 = tab_76309[*py1++];  /* (255/219)*65536 */
			y14 = tab_76309[*py1++];
			
			y21 = tab_76309[*py2++];
			y22 = tab_76309[*py2++];
			y23 = tab_76309[*py2++];
			y24 = tab_76309[*py2++];
			
			/* RGBR */
			DW = ((clp[(y11 + c41) >> 16])) |
			((clp[(y11 - c21 - c31) >> 16]) << 8) |
			((clp[(y11 + c11) >> 16]) << 16) |
			((clp[(y12 + c41) >> 16]) << 24);
			*id1++ = DW;
			
			/* GBRG */
			DW = ((clp[(y12 - c21 - c31) >> 16])) |
			((clp[(y12 + c11) >> 16]) << 8) |
			((clp[(y13 + c42) >> 16]) << 16) |
			((clp[(y13 - c22 - c32) >> 16]) << 24);
			*id1++ = DW;
			
			/* BRGB */
			DW = ((clp[(y13 + c12) >> 16])) |
			((clp[(y14 + c42) >> 16]) << 8) |
			((clp[(y14 - c22 - c32) >> 16]) << 16) |
			((clp[(y14 + c12) >> 16]) << 24);
			*id1++ = DW;
			
			/* RGBR */
			DW = ((clp[(y21 + c41) >> 16])) |
			((clp[(y21 - c21 - c31) >> 16]) << 8) |
			((clp[(y21 + c11) >> 16]) << 16) |
			((clp[(y22 + c41) >> 16]) << 24);
			*id2++ = DW;
			
			/* GBRG */
			DW = ((clp[(y22 - c21 - c31) >> 16])) |
			((clp[(y22 + c11) >> 16]) << 8) |
			((clp[(y23 + c42) >> 16]) << 16) |
			((clp[(y23 - c22 - c32) >> 16]) << 24);
			*id2++ = DW;
			
			/* BRGB */
			DW = ((clp[(y23 + c12) >> 16])) |
			((clp[(y24 + c42) >> 16]) << 8) |
			((clp[(y24 - c22 - c32) >> 16]) << 16) |
			((clp[(y24 + c12) >> 16]) << 24);
			*id2++ = DW;
		}
		id1 -= (9 * width) >> 2;
		id2 -= (9 * width) >> 2;
		py1 += width;
		py2 += width;
	}
}

#define RGB4Y 1.164
#define B4U 2.018
#define Y2ADD 16
#define G4U 0.391
#define G4V 0.813
#define U2ADD 128
#define R4V 1.596
#define V2ADD 128
#define SCALEBITS 13
#define FIX(x)((int) ((x) * (1L << SCALEBITS) + 0.5))

int g_RGB4Y_Tab[256];
int g_B4U_Tab[256];
int g_G4U_Tab[256];
int g_G4V_Tab[256];
int g_R4V_Tab[256];
void InitColorSpace()
{
	for (int i = 0; i < 256; i++)
	{
		g_RGB4Y_Tab[i]= FIX(RGB4Y) * (i - Y2ADD);
		g_B4U_Tab[i]= FIX(B4U ) * (i - U2ADD);
		g_G4U_Tab[i]= FIX(G4U ) * (i - U2ADD);
		g_G4V_Tab[i]= FIX(G4V ) * (i - V2ADD);
		g_R4V_Tab[i]= FIX(R4V ) * (i - V2ADD);
	}
}

inline unsigned char ClipColorvalue(int x)
{
	return x < 0 ? 0 : (x > 255 ? 255 : x);
}

void YUV420_RGB32_4Pixel(unsigned char * pRGB, unsigned char * pY, unsigned char* pU, unsigned char* pV, int Width)
{
	int nRGB4Y = 0;
	int nB4U = g_B4U_Tab[pU[0]];
	int nG4UV = g_G4U_Tab[pU[0]] + g_G4V_Tab[pV[0]];
	int nR4V = g_R4V_Tab[pV[0]];
	
	// (0, 0)
	nRGB4Y = g_RGB4Y_Tab[pY[0]];
	pRGB[0] = ClipColorvalue((nRGB4Y + nB4U ) >> SCALEBITS);
	pRGB[1] = ClipColorvalue((nRGB4Y - nG4UV) >> SCALEBITS);
	pRGB[2] = ClipColorvalue((nRGB4Y + nR4V ) >> SCALEBITS);
	pRGB[3] = 0;
	
	// (0, 1)
	nRGB4Y = g_RGB4Y_Tab[pY[1]];
	pRGB[4] = ClipColorvalue((nRGB4Y + nB4U ) >> SCALEBITS);
	pRGB[5] = ClipColorvalue((nRGB4Y - nG4UV) >> SCALEBITS);
	pRGB[6] = ClipColorvalue((nRGB4Y + nR4V ) >> SCALEBITS);
	pRGB[7] = 0;
	
	// (1, 0)
	nRGB4Y = g_RGB4Y_Tab[pY[Width]];
	pRGB[(Width<<2)+0] = ClipColorvalue((nRGB4Y + nB4U ) >> SCALEBITS);
	pRGB[(Width<<2)+1] = ClipColorvalue((nRGB4Y - nG4UV) >> SCALEBITS);
	pRGB[(Width<<2)+2] = ClipColorvalue((nRGB4Y + nR4V ) >> SCALEBITS);
	pRGB[(Width<<2)+3] = 0;
	
	// (1, 1)
	nRGB4Y = g_RGB4Y_Tab[pY[Width+1]];
	pRGB[(Width<<2)+4] = ClipColorvalue((nRGB4Y + nB4U ) >> SCALEBITS);
	pRGB[(Width<<2)+5] = ClipColorvalue((nRGB4Y - nG4UV) >> SCALEBITS);
	pRGB[(Width<<2)+6] = ClipColorvalue((nRGB4Y + nR4V ) >> SCALEBITS);
	pRGB[(Width<<2)+7] = 0;
}

///////////////////////////////////////////////////////////////////////////////////
//
void YUV420_RGB32(unsigned char * pRGB, unsigned char* pYUV, int Width, int Height)
{
	unsigned char * pY = pYUV, *pU = pY + Width * Height, *pV = pU + Width * Height/4;
	//unsigned char * pY = pYUV, *pV = pY + Width * Height, *pU = pV + Width * Height/4;
	short x, y;
	for (y = 0; y < Height; y += 2)
	{
		for (x = 0; x < Width; x += 2)
		{
			YUV420_RGB32_4Pixel(pRGB, pY, pU, pV, Width);
			pRGB += 8; pY += 2; pU += 1; pV += 1;
		}
		pRGB += Width<<2;
		pY += Width;
	}
}