//
//  DisplayView.m
//  iPlayer
//
//  Created by Gu Update on 09-7-15.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "DisplayView.h"
#import <mach/mach_time.h>

@implementation DisplayView
@synthesize context=_context,image=_image,fullScreen=_bFull,bHeng=_bHeng;
NSCondition * imageCondition;//用于同步_image的锁
- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        // Initialization code
		_context=nil;
		_image=nil;
		_bFull=YES;
		_bHeng=NO;
		imageCondition=[[NSCondition alloc]init];
    }
    return self;
}

- (id)initWithCoder:(NSCoder *)decoder
{
	if (self = [super initWithCoder:decoder]) {
        // Initialization code
		_context=nil;
		_image=nil;
		_bFull=YES;
		_bHeng=NO;
		imageCondition=[[NSCondition alloc]init];
    }
    return self;
}

- (void)drawRect:(CGRect)rect {
    // Drawing code
	_context=UIGraphicsGetCurrentContext();
	if (_image == NULL) {
#if 1
		UIImage *defaultScreen = [UIImage imageNamed:@"default_screen_black.png"];
#else
		UIImage *defaultScreen = [UIImage imageNamed:@"bxs_display.png"];
#endif
		_image = CGImageRetain(defaultScreen.CGImage);
	}	
	if(_image)
	{
		short iw=CGImageGetWidth(_image),ih=CGImageGetHeight(_image);
		short sw=rect.size.width,sh=rect.size.height;
		short w=iw,h=ih;
		
		if(sw==0 || sh==0 || iw==0 || ih==0)
			return;
		//*
		if(_bHeng)
		{
			if (_bFull) {
				w = sw;
				h = sh;
			} else {
				if(iw*sh>=sw*ih)
				{
					w=sw;
					h=w*ih/iw;
				}
				else
				{
					h=sh;
					w=h*iw/ih;
				}
			}

		}
		else
		{
			if(_bFull)
			{
				if(iw*sh>=sw*ih)
				{
					w=sw;
					h=sh;//w*ih/iw;
				}
				else
				{
					h=sh;
					w=sw;//h*iw/ih;
				}
			} 
		}
		/*同步_image对象*/
			[imageCondition lock];
			if (_image) 
			{
				CGContextDrawImage(_context, CGRectMake((sw-w)/2, (sh-h)/2, w, h), _image);	
			}
			[imageCondition unlock];
		
#if	0
		/*  测试用，显示帧数 */
		CGAffineTransform af = CGContextGetCTM(_context);
		static mach_timebase_info_data_t time_info;
		mach_timebase_info(&time_info);
		static uint64_t lastTimer;
		uint64_t now = mach_absolute_time();	
		int fps = 1000 / ((now - lastTimer) * time_info.numer/ time_info.denom /1000000);	
		lastTimer = now;		
		[[UIColor redColor] set];
		static int i = 0;
		NSString *s = [NSString stringWithFormat:@"F: %d FPS:%d", ++i, fps];
		CGContextConcatCTM(_context, af);
		[s drawAtPoint:CGPointMake(0, 30) withFont:[UIFont systemFontOfSize:40]];
#endif		
		
	}
}

- (void)setCGImage:(CGImageRef)cgimg
{
	@synchronized(self)
	{
		[imageCondition lock];
		if(_image)
		{
			CGImageRelease(_image);
		}
		_image=cgimg;
		[imageCondition unlock];
	}
	//[self setNeedsDisplay];
}

- (void)saveImage
{
	[imageCondition lock];
	UIImage* img=nil;
	if (_image) {
		img=[[UIImage alloc] initWithCGImage:_image];
	}
	[imageCondition unlock];
	if(img)
	{
		UIImageWriteToSavedPhotosAlbum(img , nil, nil, nil);
		[img release];
	}
}


- (void)dealloc {
	@synchronized(self)
	{
		[imageCondition lock];
		if(_image)
		{
			CGImageRelease(_image);
		}
		[imageCondition unlock];
	}
	[imageCondition release];
    [super dealloc];
}


@end
