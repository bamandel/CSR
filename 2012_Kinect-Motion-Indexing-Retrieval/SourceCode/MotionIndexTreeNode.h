/* 
 * File:   MotionIndexTreeNode.h
 * Author: Chelsea
 *
 * Created on June 7, 2012, 9:22 PM
 *
 * Represents a Joint node in a tree.  Each node has x, y, z values
 * and a timestamp of when this joint was recorded.
 */
#include <iostream>
#include <vector>
using namespace std;

#ifndef MOTIONINDEXTREENODE_H
#define	MOTIONINDEXTREENODE_H
class MotionIndexTreeNode {
public:
    MotionIndexTreeNode(){
        x=0;
        y=0;
        z=0;
        leftSibling = NULL;
        rightSibling = NULL;
        child = NULL;
    }
    MotionIndexTreeNode(const MotionIndexTreeNode& orig);
    virtual ~MotionIndexTreeNode(){
        
    }
    MotionIndexTreeNode* getLeftSibling(){
        return leftSibling;
    }
    MotionIndexTreeNode * getRightSibling(){
        return rightSibling;
    }
    MotionIndexTreeNode* getChild(){
        return child;
    }
    void addChild(MotionIndexTreeNode * childNode){
        child = childNode;
    }
    double getXPosition(){
        return x;
    }
    double getYPosition(){
        return y;
    }
    double getZPosition(){
        return z;
    }
    string getTimestamp(){
        return timestamp;
    }
    string getName(){
        return jointName;
    }
    void setXPosition(double xpos){
        x = xpos;
    }
    void setYPosition(double ypos){
        y = ypos;
    }
    void setZPosition(double zpos){
        z = zpos;
    }
    void setTimestamp(string time){
        timestamp = time;
    }
    void setName(string name){
        jointName = name;
    }
    void setLeftSibling(MotionIndexTreeNode * left){
        leftSibling = left;
    }
    void setRightSibling(MotionIndexTreeNode * right){
        rightSibling = right;
    }
    void setPath(string name){
        path = name;
    }
    string getPath(){
        return path;
    }
private:
    MotionIndexTreeNode * child;
    MotionIndexTreeNode * leftSibling;
    MotionIndexTreeNode * rightSibling;
    double x;
    double y;
    double z;
    string timestamp;
    string jointName;
    string path;
};

#endif	/* MOTIONINDEXTREENODE_H */

