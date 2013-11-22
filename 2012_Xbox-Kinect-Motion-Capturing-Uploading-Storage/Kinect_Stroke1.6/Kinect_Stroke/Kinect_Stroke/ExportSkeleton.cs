using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Microsoft.Kinect;
using Coding4Fun.Kinect.Wpf;
using System.Collections;
using AForge.Video.FFMPEG;
using AForge.Video;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Threading;

namespace SkeletalTracking.Utils.Export
{
    public class RecordSkeleton
    {
        public static double Count = 0;
        double[] recordedSamples = new double[4];
        string videoFolder = null;
        public bool running = false;
        VideoFileWriter writer1;
        public static int videoWidth = 320;
        public static int videoHeight = 240;
        public static int videoWidthColor = 640;
        public static int videoHeightColor = 480;
        private static readonly int Bgr32BytesPerPixel = (PixelFormats.Bgr32.BitsPerPixel + 7) / 8;

        public RecordSkeleton()
        {
            videoFolder = System.IO.Path.Combine(System.AppDomain.CurrentDomain.BaseDirectory, "Video");
            if (!Directory.Exists(videoFolder))
            {
                Directory.CreateDirectory(videoFolder);
            }
            Count++;
            writer1 = new VideoFileWriter();
            writer1.Open(System.IO.Path.Combine(videoFolder, Count.ToString() + "_color.avi"), videoWidthColor, videoHeightColor, 25, VideoCodec.MPEG4);
        }
        // 
        List<short[]> depthFrames = new List<short[]>();
        List<byte[]> depthFrames1 = new List<byte[]>();
        Thread collectorThread1;
        public void RecordVideo(short[] depthData)
        {
            depthFrames.Add(depthData);
            if (depthFrames1.Count >= 200)
            {
                if (collectorThread1 == null)
                {
                    collectorThread1 = new Thread(new ThreadStart(WriterCollectorThread1));
                    collectorThread1.IsBackground = true;
                }
                if (!collectorThread1.IsAlive)
                {
                    collectorThread1 = new Thread(new ThreadStart(WriterCollectorThread1));
                    collectorThread1.IsBackground = true;
                    collectorThread1.Start();
                }
            }
        }

        private void WriterCollectorThread1()
        {
            WriteableBitmap outputImage = new WriteableBitmap(
                            videoWidthColor,
                            videoHeightColor,
                            96,  // DpiX
                            96,  // DpiY
                            PixelFormats.Bgr32,
                            null);
            for (int i = 0; i < 200; i++)
            {
                byte[] pixels = depthFrames1[0];
                depthFrames1.RemoveAt(0);
                outputImage.WritePixels(
                        new Int32Rect(0, 0, videoWidthColor, videoHeightColor),
                        pixels,
                        videoWidthColor * Bgr32BytesPerPixel,
                        0);

                writer1.WriteVideoFrame(GetBitmap(outputImage));
            }
            GC.Collect();
        }

        public void RecordVideo(byte[] colorData)
        {
            depthFrames1.Add(colorData);
        }

        Bitmap GetBitmap(BitmapSource source)
        {
            GC.Collect();
            Bitmap bmp = new Bitmap(
              640, 480, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
            BitmapData data = bmp.LockBits(
              new System.Drawing.Rectangle(System.Drawing.Point.Empty, bmp.Size),
              ImageLockMode.WriteOnly,
              System.Drawing.Imaging.PixelFormat.Format32bppPArgb);
            source.CopyPixels(
              Int32Rect.Empty,
              data.Scan0,
              data.Height * data.Stride,
              data.Stride);
            bmp.UnlockBits(data);
            return bmp;           
        }
        bool save_started = false;
        private void SaveVideo()
        {
            
            if (save_started) return;
            save_started = true;
            if (collectorThread1 != null) collectorThread1.Abort();
            GC.Collect();
            WriteableBitmap outputImage = new WriteableBitmap(
                            videoWidth,
                            videoHeight,
                            96,  // DpiX
                            96,  // DpiY
                            PixelFormats.Bgr32,
                            null);          
            GC.Collect();
            outputImage = new WriteableBitmap(
                            videoWidthColor,
                            videoHeightColor,
                            96,  // DpiX
                            96,  // DpiY
                            PixelFormats.Bgr32,
                            null);
            for (int i = 0; i < depthFrames1.Count; i++)
            {
                byte[] pixels = depthFrames1[i];
                outputImage.WritePixels(
                        new Int32Rect(0, 0, videoWidthColor, videoHeightColor),
                        pixels,
                        videoWidthColor * Bgr32BytesPerPixel,
                        0);
                if (writer1 != null)
                {
                    if (writer1.IsOpen) writer1.WriteVideoFrame(GetBitmap(outputImage));
                }
            }
            if (writer1 != null)
            {
                writer1.Close();
            }
            
            depthFrames.Clear();
            depthFrames1.Clear();
            save_started = false;
        }
        Thread oThread;
        public void StopRecording()
        {
            oThread = new Thread(new ThreadStart(SaveVideo));
            oThread.Start();
        }
 
    }
}
