//
//  RtspTestViewController.m
//  iPlayer
//
//  Created by HSKJ on 11-10-18.
//  Copyright 2011 _MyCompanyName. All rights reserved.
//

#import "RtspTestViewController.h"
#import "DisplayView.h"
#include "FrameList.h"
#include "RtspParser.h"
#include "StdAfx.h"
#include "rtsp.h"
#include "us_parser_raw264.h"
#include "videoDecode.h"
#import <QuartzCore/QuartzCore.h>
#import <mach/mach_time.h>



@implementation RtspTestViewController
@synthesize displayview;
@synthesize urlRtsp;
@synthesize imageview;

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/
-(void)setupScaler {
	
	// Release old picture and scaler
	avpicture_free(&picture);
	sws_freeContext(img_convert_ctx);	
	
	// Allocate RGB picture
	avpicture_alloc(&picture, PIX_FMT_RGB24, m_Width, m_Height);
	avcodec_init();		// Setup scaler
	static int sws_flags =  SWS_FAST_BILINEAR;
	img_convert_ctx = sws_getContext(_pCodec->m_pAVCodecContext->width, 
									 _pCodec->m_pAVCodecContext->height,
									 _pCodec->m_pAVCodecContext->pix_fmt,
									 m_Width, 
									 m_Height,
									 PIX_FMT_RGB24,
									 sws_flags, NULL, NULL, NULL);
	
}


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	//L264Decode_Init(&_l264D);
	iHavedIFrame = NO;
	old_len = 0;
	_pDib = malloc(4*720*576);
	memset(_pDib, 0, 4*720*576);
	
	//_pDibRgb = malloc(4*720*576);
	//memset(_pDibRgb, 0, 4*720*576);
	//_pDibRgb = NULL;
	
	CATransform3D landscapeTransform = CATransform3DIdentity;
	landscapeTransform = CATransform3DRotate(landscapeTransform, M_PI, 1, 0, 0);
	
	//self.displayview.layer.transform = landscapeTransform;
    urlRtsp.delegate = self;
	//self.imageview.layer.transform = landscapeTransform;
	
	
	//_Codec.InitDecode(352, 288, CODEC_ID_H264); 
	//InitRgbYuv();
	//Yuv2Rgb = yuv2rgb_init(32, 0);
	//InitConvertTable();
    
    m_Width = 0;
    m_Height = 0;
	
	
	bExitThreadDisplay = FALSE;
}

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
	//L264Decode_End(&_l264D);
	
	//UnInitDecode(_pCodec);
	//[_timer invalidate];
	bExitThreadDisplay = TRUE;
}


- (void)dealloc {
    [super dealloc];
}

- (IBAction)onStart:(id)sender {
    static bool isPlay = NO;
    if (!isPlay ) {
        NSLog(@"====================start");
        isPlay = YES;
        if (parser == NULL) {
			RtspParser *parser1 = new RtspParser();
			//Raw264Parser *parser1 = new Raw264Parser();
            parser = parser1;
            
        }
		
		if ([self.urlRtsp.text length]) {
			[self.urlRtsp resignFirstResponder];
			
			((RtspParser*)parser)->create([self.urlRtsp.text cStringUsingEncoding:NSUTF8StringEncoding], SST_RTSP_GENERAL);
			self.urlRtsp.text = @"START";
			
			
			((RtspParser*)parser)->play();
			[NSThread detachNewThreadSelector:@selector(getFrames) toTarget:self withObject:nil];
			
			//START TIMER			
			_timer = [NSTimer scheduledTimerWithTimeInterval:1.0/30
													  target:self
													selector:@selector(displayNextFrame)
													userInfo:nil
													 repeats:YES];
		}else {
			isPlay = NO;
			((RtspParser*)parser)->stop();   
			[_timer invalidate];
		}
	}
}

- (IBAction)onSnap:(id)sender {
	if (displayview) {
		[displayview saveImage];
	}
}




