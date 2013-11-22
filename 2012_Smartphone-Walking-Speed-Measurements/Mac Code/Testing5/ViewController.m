//
//  ViewController.m
//  Testing5
//
//  Created by Lion User on 6/22/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "ViewController.h"
#import <QuartzCore/CAAnimation.h>
#import <AVFoundation/AVFoundation.h>
#import <AudioToolbox/AudioToolbox.h>
@interface ViewController ()

@end

@implementation ViewController
@synthesize ready;
@synthesize  motionManager;
@synthesize accelerometerLabel; //used to display 
@synthesize gyroscopeLabel;
@synthesize updateTimer;
@synthesize counter;
@synthesize Collection;
@synthesize referenceAttitude;
@synthesize Velocity;
@synthesize zValues;
@synthesize Time;
#define kFilteringFactor 0.1

bool record=false;
bool walkstarted=false;
float referenceroll=0;
float referencepitch=0;
float referenceyaw=0;
double lastmax=0;
int timelastmax=0;
double backtwo=0;
double backone=0;
double backonez=0;
double backtwoz=0;
double zVel=0;
double totaltime=0;
double totalvel=0;

UIAccelerationValue rollingX, rollingY, rollingZ, x, y, z;
- (void)viewDidLoad
{
    counter=0;
    [super viewDidLoad];
    zValues=[NSMutableArray array];
    Time=[NSMutableArray array];
	// Do any additional setup after loading the view, typically from a nib.
    [self redirectConsoleLogToDocumentFolder];
                   }
#define kAccelerometerMinStep				0.02
#define kAccelerometerNoiseAttenuation		3.0

double Norm(double x, double y, double z)
{
	return sqrt(x * x + y * y + z * z);
}

double Clamp(double v, double min, double max)
{
	if(v > max)
		return max;
	else if(v < min)
		return min;
	else
		return v;
}

