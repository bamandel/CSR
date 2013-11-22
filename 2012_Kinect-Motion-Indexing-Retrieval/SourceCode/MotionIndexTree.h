/* 
 * File:   MotionIndexTree.h
 * Author: Chelsea
 *
 * Created on June 5, 2012, 9:58 AM
 *
 * Represents a tree of MotionIndexTreeNodes that hold joints together.
 * Joints that are currently being exercised are the joints structured in tree.
 */
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <map>
#include "MotionIndexTreeNode.h"
#ifndef MotionIndexTree_H
#define	MotionIndexTree_H
using namespace std;
class MotionIndexTree {
public:
    MotionIndexTree(string);
    MotionIndexTree(const MotionIndexTree& orig);
    virtual ~MotionIndexTree();
    MotionIndexTreeNode* AnkleLeft;
    MotionIndexTreeNode* AnkleRight;
    MotionIndexTreeNode* ElbowLeft;
    MotionIndexTreeNode* ElbowRight;
    MotionIndexTreeNode* FootLeft;
    MotionIndexTreeNode* FootRight;
    MotionIndexTreeNode* HandLeft;
    MotionIndexTreeNode* HandRight;
    MotionIndexTreeNode* Head;
    MotionIndexTreeNode* HipCenter;
    MotionIndexTreeNode* HipLeft;
    MotionIndexTreeNode* HipRight;
    MotionIndexTreeNode* KneeLeft;
    MotionIndexTreeNode* KneeRight;
    MotionIndexTreeNode* ShoulderCenter;
    MotionIndexTreeNode* ShoulderLeft;
    MotionIndexTreeNode* ShoulderRight;
    MotionIndexTreeNode* Spine;
    MotionIndexTreeNode* WristLeft;
    MotionIndexTreeNode* WristRight;
    MotionIndexTree * leftArmExercise();
    MotionIndexTree * rightArmExercise();
    MotionIndexTree * leftLegExercise();
    MotionIndexTree * rightLegExercise();
    string getExerciseName();
    MotionIndexTreeNode* getRoot();
    void setCollection(string);
    string getCollection();
    MotionIndexTreeNode *  getJointCoordinates(string);
    map<string,MotionIndexTreeNode*> nameMap;
    string pathname;
private:
    void populateCoordinates(MotionIndexTreeNode*,string);
    void recurseDelete(MotionIndexTreeNode*);
    MotionIndexTreeNode* root;
    string collection;
    
    int frame;
    string exerciseName;
    string patientName;
    

};

#endif	/* MotionIndexTree_H */

