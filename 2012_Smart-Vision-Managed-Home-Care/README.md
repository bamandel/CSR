Introduction:

You must install the OpenCV library to use this program. Directions for this are given in section 2. The rest of this document explains what the program does and the basics of what you need to know in order to use the program. 

Contents:
1. Purpose
	1.1 Author
2. OpenCV
	2.1. Visual Studio Property Pages for OpenCV
	2.2. Creating a New Solution
	2.3. List of Files and Methods
3. Calibration
4. Foreground Detection
	4.1. Challenges to Detecting Foreground
5. Human Detection
6. Activity Classification

1. Purpose:
This program was developed to assist in the monitoring of elderly persons in their home through the use of smart computer vision. The input for this program is provided by a camera, and the output is intended to be a description of the person's activity. The activity is in terms of how much time was spent standing up, walking, sitting down, lying down, etc. This system could also be used to alert care takers to a fall. Note that this program works, but is not finished in that it only differentiates between three activities (standing, sitting, lying down) and more could be added to make the activity detection more robust. Also note that 3D data, such as the depth data from the Microsoft Kinect, may also be a good source of information to use as input for an activity profiling application, however, the Kinect has limitations of its own, e.g. range and field of view.

1.1. Author:
This program (and the readme) is written by Jackson Dean Goodwin as part of a research project in the area of biomedical informatics under the direction of the National Science Foundation at the University of Tennessee at Chattanooga. The topic of the research was "Smart Vision for Managed Home-care". 

2. OpenCV:
You must install the OpenCV (open source computer vision) library in order to use this code. I used OpenCV version 2.4.1. 

2.1. Visual Studio Property Pages for OpenCV:
I developed this in Microsoft Visual Studio 2010. The Visual Studio project contains Property Pages that tell the compiler where to look for the OpenCV library code files and dll's. The property pages are in the Property_Sheets directory in the main project directory. I saved the OpenCV library in the directory C:\OpenCV\V241. If you need to save the library in another directory, you must change the directory in the property sheets by going to View | Property Manager | ForegroundDetectionTrial1 and changing the property sheets in both the Debug | Win32 and the Release | Win32 folders (the property sheets are named OpenCV241ProjectDebug and OpenCV241ProjectRelease, respectively). Next, go to Common Properties | VC++ Directories | Include Directories and change the directories to your own. And again within the VC++ Directories | Library Directories, change the directories to your own.

2.2. Creating a New Solution:
You can open the solution file (ForegroundDetectionTrial1.sln) in Visual Studio and run the program in the debugger or you can create a new solution. If you choose to make a new solution, you must add the property sheets to the appropriate folders (which you can do in Visual Studio; see section 2.1.). 

2.3. List of Files and Methods:
The main method is in ForegroundDetectionTrial1.cpp. There is one header file, cv_yuv_codebook.h, which is for the only other method in the program, cvconnectedComponents. The cvconnectedComponents method is used for cleaning up noise in the foreground and for removing regions that are too small. StdAfx.h, StdAfx.cpp are used to build a precompiled header (PCH) file named ForegroundDetectionTrial1.pch and a precompiled types file named StdAfx.obj. The last file, targetver.h was created by Visual Studio when the project was created. There are currently no arguments to the main method.

3. Calibration:
Calibration is automatic and requires that no one be in the scene. Calibration lasts about 15 seconds and starts at the launch of the program. The purpose of calibration is to learn the background. Calibration proceeds through several different exposure levels (which you may need to adjust according to your particular camera model) in order to have a model for the background at different exposures. The background must be modeled at different exposures so that the program can choose the best exposure for the scene and adjust to ambient light dynamically without having to recalibrate. The program recalibrates when either: a) There is not much in the foreground (which implies that there is no one in the room), or b) there is too much in the foreground (which implies that the lighting in the room has changed, the camera has moved, or the background model is no longer accurate). The code for the calibration is within the while loop and within the if(calibrating && !skipCalibration){...} block.

4. Foreground Detection:
The reason we have to learn the background is so that we can detect foreground (i.e. that which is not in the background). The foreground should be a silhouette of anything that has changed in the image since calibration. Ideally, this is just the outline of the person in the room, but anything else that changes its position will show up in the foreground. The foreground is shown as an image in the window when the program runs. Every part of this image is either white or black. White parts are foreground, and black parts are background. The code for the foreground detection is within the calibration block (see section 3) and within the else block corresponding to the if(calibrating){...} block.

4.1. Challenges to Detecting Foreground:
There are several challenges to detecting foreground. One is the fact that shadows will often show up as foreground, since they are a different color (i.e. darker) than the corresponding parts of the background model. Along those same lines is the problem of slowly changing lighting conditions. For example, if (during calibration) sunlight enters the room through a window and then (after calibration) a cloud partially blocks the sun, causing the light in the room to not be as bright, the image from the camera will not match the background model as well, and there will likely be foreground detected in places where there really is no foreground object. These challenges can be handled by using a HSL (hue, saturation, lightness) color space instead of a RGB (red, green, blue) color space. The idea here is that objects in a  shadow or a brighter spot are about the same "color" (hue) as they would be otherwise, but they may differ in saturation (how close they are to a color versus a shade of gray) or in lightness. Therefore, when comparing image frames from the camera to the background model, the hue should be considered as more important than the saturation or lightness. Therefore, this program uses the HSL color space.

5. Human Detection:
Now that we have a foreground, we need to determine whether there is a human in the foreground. This can be done in several ways. One is to detect percentage of skin color in each of the "blobs" i.e. the silhouettes, or regions of foreground. Another is to detect the number of straight lines in the foreground, since humans tend to have more rounded edges, and man-made objects tend to have more straight lines. And finally, human detection can be accomplished by detecting human features such as a head. This program does a little of each of these. Skin detection is done through detecting a hue of a certain color range. Straight line detection is done using OpenCV. Head detection is actually head estimation at this point, and is done by fitting an ellipse around the "human" and finding which end of the ellipse has more skin color. Much more can be done on this part of the project. The code for the human detection is in the if(detectSkin){...} block (skin detection) and in the cvconnectedComponents method in the if(i < N){...} block (straight line detection) and in the if(lines.size() < 75){...}) block (head detection).

6. Activity Classification:
So far, I only have three activities classified: Standing, sitting, and lying down. The code for the posture detection is in the same block as that for the head detection (see section 5). More work can be done to make use of the posture information, such as fall detecion, and behavior profiling.