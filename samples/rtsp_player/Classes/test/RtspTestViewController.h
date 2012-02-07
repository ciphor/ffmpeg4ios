//
//  RtspTestViewController.h
//  iPlayer
//
//  Created by HSKJ on 11-10-18.
//  Copyright 2011 _MyCompanyName. All rights reserved.
//
#ifdef __cplusplus
extern "C" {
#endif
#include "VideoDecode.h"
//#include "yuv2rgbex.h"
#ifdef __cplusplus
}
#endif	

#import <UIKit/UIKit.h>


@class DisplayView;

@interface RtspTestViewController : UIViewController<UITextFieldDelegate> {
	IBOutlet DisplayView * displayview;
	IBOutlet UITextField * urlRtsp;
	IBOutlet UIView      * containView;
	IBOutlet UIImageView * imageview;
	//L264Decode _l264D;
	BOOL iHavedIFrame;
	int old_len;
	CGContextRef _context;
	void *parser;
	void *_pDib;
	void * _pDibRgb;
	CVIDEO_DECODE * _pCodec;
	
	AVPicture picture;
	int videoStream;
	struct SwsContext *img_convert_ctx;
	AVFrame *pFrame; 
	UIImage *currentImage;
	
	NSTimer * _timer;
	BOOL  bExitThreadDisplay;
    
    int m_Width;
    int m_Height;
}

/* Last decoded picture as UIImage */
@property (nonatomic, readonly) UIImage *currentImage;
@property(nonatomic,retain) DisplayView *displayview;
@property(nonatomic,retain) UITextField *urlRtsp;
@property (nonatomic, retain)  UIImageView *imageview;

- (IBAction)onStart:(id)sender;
- (IBAction)onSnap:(id)sender;

-(void)setupScaler;

@end