- (void)getFrames {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	uint64_t ts = 0;
    TAVPMediaInfo mediaInfo;
    
	while(TRUE) {
        if(((RtspParser*)parser)->GetMediaInfo(&mediaInfo))
        {
            m_Width = mediaInfo.videoInfo.wWidth;
            m_Height = mediaInfo.videoInfo.wHeight;
            
            _pCodec = (CVIDEO_DECODE*) malloc(sizeof(CVIDEO_DECODE));
            memset(_pCodec, 0, sizeof(CVIDEO_DECODE));
            InitDecode(_pCodec, m_Width, m_Height, CODEC_ID_H264, PIX_FMT_YUV420P);  //PIX_FMT_ARGB, PIX_FMT_YUV420P,FMT_RGBA32
        
            //InitColorSpace();
            [self setupScaler];
            
            break;
            
        }
        else
        {
            
            [NSThread sleepForTimeInterval:0.03];
            /*
            m_Width = 352;
            m_Height = 288;
            
            _pCodec = (CVIDEO_DECODE*) malloc(sizeof(CVIDEO_DECODE));
            memset(_pCodec, 0, sizeof(CVIDEO_DECODE));
            InitDecode(_pCodec, m_Width, m_Height, CODEC_ID_H264, PIX_FMT_YUV420P);  //PIX_FMT_ARGB, PIX_FMT_YUV420P,FMT_RGBA32
            
            //InitColorSpace();
            [self setupScaler];
                
            break;
            */
        }
    }
	while (TRUE) {
       int statu = ((RtspParser*)parser)->getstatu();
		NSLog(@"rtsp statu %d",statu);	
		
		MediaData_FRAME * frame = ((RtspParser*)parser)->GetNextFrame();
		if (frame == NULL) {
			[NSThread sleepForTimeInterval:0.03];
			
            NSLog(@"==============================================");
		}else 
		{
			NSLog(@"====================frame %d",frame->nFrameLength);
			
		   
			[self.urlRtsp performSelectorOnMainThread:@selector(setText:) withObject:[NSString stringWithFormat:@"len %6d,  isIFrame: %@, ts:%4d",frame->nFrameLength,frame->nParam1?@"TRUE":@"FALSE", ts] waitUntilDone:NO];
		
			if (frame->nParam1 == 1) {
				iHavedIFrame = YES;
			}
			if (!iHavedIFrame) {
				continue;
			}
//#define READ_PACKET_H264
#ifdef READ_PACKET_H264	
			unsigned char picpDataNode[32768];
			int len_264 = 0;
			if(1)
			{
				int static fileNo = 0;
				char filename[20];
				sprintf(filename, "%04d.264", fileNo);
				fileNo++;
				//FILE * file;
				//file = fopen(filename, "rb+");
				NSString *fname = [NSString stringWithFormat:@"%s",filename];
				NSString *docDir = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject];
				NSString *fullPath = [docDir stringByAppendingPathComponent:fname];
				FILE *file = fopen([fullPath cStringUsingEncoding:NSUTF8StringEncoding], "rb+");
				
				printf("read file %s", filename);
				if(file) {
					len_264 = fread(picpDataNode, 1, 32768, file);
					printf("++++++++++++++++++++read %d\n", len_264);
					fclose(file);	
				}
			}

			//解码
			int olen=0;
			int r = 0;
			int len_align = ((len_264 + 7) / 8 ) * 8;
			char *frameData = (char*) malloc(len_align);
			
			if(len_264)
			{

				memset(frameData, 0, len_align);
				memcpy(frameData, picpDataNode, len_264);	
			}
			else{
				return;
			}

#else
			//解码
			int olen=0;
			int r = 0;
			int frameData_len = frame->nFrameLength;
			int len_align = ((frameData_len + 7) / 8 ) * 8;
			char *frameData = (char*) malloc(len_align);
			memset(frameData, 0, len_align);
			memcpy(frameData, frame->pContent, frame->nFrameLength);	
#endif			

			
//#define SAVE_PACKET_H264
#ifdef SAVE_PACKET_H264		
			int static fileNo = 0;
			char filename[20];
			sprintf(filename, "%04d.264", fileNo);
			fileNo++;
			//FILE * file;
			//file = fopen(filename, "wb+");
			NSString *fname = [NSString stringWithFormat:@"%s",filename];
			NSString *docDir = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject];
			NSString *fullPath = [docDir stringByAppendingPathComponent:fname];
			FILE *file = fopen([fullPath cStringUsingEncoding:NSUTF8StringEncoding], "wb+");
			
			NSLog(@"file:%@", fullPath);
			
			printf("save file %s", filename);
			if(file) {
				int l = fwrite(frameData, 1, frame->nFrameLength, file);
				printf("++++++++++++++++++++save %d\n", l);
				fclose(file);	
			}
#endif	
			
#define DECODE_TIME
#if	defined (DECODE_TIME)
			/*  测试用，显示帧数 */
			static mach_timebase_info_data_t time_info;
			mach_timebase_info(&time_info);
			static uint64_t lastTimer;
			uint64_t now = mach_absolute_time();	
			
			
			//ts = ((now - lastTimer) * time_info.numer/ time_info.denom /1000000);
			//NSLog(@"time stamp: %d", ts);
			lastTimer = now;	
			
#endif				
			int keyFrame = 0;
			NSLog(@"head:%x,%x,%x,%x,%x", (unsigned char) frameData[0], (unsigned char) frameData[1], (unsigned char) frameData[2], (unsigned char) frameData[3], (unsigned char) frameData[4]);
			//r = L264Decode_DecodeFrame(&_l264D, (const unsigned char*)frameData, frame->nFrameLength, (unsigned char*)_pDib, &olen, FMT_RGBA32, 0);
			r = AVDecode(_pCodec, (unsigned char*)frameData, frameData_len, (unsigned char**)&_pDib, &keyFrame);

#if	defined (DECODE_TIME)
			now = mach_absolute_time();
			ts = ((now - lastTimer) * time_info.numer/ time_info.denom /1000000);
			NSLog(@"DECODE time stamp: %d", ts);
			lastTimer = now;
#endif
			
//#define DUMP_YUV
#if defined(DUMP_YUV)
			if(keyFrame)
			{
				int static fileNo = 0;
				char filename[20];
				sprintf(filename, "%04d.yuv", fileNo);
				fileNo++;
				//FILE * file;
				//file = fopen(filename, "rb+");
				NSString *fname = [NSString stringWithFormat:@"%s",filename];
				NSString *docDir = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject];
				NSString *fullPath = [docDir stringByAppendingPathComponent:fname];
				FILE *file = fopen([fullPath cStringUsingEncoding:NSUTF8StringEncoding], "wb+");
				NSLog(@"file:%@", fullPath);

				if(file) {
					int l = fwrite(_pDib, 1, 352*288*3/2, file);
					printf("++++++++++++++++++++save %d\n", l);
					fclose(file);	
				}	
				
			}
