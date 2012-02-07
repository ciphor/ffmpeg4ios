//
//  DisplayView.h
//  iPlayer
//
//  Created by Gu Update on 09-7-15.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface DisplayView : UIView {
@protected
	CGContextRef _context;
	CGImageRef _image;
	BOOL _bFull;
	BOOL _bHeng;
}

@property (nonatomic,readonly) CGContextRef context;
@property (nonatomic,readonly) CGImageRef image;
@property (nonatomic) BOOL fullScreen;
@property (nonatomic) BOOL bHeng;

- (void)setCGImage:(CGImageRef)cgimg;
- (void)saveImage;

@end
