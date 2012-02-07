//
//  iPlayerAppDelegate.h
//  iPlayer
//
//  Created by Gu Update on 09-7-15.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

@interface iPlayerAppDelegate : NSObject <UIApplicationDelegate> {
    
    UIWindow *window;
    UINavigationController *navigationController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet UINavigationController *navigationController;

- (void) activateWWAN;

@end

