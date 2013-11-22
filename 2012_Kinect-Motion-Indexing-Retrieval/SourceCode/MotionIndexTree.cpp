/* 
 * File:   MotionIndexTree.cpp
 * Author: Chelsea
 * 
 * Created on June 5, 2012, 9:58 AM
 */

#include "MotionIndexTree.h"

using namespace std;

MotionIndexTree::MotionIndexTree(string path) {
    pathname = path;
    //stringstream ss;//create a stringstream
    //ss << collectionNo<<"/";//add number to the stream
    //collection = ss.str();
    //frame = frameNo;
    root = NULL;
    //initialize 20 joints and set their joint names
    AnkleLeft = new MotionIndexTreeNode();  AnkleLeft->setName("ankleLeft"); nameMap["ankleLeft"] = AnkleLeft;
    AnkleRight = new MotionIndexTreeNode(); AnkleRight->setName("ankleRight"); nameMap["ankleRight"] = AnkleRight;
    ElbowLeft = new MotionIndexTreeNode();  ElbowLeft->setName("elbowLeft"); nameMap["elbowLeft"] = ElbowLeft;
    ElbowRight = new MotionIndexTreeNode(); ElbowRight->setName("elbowRight"); nameMap["elbowRight"] = ElbowRight;
    FootLeft = new MotionIndexTreeNode();   FootLeft->setName("eootLeft"); nameMap["footLeft"] = FootLeft;
    FootRight = new MotionIndexTreeNode();  FootRight->setName("footRight"); nameMap["footRight"] = FootRight;
    HandLeft = new MotionIndexTreeNode();   HandLeft->setName("handLeft"); nameMap["handLeft"] = HandLeft;
    HandRight = new MotionIndexTreeNode();  HandRight->setName("handRight"); nameMap["handRight"] = HandRight;
    Head = new MotionIndexTreeNode();       Head->setName("head"); nameMap["head"] = Head;
    HipCenter = new MotionIndexTreeNode();  HipCenter->setName("hipCenter"); nameMap["hipCenter"] = HipCenter;
    HipLeft = new MotionIndexTreeNode();    HipLeft->setName("hipLeft"); nameMap["hipLeft"] = HipLeft;
    HipRight = new MotionIndexTreeNode();   HipRight->setName("hipRight"); nameMap["hipRight"] = HipRight;
    KneeLeft = new MotionIndexTreeNode();   KneeLeft->setName("kneeLeft");nameMap["kneeLeft"] = KneeLeft;
    KneeRight = new MotionIndexTreeNode();  KneeRight->setName("kneeRight"); nameMap["kneeRight"] = KneeRight;
    ShoulderCenter = new MotionIndexTreeNode(); ShoulderCenter->setName("shoulderCenter"); nameMap["shoulderCenter"] = ShoulderCenter;
    ShoulderLeft = new MotionIndexTreeNode();   ShoulderLeft->setName("shoulderLeft"); nameMap["shoulderLeft"] = ShoulderLeft;
    ShoulderRight = new MotionIndexTreeNode();  ShoulderRight->setName("shoulderRight"); nameMap["shoulderRight"] = ShoulderRight;
    Spine = new MotionIndexTreeNode();      Spine->setName("spine"); nameMap["spine"] = Spine;
    WristLeft = new MotionIndexTreeNode();  WristLeft->setName("wristLeft"); nameMap["wristLeft"] = WristLeft;
    WristRight = new MotionIndexTreeNode(); WristRight->setName("wristRight"); nameMap["wristRight"] = WristRight;

}
MotionIndexTree::MotionIndexTree(const MotionIndexTree& orig){

}
MotionIndexTree::~MotionIndexTree(){
    recurseDelete(root);
}
/*
 * Build motion index tree for a left arm exercise.
 * Populate coordinates by checking text files for each joint
 * that contain the joint coordinates
 */
MotionIndexTree * MotionIndexTree::leftArmExercise(){
    root = Spine;
    Spine->addChild(ShoulderLeft);
    ShoulderLeft->addChild(ElbowLeft);
    ElbowLeft->addChild(WristLeft);
    WristLeft->addChild(HandLeft);
    exerciseName = "leftArm";
    Spine->setPath(pathname+"spine.txt");
    //populateCoordinates(Spine,Spine->getPath());
    ShoulderLeft->setPath(pathname+"shoulderLeft.txt");
    //populateCoordinates(ShoulderLeft,ShoulderLeft->getPath());
    ElbowLeft->setPath(pathname+"elbowLeft.txt");
    //populateCoordinates(ElbowLeft,ElbowLeft->getPath());
    WristLeft->setPath(pathname+"wristLeft.txt");
    //populateCoordinates(WristLeft,WristLeft->getPath());
    HandLeft->setPath(pathname+"handLeft.txt");
    //populateCoordinates(HandLeft,HandLeft->getPath());
    return this;
}
/*
 * Build motion index tree for a right arm exercise.
 * Populate coordinates by checking text files for each joint
 * that contain the joint coordinates
 */
