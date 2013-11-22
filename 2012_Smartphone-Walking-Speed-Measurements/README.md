Contents:

Project Description

System Requirements

Xcode Section

Collecting Data

Java Section

Unfinished Features/Future Development Possibilities

Acknowledgments


I. Project Description

The goal of this project was to develop a workable algorithm to measure walking speed using the accelerometer of an iPod Touch. Currently, this is accomplished through an app that collects data on the phone, which then uploads to the computer, where it is then analyzed.

II.  System Requirements

Must have a computer running Mac OS X 10.7 or later. 

Xcode version 5 or later

Java SDK 6 or later

III. Xcode Section

This section of the project was written in Objective-C, using the QuartzCore, CoreMotion, and UIKit frameworks in Xcode.
To use this code, you must first set up an apple device, either an iPod Touch or iPhone to work with Xcode. Email address and password for doing this available on request from yu-cao@utc.edu
A useful tutorial on how to do this can be found here: http://udn.epicgames.com/Three/AppleiOSProvisioningSetup.html 
Once the device is set up with Xcode, you can compile and run the project from within Xcode and it will load it on the device.
Once loaded onto the device, it can then be used to collect data (see section IV).
Once the desired data has been collected, it can then be uploaded to the computer in the following fashion:

First, connect the device to the computer.

Second, select the device in the Xcode Organizer.

Select “Applications” from the list that appears under the device.

Select the application, current version is “Testing5”

Click the “Download” button at the buttom of the page, and type in what you would like to call that dataset.

Open up .xcappdata file (by right-clicking and selecting “show package contents”), and then navigate to AppData\Documents

Copy the console.log file wherever you plan to use it.

The console.log file contains all the data collected from the phone.  
Important note: this does not delete data from phone. You will have to manually delete the app (under the Xcode Organizer) and reload it onto the phone to collect more data.

IV. Collecting Data

To collect data, follow this procedure:

When ready, open the app and hit the “Start Collection” button.

Once button is pressed, you have five second to locate the phone (it will need to have a belt-clip) onto your belt at the base of your spine.

Wait a few seconds to begin walking.

When finished, either hit the stop collection button or simply exit the app. Important: the app does not have a way to detect when walking has stopped, so either stop collection before removing from body or bear in mind that the final integration may be flawed.

V.  Java Section

Currently, all code is contained in one class.

This program requires 9 runtime arguments.

The first argument should be a console.log file containing the data.

The other 8 arguments are output files that the program writes to. These are primarily for development and testing purposes, as they contain useful information about the data.

Once compiled and run, the results are displayed on the Console.

VI.  Unfinished Features/Future Development Possibilities

Noise notification for when app has begun collecting data

Porting the Java code into Objective-C so that the integration can be done on the device.

Updating Java code so that it recognizes “New” command from Xcode data and treats each additional “New” command as a separate data set.

Changing algorithm so that it examines only five second intervals and thus can be done in almost real-time on the device.

VII. Acknowledgments

Developed by Jeffrey Cox. Special thanks to Dr. Yu Cao for mentorship and training. 

Facilities provided by University of Tennessee at Chattanooga.

Funding provided by the National Science Foundation.