-(void) updateSensorData {
    if (motionManager.accelerometerAvailable && motionManager.gyroAvailable){
        
         CMDeviceMotion *currentDeviceMotion=motionManager.deviceMotion;
        
        CMAccelerometerData *accelerometerData=motionManager.accelerometerData;
        rollingX=currentDeviceMotion.gravity.x; //gravity function serves as a high-pass filter
        rollingY=currentDeviceMotion.gravity.y;
        rollingZ=currentDeviceMotion.gravity.z;
        float accelX=accelerometerData.acceleration.x-rollingX;
        float accelY=accelerometerData.acceleration.y-rollingY;
        float accelZ=accelerometerData.acceleration.z-rollingZ;
        
        float magnitude=(rollingX*rollingX+rollingY*rollingY+rollingZ*rollingZ);
        float vert=(accelX*rollingX+accelY*rollingY+accelZ*rollingZ)/magnitude;
        float vertm=(vert*rollingX+vert*rollingY+vert*rollingZ);
        
        
        accelX=accelX-vert*rollingX; //removes vertical component of accelerations, leaving only horizontal
        accelY=accelY-vert*rollingY;
        accelZ=accelZ-vert*rollingZ;
               accelerometerLabel.text= [NSString stringWithFormat:@"Accelerometer\n----------\nx: %+.2f\ny: %.2f\nz:%+.2f", accelX, accelY, accelZ];
        
        counter++;
        if (record){
        if (counter==500){
         referenceAttitude=currentDeviceMotion.attitude; //this provides a reference in case angles are wanted from time of start
             referenceroll=referenceAttitude.roll;
             referencepitch=referenceAttitude.pitch;
             referenceyaw=referenceAttitude.yaw;
         }
        if(counter>500){
                  
        CMAttitude *currentAttitude=currentDeviceMotion.attitude; //following is the corrections to the angles, based on the reference angle
        float roll=currentAttitude.roll-referenceroll;
        roll=roll*180/3.14159265359; //converts angle from radians to degrees
        float pitch=currentAttitude.pitch;
        pitch=pitch*180/3.14159265359;
        float yaw=currentAttitude.yaw-referenceyaw;
        yaw=yaw*180/3.14159265359;
           
            double CurrentTime=CACurrentMediaTime(); //following code is intended for real-time integration, but is not currently accurate
            double timedif=CurrentTime-backtwo;
            NSNumber *aZ= [NSNumber numberWithFloat:accelZ]; 
            [zValues addObject:aZ];
            NSNumber *time= [NSNumber numberWithFloat:(CurrentTime-backone)]; 
            [Time addObject:time];
            if(counter%2==0&&counter>500){
                zVel+=(9.8*(timedif)*((backtwoz)+4*(backonez)+(accelZ)))/6.00;
                if (walkstarted){
                totaltime+=timedif;
                totalvel+=(zVel*timedif);
                }else {
                    if (accelZ>0.1) walkstarted=true;
                }
            }
            zVel*=0.985;
            if (accelZ<0.1&&accelZ>-0.1&&backonez<0.1&&backonez>-0.1&&backtwoz<0.1&&backtwoz>-0.1){
                zVel*=0.95;
            }
            backone=CurrentTime;
            backtwo=backone;
            backonez=accelZ;
            backtwoz=backonez; //end of non-functional integration code
            
            
        gyroscopeLabel.text=[NSString stringWithFormat: @"Gyroscope\n------\nroll: %+0.2f\npitch: %.2f\nyaw: %+.2f",
                             roll, pitch, yaw];
            NSLog(@"X: %f Y: %f Z: %f V %f R: %f P: %f  Ya: %f Steps: %i", accelX, accelY, accelZ, vertm, roll, pitch, yaw, 0);
        }
        }
    }

}
-(void) redirectConsoleLogToDocumentFolder
{
    NSArray *paths=NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory =[paths objectAtIndex:0];
    NSString *logPath =[documentsDirectory stringByAppendingPathComponent:@"console.log"];
    freopen([logPath cStringUsingEncoding:NSASCIIStringEncoding], "a+",stderr);
}
- (void)viewDidUnload
{
    self.motionManager=nil;
    [self setAccelerometerLabel:nil];
    [self setGyroscopeLabel:nil];
    [self setCollection:nil];
    [self setVelocity:nil];
    [super viewDidUnload];
    // Release any retained subviews of the main view.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
}

- (IBAction)startCollection:(UIButton *)sender {
    self.motionManager=[[CMMotionManager alloc] init];
    
    if (motionManager.accelerometerAvailable){
        motionManager.accelerometerUpdateInterval=1.0/60.0;
        [motionManager startAccelerometerUpdates];
    }else {
        accelerometerLabel.text=@"This device has no accelerometer.";
    }
    if(motionManager.isDeviceMotionAvailable){
        motionManager.deviceMotionUpdateInterval=1.0/60.0;
        [motionManager startDeviceMotionUpdates];
    } 
    if (motionManager.gyroAvailable){
        motionManager.gyroUpdateInterval=1.0/60.0;
        [motionManager startGyroUpdates];
    }else {
        gyroscopeLabel.text=@"This device has no gyroscope.";
    }
    counter=100;
    record=true;
    self.updateTimer=[NSTimer scheduledTimerWithTimeInterval:1.0/60.0 target:self selector:@selector(updateSensorData) userInfo:nil repeats:YES];

}

- (IBAction)stopCollection:(UIButton *)sender {
   
    [motionManager stopGyroUpdates];
    [motionManager stopAccelerometerUpdates];
    [motionManager stopDeviceMotionUpdates];
    counter=0;
    zVel=0;
    totaltime=0;
    totalvel=0;
    backtwo=0;
    backone=0;
    backonez=0;
    backtwoz=0;
    record=false;
    walkstarted=false;
     NSLog(@" New ");
}

- (IBAction)Integration:(id)sender { //intended to be a function that does integration similar to that currently being done in Java
    
}
/*-(void)playsound{
    AudioServicesPlaySystemSound(ready);
    
}*/
@end