#endif
			
			free(frameData);
			
			if(r == 0)
			{
                NSLog(@"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
				//L264Decode_End(&_l264D);
				//L264Decode_Init(&_l264D);
				///UnInitDecode(_pCodec);
				//InitDecode(_pCodec, 352, 288, CODEC_ID_H264, PIX_FMT_YUV420P);
				iHavedIFrame = NO;
			}
			else
			{	
				NSLog(@"oooooooooooooooooooooooooooooooooooooooooo");
				//if (old_len != olen) {
				{
					old_len = olen;
					//CGContextRef context = NULL;
					//CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
					
//					Yuv12ToRgb32(352,288, (unsigned char *)_pDib, (unsigned char*) _pDibRgb);
//					Yuv2Rgb((unsigned char*) _pDibRgb, (unsigned char*)_pDib, (unsigned char*)_pDib + 352*288 ,
//							(unsigned char*)_pDib + 352*288*5/4,
//							352,288,352, 288,352*288*3/2);
//					ConvertYUVtoRGB((unsigned char*)_pDib, (unsigned char*)_pDib + 352*288 ,
//									(unsigned char*)_pDib + 352*288*5/4,
//									(unsigned char*) _pDibRgb, 
//									352,288);
//					YUV420_RGB32((unsigned char*)_pDibRgb, (unsigned char*)_pDib, 352, 288);
					
					sws_scale (img_convert_ctx, _pCodec->m_pPicture->data, _pCodec->m_pPicture->linesize,
							   0, _pCodec->m_pAVCodecContext->height,
							   picture.data, picture.linesize);	
					
#if	defined (DECODE_TIME)
					now = mach_absolute_time();
					ts = ((now - lastTimer) * time_info.numer/ time_info.denom /1000000);
					NSLog(@"YUV2RGB time stamp: %d", ts);
					lastTimer = now;
#endif
					
//#define DUMP_RGB
#if defined(DUMP_RGB)
					if(keyFrame)
					{
						int static fileNo = 0;
						char filename[20];
						sprintf(filename, "%04d.rgb", fileNo);
						fileNo++;
						//FILE * file;
						//file = fopen(filename, "rb+");
						NSString *fname = [NSString stringWithFormat:@"%s",filename];
						NSString *docDir = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject];
						NSString *fullPath = [docDir stringByAppendingPathComponent:fname];
						FILE *file = fopen([fullPath cStringUsingEncoding:NSUTF8StringEncoding], "wb+");
						NSLog(@"file:%@", fullPath);
						
						if(file) {
							int l = fwrite(_pDib, 1, 352*288*4, file);
							printf("++++++++++++++++++++save %d\n", l);
							fclose(file);	
						}	
						
					}
#endif				
					//no out
					//context = CGBitmapContextCreate(_pDibRgb, 352,288, 8, 4* 288, colorSpace, kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Little);
					
					//blue color
					//context = CGBitmapContextCreate(_pDibRgb, 352,288, 8, 4* 352, colorSpace, kCGImageAlphaNoneSkipLast| kCGBitmapByteOrder32Little);
					
					//
					//context = CGBitmapContextCreate(_pDib, 352,288, 8, 4* 352, colorSpace, kCGImageAlphaNoneSkipLast | kCGBitmapByteOrder32Little);
					//CGColorSpaceRelease(colorSpace);
					//CGContextRelease(_context);
					//_context = context;
					

					
					CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
					CFDataRef data = CFDataCreateWithBytesNoCopy(kCFAllocatorDefault, picture.data[0], picture.linesize[0] * 288, kCFAllocatorNull);
					CGDataProviderRef provider = CGDataProviderCreateWithCFData(data);
					CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
					CGImageRef cgImage = CGImageCreate(m_Width, 
													   m_Height, 
													   8, 
													   24, 
													   picture.linesize[0], 
													   colorSpace, 
													   bitmapInfo, 
													   provider, 
													   NULL, 
													   NO, 
													   kCGRenderingIntentDefault);
					//[self.displayview setCGImage:cgImage];
					//[self.displayview performSelectorOnMainThread:@selector(setNeedsDisplay) withObject:nil waitUntilDone:YES];

					CGColorSpaceRelease(colorSpace);
					if(currentImage != nil)
					{
						[currentImage release];
						currentImage = nil;
					}
					currentImage = [UIImage imageWithCGImage:cgImage];
					CGImageRelease(cgImage);
					CGDataProviderRelease(provider);
					CFRelease(data);
					
					//self.imageview.image = nil;
					
					//self.imageview.image = image;
					//[self.imageview setImage:[UIImage imageNamed:@"icon.PNG"]];
					//[self.imageview performSelectorOnMainThread:@selector(setNeedsDisplay) withObject:nil waitUntilDone:NO];
					
					//CGRect rect = CGRectMake(0.0f, 0.0f, 320.f,109.0f);
					//UIImageView * myImage = [[UIImageView alloc] initWithFrame:rect];
					//[myImage setImage:image];
					//myImage.opaque = YES;
					//[self.view addSubview:myImage];
					//[myImage release];
					
					
				}
				
				//[self.displayview setCGImage:CGBitmapContextCreateImage(_context)];
				//[self.displayview setCGImage:cgImage];
				//[self.displayview performSelectorOnMainThread:@selector(setNeedsDisplay) withObject:nil waitUntilDone:YES];
			}

#if	defined (DECODE_TIME)
			now = mach_absolute_time();
			ts = ((now - lastTimer) * time_info.numer/ time_info.denom /1000000);
			NSLog(@"DISPLAY time stamp: %d", ts);
			lastTimer = now;
#endif
		}
		
		//[NSThread sleepForTimeInterval:0.03];
		
	}
    [pool release];
}

-(void)displayNextFrame {
	//[timer invalidate];
	//[self.imageview setImage:[UIImage imageNamed:@"icon.PNG"]];
	//NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	//while (! bExitThreadDisplay) 
	{
		
		if(currentImage != nil)
		{
			imageview.image = currentImage;
			NSLog(@"image%f %f\n", currentImage.size.height, currentImage.size.width); 
		}
		
		//[NSThread sleepForTimeInterval:0.03];
	}
	
	//[pool release];

}


- (void)textFieldDidBeginEditing:(UITextField *)textField {
	containView.frame = CGRectMake(0, 0, 200, 133);
}

- (void)textFieldDidEndEditing:(UITextField *)textField {
	containView.frame = CGRectMake(0, 283, 200, 133);
    [textField resignFirstResponder];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    [textField resignFirstResponder];
    return YES;
}

@end