MotionIndexTree* MotionIndexTree::rightArmExercise(){
    root = Spine;
    Spine->addChild(ShoulderRight);
    ShoulderRight->addChild(ElbowRight);
    ElbowRight->addChild(WristRight);
    WristRight->addChild(HandRight);
    exerciseName = "rightArm";
    Spine->setPath(pathname+"spine.txt");
    //populateCoordinates(Spine,Spine->getPath());
    ShoulderRight->setPath(pathname+"shoulderRight.txt");
    //populateCoordinates(ShoulderRight,ShoulderRight->getPath());
    ElbowRight->setPath(pathname+"elbowRight.txt");
    //populateCoordinates(ElbowRight,ElbowRight->getPath());
    WristRight->setPath(pathname+"wristRight.txt");
    //populateCoordinates(WristRight,WristRight->getPath());
    HandRight->setPath(pathname+"handRight.txt");
    //populateCoordinates(HandRight,HandRight->getPath());
    return this;
}

/*
 * Build motion index tree for a left leg exercise.
 * Populate coordinates by checking text files for each joint
 * that contain the joint coordinates
 */
MotionIndexTree* MotionIndexTree::leftLegExercise(){
    root = HipLeft;
    HipLeft->addChild(KneeLeft);
    KneeLeft->addChild(AnkleLeft);
    AnkleLeft->addChild(FootLeft);
    exerciseName = "leftLeg";
    HipLeft->setPath(pathname+"hipLeft.txt");
    //populateCoordinates(HipLeft,HipLeft->getPath());
    KneeLeft->setPath(pathname+"kneeLeft.txt");
    //populateCoordinates(KneeLeft,KneeLeft->getPath());
    AnkleLeft->setPath(pathname+"ankleLeft.txt");
    //populateCoordinates(AnkleLeft,AnkleLeft->getPath());
    FootLeft->setPath(pathname+"footLeft.txt");
    //populateCoordinates(FootLeft,FootLeft->getPath());
    return this;
}
/*
 * Build motion index tree for a right arm exercise.
 * Populate coordinates by checking text files for each joint
 * that contain the joint coordinates
 */
MotionIndexTree* MotionIndexTree::rightLegExercise(){
    root = HipRight;
    HipRight->addChild(KneeRight);
    KneeRight->addChild(AnkleRight);
    AnkleRight->addChild(FootRight);
    exerciseName = "rightLeg";
    HipRight->setPath(pathname+"hipRight.txt");
    //populateCoordinates(HipRight,HipRight->getPath());
    KneeRight->setPath(pathname+"kneeRight.txt");
    //populateCoordinates(KneeRight,KneeRight->getPath());
    AnkleRight->setPath(pathname+"ankleRight.txt");
    //populateCoordinates(AnkleRight,AnkleRight->getPath());
    FootRight->setPath(pathname+"footRight.txt");
    //populateCoordinates(FootRight,FootRight->getPath());
    return this;
}
/*
 * Return the name of the current exercised being performed
 */
string MotionIndexTree::getExerciseName(){
    return exerciseName;
}
/*
 * param node-MotionIndexTreeNode (joint node) whose x,y,z coordinates will be set
 * param filename- filename of where to find text file coordinates for this joint node
 */
void MotionIndexTree::populateCoordinates(MotionIndexTreeNode* node, string filename){
    ifstream infile;
    string data,token;
    int count = 0;
    infile.open (filename.c_str(), ifstream::in);
    getline(infile,data); //only want coordinates from start frame, this is the first line of text file
    infile.close();
    istringstream iss(data);
    while ( getline(iss, token, ',') ) //tokenize file; x,y,z coordinates seprated by commas
    {
        if(count==0)
            node->setXPosition(atof(token.c_str()));
        else if (count==1)
            node->setYPosition(atof(token.c_str()));
        else if (count==2)
            node->setZPosition(atof(token.c_str()));
        else
            node->setTimestamp(token);
        count++;
    }
    iss.clear();
}
/*
 * Return the root to this motion index tree
 */
MotionIndexTreeNode* MotionIndexTree::getRoot(){
    return root;
}
void MotionIndexTree::setCollection(string no){
    collection = no;
}
string MotionIndexTree::getCollection(){
    return collection;
}
MotionIndexTreeNode * MotionIndexTree::getJointCoordinates(string name){
    return nameMap[name];
}
void MotionIndexTree::recurseDelete(MotionIndexTreeNode* node){
     if(node!=NULL){
            recurseDelete(node->getRightSibling());
            recurseDelete(node->getChild());
            delete node;
    }

}