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

namespace Kinect_Stroke
{
    class Calculations
    {
        double PI = Math.PI;

        // returns angle of joint b relative to joints a and c
        public double angleBetweenJoints(Joint a, Joint b, Joint c)
        {

            // calculate vector from b to a
            double x1 = a.Position.X - b.Position.X, y1 = a.Position.Y - b.Position.Y, z1 = a.Position.Z - b.Position.Z;

            // calculate vector from b to c
            double x2 = c.Position.X - b.Position.X, y2 = c.Position.Y - b.Position.Y, z2 = c.Position.Z - b.Position.Z;

            // calculate angle between the two vectors
            double angleInRadians = Math.Acos((x1 * x2 + y1 * y2 + z1 * z2) / Math.Sqrt(x1 * x1 + y1 * y1 + z1 * z1) / Math.Sqrt(x2 * x2 + y2 * y2 + z2 * z2));

            // return angle in degrees
            return angleInRadians * 180 / Math.PI;
        }


        public double angleBetweenTwoVectorsAndJoint(Vector3 a, Joint b, Vector3 c)
        {

            // calculate vector from b to a
            double x1 = a.X - b.Position.X, y1 = a.Y - b.Position.Y, z1 = a.Z - b.Position.Z;

            // calculate vector from b to c
            double x2 = c.X - b.Position.X, y2 = c.Y - b.Position.Y, z2 = c.Z - b.Position.Z;

            // calculate angle between the two vectors
            double angleInRadians = Math.Acos((x1 * x2 + y1 * y2 + z1 * z2) / Math.Sqrt(x1 * x1 + y1 * y1 + z1 * z1) / Math.Sqrt(x2 * x2 + y2 * y2 + z2 * z2));

            // return angle in degrees
            return angleInRadians * 180 / Math.PI;
        }

        public int readarmangle(Skeleton[] skeldata, JointType joint1, JointType joint2, int ang1)
        {
            ang1 = 0;
            Vector2 joint1pos = new Vector2(0, 0);
            Vector2 joint2pos = new Vector2(0, 0);


            int ang = 0;

            foreach (Skeleton data in skeldata)
            {
                if (data.TrackingState == SkeletonTrackingState.Tracked)
                {
                    foreach (Joint joint in data.Joints)
                    {
                        if (joint.JointType == joint1)
                        {
                            joint1pos.X = joint.Position.X;
                            joint1pos.Y = joint.Position.Y;
                        }
                        if (joint.JointType == joint2)
                        {
                            joint2pos.X = joint.Position.X;
                            joint2pos.Y = joint.Position.Y;
                        }
                    }

                    Vector2 anglevec = joint1pos - joint2pos;
                    ang = (int)MathHelper.ToDegrees((float)Math.Atan2(anglevec.X, anglevec.Y));
                    if (ang < 0) ang += 360;


                }
            }
            return ang;
        }

        public float getCameraAngle(float head_Y, float head_Z, float rightFoot_Y, float leftFoot_Y)//float rightFoot)
        {

            // find lowest foot
            float minimumFoot = rightFoot_Y < leftFoot_Y ? rightFoot_Y : leftFoot_Y;

            // calculate y position of the midpoint between the head and foot
            float yOffset = (head_Y + minimumFoot) / 2;

            // calculate the angle between the midpoint and the z axis
            return (float)(((Math.Atan(Math.Abs(yOffset) / head_Z)) * 180) / PI);

        }

        public int angleRounding(int angle)
        {
            angle = (int)(Math.Round((float)angle / (float)1, 0) * 1);
            return angle;
        }

   
    }
}
