//
//  iPlayerAppDelegate.m
//  iPlayer
//
//  Created by Gu Update on 09-7-15.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import "iPlayerAppDelegate.h"
#import "RtspTestViewController.h"

@implementation iPlayerAppDelegate

@synthesize window;
@synthesize navigationController;

#pragma mark -
#pragma mark Application lifecycle

- (void)applicationDidFinishLaunching:(UIApplication *)application {    
    
	application.idleTimerDisabled=YES;
	[application setStatusBarHidden:YES];
		
	RtspTestViewController *rtsp = [[RtspTestViewController alloc] initWithNibName:@"RtspTestViewController" bundle:nil];
	
	navigationController = [[UINavigationController alloc] initWithRootViewController:rtsp];
	[window addSubview:[navigationController view]];
    [window makeKeyAndVisible];
	
	
}


- (void)applicationWillTerminate:(UIApplication *)application {
	// Save data if appropriate
}

//iphone4不退出
- (void)applicationWillResignActive:(UIApplication *)application {
	[[NSNotificationCenter defaultCenter] postNotificationName:@"MEApplicationDidEnterBackgroundNotification" object:nil];
}	 

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[navigationController release];
	[window release];
	[super dealloc];
}
@end

