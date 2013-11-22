//
//  ViewController.h
//  Testing5
//
//  Created by Lion User on 6/22/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CoreMotion/CoreMotion.h>
#import <AudioToolbox/AudioToolbox.h>

@interface ViewController : UIViewController {
    CMMotionManager *motionManager;
    UILabel *accelerometerLabel;
    UILabel *gyroscopeLabel;
    UILabel *Velocity;
    NSTimer *updateTimer;
    int counter;
    CMAttitude *referenceAttitude;
    UIButton *Collection;
    NSMutableArray *zValues;
    NSMutableArray *Time;
    SystemSoundID ready;

}

@property(nonatomic, retain) CMMotionManager *motionManager;
@property (nonatomic, retain) IBOutlet UILabel *accelerometerLabel;
@property (nonatomic, retain) IBOutlet UILabel *gyroscopeLabel;
@property (nonatomic, retain) NSTimer *updateTimer;
@property (nonatomic) int counter;
@property (strong, nonatomic) IBOutlet UIButton *Collection;
@property (nonatomic, retain) CMAttitude *referenceAttitude;
@property (strong, nonatomic) IBOutlet UILabel *Velocity;
- (IBAction)startCollection:(UIButton *)sender;
- (IBAction)stopCollection:(UIButton *)sender;
- (IBAction)Integration:(id)sender;
@property(nonatomic, retain) NSMutableArray *zValues;
@property(nonatomic, retain) NSMutableArray *Time;
@property(nonatomic) SystemSoundID ready;
@end
