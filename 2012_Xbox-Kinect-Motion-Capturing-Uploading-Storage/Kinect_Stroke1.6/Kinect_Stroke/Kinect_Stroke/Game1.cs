using System;
using System.Collections.Generic;
using System.Linq;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Media;
using Microsoft.Kinect;
using System.IO;
using Coding4Fun.Kinect.Wpf;
using AForge.Video.VFW;
using AForge.Video.FFMPEG;
using AForge.Video;
using SkeletalTracking.Utils.Export;
/*This is the last known save of Kinect_Stroke
 

 */

namespace Kinect_Stroke
{
    /// <summary>
    /// This is the main type for your game
    /// </summary>
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        public Random generator;
        float elevation;
        int elevation2, skelID, countRecord = 0;
        double legAngle,  timeDifference;
        int count = 0, angle = 0, angle2; 
        bool isfirst = true, flagSecondCount = false; 
        string connectedStatus = "Not connected", startFlag, text; 
        byte[] pixelsFromFrame;
        Calculations calculate = new Calculations();
        TimeSpan timeSpan, timeSpan2;
        DateTime startTime, startTime2;
        DateTime endTime, endTime2; 
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        MouseState firstState, secondState;
        KeyboardState oldState, previousState, currentState;
        Boolean isPaused = false;
        Texture2D kinectRGBVideo, overlay, mainMenu, mousetexture, exerciseMenu, saveMenu, areYouSureMenu, savingPromptScreen,
                  kinectCalibrationScreen, rightArmHover, rightLegHover, leftArmHover, leftLegHover, yesHover, noHover, Kinect_Menu_Hover, continueHover,
                  debugColor, enterPatientName, excellent, particleTex;
        Skeleton[] skeletonData;
        Rectangle textBox;
        Vector2 mousePosition; 
        Vector3 point1, point2;
        KinectSensor kinectSensor;
        SpriteFont font; 
        SoundEffect applause, buttonClick, gamePlayMeditation;
        SoundEffectInstance gamePlayMeditationInstance;
        public List<string> patientNameList;
       
        ColorImageFrame colorImageFrame;
        RecordSkeleton recordSkeleton = new RecordSkeleton();
        Joint rightHand, leftHand, head, rightWrist, leftWrist, rightElbow, leftElbow, shoulderCenter, rightShoulder, leftShoulder, spine,
              hipCenter, rightHip, leftHip, rightKnee, leftKnee, rightAnkle, leftAnkle, rightFoot, leftFoot;

        string[] skeletalJointFileNames = new string[] {"handRight.txt", "handLeft.txt", "head.txt", "wristRight.txt", "wristLeft.txt","elbowRight.txt", 
            "elbowLeft.txt", "shoulderCenter.txt", "shoulderRight.txt","shoulderLeft.txt", "spine.txt", "hipCenter.txt", "hipRight.txt", "hipLeft.txt", 
            "kneeRight.txt", "kneeLeft.txt", "ankleRight.txt", "ankleLeft.txt", "footRight.txt", "footLeft.txt", "angle.txt", "legAngle.txt"};

        StreamWriter writerRightHand = new StreamWriter("handRight.txt"), writerLeftHand = new StreamWriter("handLeft.txt"),
            writerHead = new StreamWriter("head.txt"), writerRightWrist = new StreamWriter("wristRight.txt"),
            writerLeftWrist = new StreamWriter("wristLeft.txt"), writerRightElbow = new StreamWriter("elbowRight.txt"),
            writerLeftElbow = new StreamWriter("elbowLeft.txt"), writerShoulderCenter = new StreamWriter("shoulderCenter.txt"),
            writerRightShoulder = new StreamWriter("shoulderRight.txt"), writerLeftShoulder = new StreamWriter("shoulderLeft.txt"),
            writerSpine = new StreamWriter("spine.txt"), writerHipCenter = new StreamWriter("hipCenter.txt"),
            writerRightHip = new StreamWriter("hipRight.txt"), writerLeftHip = new StreamWriter("hipLeft.txt"),
            writerRightKnee = new StreamWriter("kneeRight.txt"), writerLeftKnee = new StreamWriter("kneeLeft.txt"),
            writerRightAnkle = new StreamWriter("ankleRight.txt"), writerLeftAnkle = new StreamWriter("ankleLeft.txt"),
            writerRightFoot = new StreamWriter("footRight.txt"), writerLeftFoot = new StreamWriter("footLeft.txt"),
            writerAngle = new StreamWriter("angle.txt"), writerElevation = new StreamWriter("elevation.txt"),
        writerLegAngle = new StreamWriter("legAngle.txt");

        enum gameState
        {
            titleScreen,
            mainGame,
            excerciseChoiceScreen,
            saveScreen,
            areYouSureScreen,
            savingPromptScreen,
            kinectCalibrationScreen,
            enterPatientNameScreen
        }
        gameState state = gameState.titleScreen;

        enum exerciseState
        {
            none,
            leftArm,
            leftLeg,
            rightArm,
            rightLeg
        }
        exerciseState exState = exerciseState.none;

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
            graphics.PreferredBackBufferWidth = 640;
            graphics.PreferredBackBufferHeight = 480;
        }

        void KinectSensors_StatusChanged(object sender, StatusChangedEventArgs e)
        {
            if (this.kinectSensor == e.Sensor)
            {
                if (e.Status == KinectStatus.Disconnected ||
                    e.Status == KinectStatus.NotPowered)
                {
                    this.kinectSensor = null;
                    this.DiscoverKinectSensor();
                }
            }
        }

        private bool InitializeKinect()
        {

            kinectSensor.ColorStream.Enable(ColorImageFormat.RgbResolution640x480Fps30);
            kinectSensor.ColorFrameReady += new EventHandler<ColorImageFrameReadyEventArgs>(kinectSensor_ColorFrameReady);
            kinectSensor.SkeletonStream.Enable(new TransformSmoothParameters()
        {
            Smoothing = 0.5f,
            Correction = 0.5f,
            Prediction = 0.5f,
            JitterRadius = 0.05f,
            MaxDeviationRadius = 0.04f
        }); 
            kinectSensor.SkeletonFrameReady += new EventHandler<SkeletonFrameReadyEventArgs>(kinectSensor_SkeletonFrameReady);
            try
            {
                kinectSensor.Start();
                kinectSensor.ElevationAngle = 0;
            }
            catch
            {
                connectedStatus = "Unable to start the Kinect Sensor";
                return false;
            }
            return true;
        }

        void kinectSensor_SkeletonFrameReady(object sender, SkeletonFrameReadyEventArgs e)
        {
            using (SkeletonFrame skeletonFrame = e.OpenSkeletonFrame())
            {
                if (skeletonFrame != null)
                {
                    skeletonData = new Skeleton[skeletonFrame.SkeletonArrayLength];
                    skeletonFrame.CopySkeletonDataTo(skeletonData);
                    Skeleton playerSkeleton = (from s in skeletonData where s.TrackingState == SkeletonTrackingState.Tracked select s).FirstOrDefault();

                    if (playerSkeleton != null)
                    {
                        // flag for the first skeleton in the frame so it tracks only that one
                        skelID = playerSkeleton.TrackingId;
                        kinectSensor.SkeletonStream.AppChoosesSkeletons = true;
                        kinectSensor.SkeletonStream.ChooseSkeletons(skelID);

                        //assign a jointtype to a declared joint
                        head = playerSkeleton.Joints[JointType.Head];
                        rightHand = playerSkeleton.Joints[JointType.HandRight];
                        leftHand = playerSkeleton.Joints[JointType.HandLeft];
                        rightWrist = playerSkeleton.Joints[JointType.WristRight];
                        leftWrist = playerSkeleton.Joints[JointType.WristLeft];
                        rightElbow = playerSkeleton.Joints[JointType.ElbowRight];
                        leftElbow = playerSkeleton.Joints[JointType.ElbowLeft];
                        shoulderCenter = playerSkeleton.Joints[JointType.ShoulderCenter];
                        rightShoulder = playerSkeleton.Joints[JointType.ShoulderRight];
                        leftShoulder = playerSkeleton.Joints[JointType.ShoulderLeft];
                        spine = playerSkeleton.Joints[JointType.Spine];
                        hipCenter = playerSkeleton.Joints[JointType.HipCenter];
                        rightHip = playerSkeleton.Joints[JointType.HipRight];
                        leftHip = playerSkeleton.Joints[JointType.HipLeft];
                        rightKnee = playerSkeleton.Joints[JointType.KneeRight];
                        leftKnee = playerSkeleton.Joints[JointType.KneeLeft];
                        rightAnkle = playerSkeleton.Joints[JointType.AnkleRight];
                        leftAnkle = playerSkeleton.Joints[JointType.AnkleLeft];
                        rightFoot = playerSkeleton.Joints[JointType.FootRight];
                        leftFoot = playerSkeleton.Joints[JointType.FootLeft];

                        
                    }
                }
            }
        }

         void kinectSensor_ColorFrameReady(object sender, ColorImageFrameReadyEventArgs e)
        {
            using (colorImageFrame = e.OpenColorImageFrame())
            {
                if (colorImageFrame != null)
                {
                    GC.Collect();
                     pixelsFromFrame = new byte[colorImageFrame.PixelDataLength];
                    colorImageFrame.CopyPixelDataTo(pixelsFromFrame);
                    Color[] color = new Color[colorImageFrame.Height * colorImageFrame.Width];                    
                    kinectRGBVideo = new Texture2D(graphics.GraphicsDevice, colorImageFrame.Width, colorImageFrame.Height);
                    // Go through each pixel and set the bytes correctly
                    // Remember, each pixel has a Red, Green and Blue
                    int index = 0;
                    for (int y = 0; y < colorImageFrame.Height; y++)
                    {
                        for (int x = 0; x < colorImageFrame.Width; x++, index += 4)
                        {
                            color[y * colorImageFrame.Width + x] = new Color(pixelsFromFrame[index + 2],
                                pixelsFromFrame[index + 1], pixelsFromFrame[index + 0]);
                        }
                    }
                    // Set pixeldata from the ColorImageFrame to a Texture2D
                     kinectRGBVideo.SetData(color);
                     recordSkeleton.RecordVideo(pixelsFromFrame);    
                }       
            }
        }

        private void DiscoverKinectSensor()
        {
            foreach (KinectSensor sensor in KinectSensor.KinectSensors)
            {
                if (sensor.Status == KinectStatus.Connected)
                {
                    // Found one, set our sensor to this
                    kinectSensor = sensor;
                    break;
                }
            }

            if (this.kinectSensor == null)
            {
                connectedStatus = "Found none Kinect Sensors connected to USB";
                return;
            }

            // You can use the kinectSensor.Status to check for status
            // and give the user some kind of feedback
            switch (kinectSensor.Status)
            {
                case KinectStatus.Connected:
                    {
                        connectedStatus = "Status: Connected";
                        break;
                    }
                case KinectStatus.Disconnected:
                    {
                        connectedStatus = "Status: Disconnected";
                        break;
                    }
                case KinectStatus.NotPowered:
                    {
                        connectedStatus = "Status: Connect the power";
                        break;
                    }
                default:
                    {
                        connectedStatus = "Status: Error";
                        break;
                    }
            }

            // Init the found and connected device
            if (kinectSensor.Status == KinectStatus.Connected)
            {
                InitializeKinect();
            }
        }

        protected override void Initialize()
        {
            KinectSensor.KinectSensors.StatusChanged += new EventHandler<StatusChangedEventArgs>(KinectSensors_StatusChanged);
            DiscoverKinectSensor();
            oldState = Keyboard.GetState();
            mousePosition = Vector2.Zero;

            textBox = new Rectangle(150, 172, 375, 40);

            point1.X = 0;
            point1.Y = .5F;
            point1.Z = 1.5F;
            point2.X = 0;
            point2.Y = -.5F;
            point2.Z = 1.5F;
           
               
                GC.Collect();  
            base.Initialize();
        }

        protected override void LoadContent()
        {
            spriteBatch = new SpriteBatch(GraphicsDevice);
            kinectRGBVideo = new Texture2D(GraphicsDevice, 1337, 1337);
            mainMenu = Content.Load<Texture2D>("Kinect_Menu");
            exerciseMenu = Content.Load<Texture2D>("ExerciseMenu");
            saveMenu = Content.Load<Texture2D>("SaveMenu");
            overlay = Content.Load<Texture2D>("UTCNSFREU");
            font = Content.Load<SpriteFont>("SpriteFont1");
            mousetexture = Content.Load<Texture2D>("cursor");
            areYouSureMenu = Content.Load<Texture2D>("areYouSureMenu");
            savingPromptScreen = Content.Load<Texture2D>("savingPrompt");
            kinectCalibrationScreen = Content.Load<Texture2D>("kinectCalibrationScreen");
            rightArmHover = Content.Load<Texture2D>("rightArmHover");
            rightLegHover = Content.Load<Texture2D>("rightLegHover");
            leftLegHover = Content.Load<Texture2D>("leftLegHover");
            leftArmHover = Content.Load<Texture2D>("leftArmHover");
            noHover = Content.Load<Texture2D>("noHover");
            yesHover = Content.Load<Texture2D>("yesHover");
            Kinect_Menu_Hover = Content.Load<Texture2D>("Kinect_Menu_Hover");
            continueHover = Content.Load<Texture2D>("continueHover");
            applause = Content.Load<SoundEffect>("Crowd 10 Applause");
            gamePlayMeditation = Content.Load<SoundEffect>("firstchakramin");
            buttonClick = Content.Load<SoundEffect>("Switch Click On Off 3");
            gamePlayMeditationInstance = gamePlayMeditation.CreateInstance();
            debugColor = Content.Load<Texture2D>("solidred");
            enterPatientName = Content.Load<Texture2D>("enterPatientName");
            excellent = Content.Load<Texture2D>("excellent");
            particleTex = Content.Load<Texture2D>("fire");
            MediaPlayer.Volume = 1;
            generator = new Random();
            text = "";
        }

        protected override void UnloadContent()
        {
            kinectSensor.Stop();
            kinectSensor.Dispose();
        }

        protected override void Update(GameTime gameTime)
        {
            MouseState mouseState = Mouse.GetState();
            secondState = Mouse.GetState();
            mousePosition.X = mouseState.X;
            mousePosition.Y = mouseState.Y;
            int x = Mouse.GetState().X;
            int y = Mouse.GetState().Y;
            KeyboardState keyboardState = Keyboard.GetState();
            Keys[] pressedKeys;
            pressedKeys = keyboardState.GetPressedKeys();
            patientNameList = new List<string>();
            switch (state)
            {
                case gameState.titleScreen:
                    if (gamePlayMeditationInstance.State == SoundState.Stopped)
                    {
                        gamePlayMeditationInstance.Volume = 1;
                        gamePlayMeditationInstance.Play();
                    }
                    else
                        gamePlayMeditationInstance.Resume();

                    //tells where the mouse can be clicked and responded to for each button
                    //switches to different gameState if clicked in the proper x, y coordinate

                    if (Mouse.GetState().LeftButton == ButtonState.Pressed)
                    {
                        if ((x > 100) && (x < 510))
                        {
                            if ((y > 300) && (y < 450))
                            {
                                buttonClick.Play();
                                state = gameState.kinectCalibrationScreen;
                            }
                        }
                    }
                    GC.Collect();
                    break;
                case gameState.enterPatientNameScreen:
                    currentState = Keyboard.GetState();

                    if (previousState != currentState)
                    {
                        for (int i = 0; i < pressedKeys.Length; ++i)
                        {
                            patientNameList.Add(pressedKeys[i].ToString());
                            if (keyboardState.IsKeyDown(Keys.Back))
                            {
                                text = text.Remove(text.Length - 1, 1);
                            }
                            else if (keyboardState.IsKeyDown(Keys.Space))
                            {
                                text = text + " ";
                            }
                            else
                            {
                                text = text + patientNameList[i];
                            }
                        }
                    }

                    if (Mouse.GetState().LeftButton == ButtonState.Pressed)
                    {
                        if ((x > 225) && (x < 375))
                        {
                            if ((y > 300) && (y < 375))
                            {
                                buttonClick.Play();
                                state = gameState.kinectCalibrationScreen;
                            }
                        }
                    }

                    previousState = currentState;
                    GC.Collect();
                    break;
                case gameState.kinectCalibrationScreen:
                    if (count < 25)
                    {
                        elevation = calculate.getCameraAngle(head.Position.Y, head.Position.Z, rightFoot.Position.Y, leftFoot.Position.Y);
                        elevation = elevation * -1;
                        elevation2 = (int)(Math.Floor(elevation + .5));
                        count += 1;
                    }

                    if (count >= 25)
                    {
                        if (isfirst == true)
                        {
                            kinectSensor.ElevationAngle = elevation2;
                        }
                        isfirst = false;
                    }
                    state = gameState.excerciseChoiceScreen;                  
                    break;
                case gameState.excerciseChoiceScreen:

                    //tells where the mouse can be clicked and responded to for each button
                    //switches to different gameState if clicked in the proper x, y coordinate
                    if (Mouse.GetState().LeftButton == ButtonState.Pressed)
                    {
                        
                        if (firstState != secondState)
                        {
                            if ((x > 68) && (x < 212))
                            {
                                if ((y > 100) && (y < 160))
                                {
                                    buttonClick.Play();
                                    state = gameState.areYouSureScreen;
                                    exState = exerciseState.rightArm;
                                }
                                else if ((y > 247) && (y < 310))
                                {
                                    buttonClick.Play();
                                    state = gameState.areYouSureScreen;
                                    exState = exerciseState.rightLeg;
                                }
                            }
                            if ((x > 342) && (x < 486))
                            {
                                if ((y > 100) && (y < 160))
                                {
                                    buttonClick.Play();
                                    state = gameState.areYouSureScreen;
                                    exState = exerciseState.leftArm;
                                }
                                else if ((y > 247) && (y < 310))
                                {
                                    buttonClick.Play();
                                    state = gameState.areYouSureScreen;
                                    exState = exerciseState.leftLeg;
                                }
                            }
                        }
                    }
                    firstState = secondState;
                    GC.Collect();
                    break;
                case gameState.areYouSureScreen:
                    if (firstState != secondState)
                    {
                        if (Mouse.GetState().LeftButton == ButtonState.Pressed)
                        {
                           
                            if ((y > 250) && (y < 310))
                            {
                                // user picks YES
                                if ((x > 52) && (x < 200))
                                {
                                    buttonClick.Play();
                                    startTime = DateTime.Now;
                                    state = gameState.mainGame;
                                }
                                //user picks NO+
                                else if ((x > 410) && (x < 550))
                                {
                                    buttonClick.Play();
                                    state = gameState.excerciseChoiceScreen;
                                }
                            }
                        }
                    }
                    firstState = secondState;
                    break;
                case gameState.saveScreen:
                    //tells where the mouse can be clicked and responded to for each button
                    //switches to different gameState if clicked in the proper x, y coordinate
                    if (firstState != secondState)
                    {
                        if (Mouse.GetState().LeftButton == ButtonState.Pressed)
                        {
                           
                            if ((y > 250) && (y < 310))
                            {
                                // user picks YES
                                if ((x > 52) && (x < 200))
                                {
                                    buttonClick.Play();     
                                    recordSkeleton.StopRecording();
                                    gamePlayMeditationInstance.Stop();
                                    applause.Play();
                                    foreach (string k in skeletalJointFileNames)
                                    {
                                        string sourcePath = @"C:\Users\Josh\Desktop\Kinect_Stroke1.6\Kinect_Stroke\Kinect_Stroke\bin\x86\Debug\" + k;
                                        string targetFolder = @"c:\Kinect_Skeletal_Point_Data\" + text + "\\" + exState;
                                        // Create sub-folder if necessary.
                                        if (Directory.Exists(targetFolder) == false)
                                        {
                                            Directory.CreateDirectory(targetFolder);
                                        }

                                        // Build full path for target folder and file.
                                        FileInfo docFileInfo = new FileInfo(sourcePath);
                                        string targetPath = string.Format(@"{0}\{1}", targetFolder, docFileInfo.Name);

                                        // Delete old copy if necessary - copy will not work if it already exists
                                        // in the target folder.
                                        if (File.Exists(targetPath))
                                        {
                                            File.Delete(targetPath);
                                        }

                                        // Copy file to target folder.
                                        File.Copy(sourcePath, targetPath);
                                    }
                                    state = gameState.savingPromptScreen;
                                }
                                //user picks NO+
                                else if ((x > 410) && (x < 550))
                                {
                                    buttonClick.Play();
                                    gamePlayMeditationInstance.Stop();
                                    state = gameState.titleScreen;
                                }
                            }
                        }
                    }
                    startTime2 = DateTime.Now;
                    firstState = secondState;
                    GC.Collect();
                    break;
                case gameState.savingPromptScreen:
                    endTime2 = DateTime.Now;
                    timeSpan2 = endTime2 - startTime2;
                    if (timeSpan2.TotalSeconds > 10)
                    {
                        state = gameState.titleScreen;
                    }
                    break;
                case gameState.mainGame:              
                    currentState = Keyboard.GetState();
                    if (gamePlayMeditationInstance.State == SoundState.Stopped)
                    {
                        gamePlayMeditationInstance.Volume = 1;
                        gamePlayMeditationInstance.Play();
                    }
                    else
                        gamePlayMeditationInstance.Resume();
                    if (previousState != currentState)
                    {
                        if (keyboardState.IsKeyDown(Keys.R))
                        {
                            flagSecondCount = true;
                        }
                        if (keyboardState.IsKeyDown(Keys.P))
                        {
                            if (isPaused == true)
                            {
                                isPaused = false;
                            }
                            else
                                isPaused = true;
                        }
                        if (isPaused == true)
                        {
                            gamePlayMeditationInstance.Pause();
                        }
                        //if spacebar is pressed, game switches to save screen
                        if (Keyboard.GetState().IsKeyDown(Keys.Space))
                        {
                            buttonClick.Play();
                            state = gameState.saveScreen;
                        }
                    }
                   
                    previousState = currentState;
                    
                    break;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.Escape))
            {
                this.Exit();
            }
            if (countRecord % 10 == 0)
            {
                recordSkeleton.StopRecording();
            }
           
            countRecord++;
            base.Update(gameTime);
        } 

        protected override void Draw(GameTime gameTime)
        {
            spriteBatch.Begin();
            Vector2 patientNameVector = new Vector2(180, 240);
            MouseState mouseState = Mouse.GetState();
            secondState = Mouse.GetState();
            mousePosition.X = mouseState.X;
            mousePosition.Y = mouseState.Y;
            int x = Mouse.GetState().X;
            int y = Mouse.GetState().Y;
            switch (state)
            {
                case gameState.titleScreen:
                    spriteBatch.Draw(mainMenu, new Rectangle(0, 0, 640, 480), Color.White);
                    Rectangle startButton = new Rectangle(100, 285, 460, 145);  
                    if (new Rectangle(x, y, 1, 1).Intersects(startButton))
                    {
                        spriteBatch.Draw(Kinect_Menu_Hover, startButton, Color.White);
                    }
                    spriteBatch.Draw(mousetexture, mousePosition, null, Color.White, 0f,
                    Vector2.Zero, 1.0f, SpriteEffects.None, 0f);
                    break;
                case gameState.enterPatientNameScreen:
                    spriteBatch.Draw(enterPatientName, new Rectangle(0, 0, 640, 480), Color.White);
                    Rectangle continueButton = new Rectangle(240, 308, 160, 80);
                    if (new Rectangle(x, y, 1, 1).Intersects(continueButton))
                    {
                        spriteBatch.Draw(continueHover, continueButton, Color.White);
                    }
                    spriteBatch.Draw(mousetexture, mousePosition, null, Color.White, 0f,
                    Vector2.Zero, 1.0f, SpriteEffects.None, 0f);
                    spriteBatch.Draw(debugColor, textBox, Color.White);
                    spriteBatch.DrawString(font, text, new Vector2(textBox.X, textBox.Y), Color.White);
                    break;
                case gameState.kinectCalibrationScreen:
                    spriteBatch.Draw(kinectCalibrationScreen, new Rectangle(0, 0, 640, 480), Color.White);
                    spriteBatch.Draw(mousetexture, mousePosition, null, Color.White, 0f,
                    Vector2.Zero, 1.0f, SpriteEffects.None, 0f);
                    break;
                case gameState.excerciseChoiceScreen:
                    spriteBatch.Draw(exerciseMenu, new Rectangle(0, 0, 640, 480), Color.White);
                    Rectangle rightArm = new Rectangle(80, 90, 160, 80);
                    Rectangle rightLeg = new Rectangle(80, 240, 160, 80);
                    Rectangle leftArm = new Rectangle(355, 90, 160, 80);
                    Rectangle leftLeg = new Rectangle(355, 240, 160, 80);
                    if (new Rectangle(x, y, 1, 1).Intersects(rightArm))
                    {
                        spriteBatch.Draw(rightArmHover, rightArm, Color.White);
                    }
                    if (new Rectangle(x, y, 1, 1).Intersects(rightLeg))
                    {
                        spriteBatch.Draw(rightLegHover, rightLeg, Color.White);
                    }
                    if (new Rectangle(x, y, 1, 1).Intersects(leftArm))
                    {
                        spriteBatch.Draw(leftArmHover, leftArm, Color.White);
                    }
                    else if (new Rectangle(x, y, 1, 1).Intersects(leftLeg))
                    {
                        spriteBatch.Draw(leftLegHover, leftLeg, Color.White);
                    }      
                    spriteBatch.Draw(mousetexture, mousePosition, null, Color.White, 0f,
                    Vector2.Zero, 1.0f, SpriteEffects.None, 0f);
                    break;
                case gameState.areYouSureScreen:
                    spriteBatch.Draw(areYouSureMenu, new Rectangle(0, 0, 640, 480), Color.White);
                    Rectangle yes = new Rectangle(65, 255, 160, 80);
                    Rectangle no = new Rectangle(415, 255, 160, 80);
                    if (new Rectangle(x, y, 1, 1).Intersects(yes))
                    {
                        spriteBatch.Draw(yesHover, yes, Color.White);
                    }

                    else if (new Rectangle(x, y, 1, 1).Intersects(no))
                    {
                        spriteBatch.Draw(noHover, no, Color.White);
                    }

                    spriteBatch.Draw(mousetexture, mousePosition, null, Color.White, 0f,
                    Vector2.Zero, 1.0f, SpriteEffects.None, 0f);
                    break;
                case gameState.saveScreen:
                    spriteBatch.Draw(saveMenu, new Rectangle(0, 0, 640, 480), Color.White);
                    yes = new Rectangle(65, 255, 160, 80);
                    no = new Rectangle(415, 255, 160, 80);

                    if (new Rectangle(x, y, 1, 1).Intersects(yes))
                    {
                        spriteBatch.Draw(yesHover, yes, Color.White);
                    }

                    else if (new Rectangle(x, y, 1, 1).Intersects(no))
                    {
                        spriteBatch.Draw(noHover, no, Color.White);
                    }

                    spriteBatch.Draw(mousetexture, mousePosition, null, Color.White, 0f,
                    Vector2.Zero, 1.0f, SpriteEffects.None, 0f);
                    break;
                case gameState.savingPromptScreen:
                    spriteBatch.Draw(savingPromptScreen, new Rectangle(0, 0, 640, 480), Color.White);
                    spriteBatch.Draw(mousetexture, mousePosition, null, Color.White, 0f,
                    Vector2.Zero, 1.0f, SpriteEffects.None, 0f);
                    break;
                case gameState.mainGame:
                    switch (exState)
                    {
                        case exerciseState.rightArm:
                            angle = calculate.readarmangle(skeletonData, JointType.ElbowRight, JointType.ShoulderRight, angle);
                            angle = calculate.angleRounding(angle);
                            angle2 = (360 - angle) - 180;

                            if (angle2 < 1 && angle2 > -90)
                            {
                                angle2 = 0;

                            }
                            else if (angle2 >= -179 && angle2 <= -80)
                            {
                                angle2 = 180;
                            }                          
                            break;
                        case exerciseState.leftArm:
                            angle = calculate.readarmangle(skeletonData, JointType.ElbowLeft, JointType.ShoulderLeft, angle);
                            angle = calculate.angleRounding(angle);
                            angle2 = 180 - (360 - angle); //- 180;

                            if (angle2 < 1 && angle2 > -90)
                            {
                                angle2 = 0;

                            }
                            else if (angle2 >= -180 && angle2 <= -80)
                            {
                                angle2 = 180;
                            }
                            break;
                        case exerciseState.rightLeg:
                            legAngle = calculate.angleBetweenTwoVectorsAndJoint(point1, rightAnkle, point2);
                            break;
                        case exerciseState.leftLeg:
                            legAngle = calculate.angleBetweenTwoVectorsAndJoint(point1, rightAnkle, point2);
                            break;
                    }


                    GraphicsDevice.Clear(Color.CornflowerBlue);
                    spriteBatch.Draw(kinectRGBVideo, new Rectangle(0, 0, 640, 480), Color.White);
                    spriteBatch.Draw(overlay, new Rectangle(0, 0, 640, 480), Color.White);
                    spriteBatch.DrawString(font, connectedStatus, new Vector2(20, 80), Color.White);
                    endTime = DateTime.Now;
                    timeSpan = endTime - startTime;
                    timeDifference = timeSpan.TotalSeconds;
                    if (flagSecondCount == true)
                    {
                        startFlag = "START";
                        writerAngle.WriteLine(startFlag);
                    }
                    else
                    {
                        startFlag = " ";
                    }

                    writerHead.WriteLine(head.Position.X + "," + head.Position.Y + "," +
                                             head.Position.Z + "," + timeDifference);
                    writerRightHand.WriteLine(rightHand.Position.X + "," + rightHand.Position.Y + ","
                                            + rightHand.Position.Z + "," + timeDifference);
                    writerLeftHand.WriteLine(+leftHand.Position.X + "," + leftHand.Position.Y + ","
                                            + leftHand.Position.Z + "," + timeDifference);
                    writerRightWrist.WriteLine(rightWrist.Position.X + "," + rightWrist.Position.Y + ","
                                            + rightWrist.Position.Z + "," + timeDifference);
                    writerLeftWrist.WriteLine(leftWrist.Position.X + "," + leftWrist.Position.Y + ","
                                            + leftWrist.Position.Z + "," + timeDifference);
                    writerRightElbow.WriteLine(rightElbow.Position.X + "," + rightElbow.Position.Y + ","
                                            + rightElbow.Position.Z + "," + timeDifference);
                    writerLeftElbow.WriteLine(leftElbow.Position.X + "," + leftElbow.Position.Y + ","
                                            + leftElbow.Position.Z + "," + timeDifference);
                    writerRightShoulder.WriteLine(rightShoulder.Position.X + "," + rightShoulder.Position.Y + ","
                                            + rightShoulder.Position.Z + "," + timeDifference);
                    writerLeftShoulder.WriteLine(leftShoulder.Position.X + "," + leftShoulder.Position.Y + ","
                                            + leftShoulder.Position.Z + "," + timeDifference);
                    writerShoulderCenter.WriteLine(shoulderCenter.Position.X + "," + shoulderCenter.Position.Y + ","
                                            + shoulderCenter.Position.Z + "," + timeDifference);
                    writerSpine.WriteLine(spine.Position.X + "," + spine.Position.Y + ","
                                            + spine.Position.Z + "," + timeDifference);
                    writerHipCenter.WriteLine(hipCenter.Position.X + "," + hipCenter.Position.Y + ","
                                            + hipCenter.Position.Z + "," + timeDifference);
                    writerRightHip.WriteLine(rightHip.Position.X + "," + rightHip.Position.Y + ","
                                            + rightHip.Position.Z + "," + timeDifference);
                    writerLeftHip.WriteLine(leftHip.Position.X + "," + leftHip.Position.Y + ","
                                            + leftHip.Position.Z + "," + timeDifference);
                    writerRightKnee.WriteLine(rightKnee.Position.X + "," + rightKnee.Position.Y + ","
                                            + rightKnee.Position.Z + "," + timeDifference);
                    writerLeftKnee.WriteLine(leftKnee.Position.X + "," + leftKnee.Position.Y + ","
                                            + leftKnee.Position.Z + "," + timeDifference);
                    writerRightAnkle.WriteLine(rightAnkle.Position.X + "," + rightAnkle.Position.Y + ","
                                            + rightAnkle.Position.Z + "," + timeDifference);
                    writerLeftAnkle.WriteLine(leftAnkle.Position.X + "," + leftAnkle.Position.Y + ","
                                            + leftAnkle.Position.Z + "," + timeDifference);
                    writerRightFoot.WriteLine(rightFoot.Position.X + "," + rightFoot.Position.Y + ","
                                            + rightFoot.Position.Z + "," + timeDifference);
                    writerLeftFoot.WriteLine(leftFoot.Position.X + "," + leftFoot.Position.Y + ","
                                            + leftFoot.Position.Z + "," + timeDifference);
                    writerElevation.WriteLine(elevation2 + "," + timeDifference);
                    writerAngle.WriteLine(angle2 + "," + timeDifference);
                    writerLegAngle.WriteLine(legAngle + "," + timeDifference);
                    flagSecondCount = false;
                    break;
            }
            spriteBatch.End();
            base.Draw(gameTime);
        }    
    }
}