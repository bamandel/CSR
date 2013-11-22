/* 
 * File:   Matching.cpp
 * Author: Chelsea
 * 
 * Created on June 25, 2012, 1:05 PM
 */

#include "Matching.h"
#include "DTW.h"
using namespace std;
Matching::Matching(string exerciseName) {
    startTime = 0;
    endTime = 0;
    exercise = exerciseName;
}

Matching::Matching(const Matching& orig) {
}

Matching::~Matching() {
}
/*
 * Used for approach two to compare the angles between the query and all
 * library exercises using Dynamic Time Warping
 */
double Matching::matchAngles(string queryPath, string libraryPath){

    string line;
    vector<double> queryAngles;
    vector<double> libraryAngles;
    ifstream infile;
    infile.open(queryPath.c_str(),ifstream::in);
    if(infile){
        while(getline(infile,line)){
            queryAngles.push_back(atof(line.c_str()));
        }
    }
    infile.close();
    line.clear();
    ifstream infile2;
    infile2.open(libraryPath.c_str(),ifstream::in);
    if(infile2){
        while(getline(infile2,line)){
            libraryAngles.push_back(atof(line.c_str()));
        }
    }
    infile2.close();
    line.clear();

    DTW dtw(queryAngles,libraryAngles);
    return dtw.getDistance();
}
/*
 * Used for approach 3, rule-based algorithm
 */
int Matching::scanFile(string filename){
    ifstream infile;
    int count;
    bool inRange = false;
    string line,token;
    infile.open(filename.c_str(),ifstream::in);
    if(infile){
        while(getline(infile,line)){
            stringstream iss;
            iss << line;
            count = 0;
            while(getline(iss,token,',')){
                if(line == "START\r"){ //find where exercise begins
                    inRange = true;
                    continue;
                }
                if(count == 0 && !inRange)
                    angles.push_back(atof(token.c_str()));
                else if (count == 0 && inRange)
                    rangeAngles.push_back(atof(token.c_str()));
                else if (count==1 && !inRange)
                    times.push_back(atoi(token.c_str()));
                else if (count ==1 && inRange){
                    rangeTimes.push_back(atof(token.c_str()));
                    if(startTime == 0)
                        startTime = atof(token.c_str());
                }
                count++;
            }
        }
    }
    infile.close();
    endTime = rangeTimes.at(rangeTimes.size()-1);

    int margin = 0;
    int holdTime = 0;
    int MIN0 = 0;
    int MIN1 = 0;
    int MIN2 = 0;
    double baselineAverage = averageVals(angles,0);
    if(exercise == "leftArm"){ //each exercise has own ranges
        MIN0 = 85;
        MIN1 = baselineAverage+5;
        MIN2 = 45;
        margin = 5;
        holdTime = 10;
    }
    else if (exercise == "rightArm"){
        MIN0 = 80;
        MIN1 = baselineAverage+5;
        MIN2 = 35;
        margin = 5;
        holdTime = 10;
    }
    else if (exercise == "rightLeg" || exercise=="leftLeg"){
        MIN0 = 38;
        MIN1 = baselineAverage+2;
        MIN2 = 35;
        margin = 2;
        holdTime= 5;
    }
    bool range2 = false;
    bool range0 = false;
    int index = 0; //represents the first occurance of maximum range hit
    for(int i=0;i<rangeAngles.size();i++){
        if(rangeAngles.at(i)>= MIN0) {//have hit 90 degrees
            if(range0==false)
                index = i;
            range0 = true;
        }
        else if (rangeAngles.at(i)>= MIN1 && rangeAngles.at(i) <= MIN2){//some movement of limb
            if(index ==0 && range2 == false && range0 == false)
                index = i;
        }
        else if (rangeAngles.at(i)>= MIN2 && rangeAngles.at(i) <= MIN0){ //raise limb enough to resist gravity
            if(index ==0 && range0==false)
                index = i;
            range2 = true;
        }
    }
    int place = 0; //count ten or five seconds from hitting maximum range
    bool recorded10secs = false;
        for(int i=index;i<rangeTimes.size();i++){
            if(rangeTimes.at(i)>=rangeTimes.at(index)+holdTime){
                place = i;
                recorded10secs = true;
                break;
            }
        }
    if(!recorded10secs){ //recording was not long enough to determine point value
        
        return -1;
    }
    //check performance specifically during ten seconds
    int counter = 0; //counts how many elements are in ten second range
    double range0Counter = 0;
    double range2Counter = 0;
    double range3Counter = 0;
    for(int k=index;rangeTimes.at(k)<rangeTimes.at(place);k++){
        if(rangeAngles.at(k)>= MIN0) {//have hit 90 degrees
            range0Counter++;
        }
        else if (rangeAngles.at(k)>= MIN1 && rangeAngles.at(k) <= MIN2){//some movement of limb
            range3Counter++;
        }
        else if (rangeAngles.at(k)>= MIN2 && rangeAngles.at(k) <= MIN0){ //raise limb enough to resist gravity
            range2Counter++;
        }

        counter++;
    }
    //cout<<rangeTimes.at(index)<<","<<rangeTimes.at(place)<<endl;
    //cout<<rangeTimes.at(index)<<" "<<rangeTimes.at(place)<<endl;
    //cout<<index<<endl;
    if(range0Counter!=0 && range0Counter > counter-5) //held 90 degrees for full 10 seconds
        return 0;
    
    else if (range0Counter > 0){ //hit 90 degrees but either hit bed (2) or did not hit bed (1)
        if(rangeAngles.at(place) > baselineAverage-margin && rangeAngles.at(place) < baselineAverage+margin) //hit bed
            return 2;
        else
            return 1; //did not hit bed
    }
    else{
        if (range3Counter ==0 && range2Counter==0 )
            return 4; //no movement
        else{
            for(int k=index;k<rangeTimes.size();k++){
                if(rangeTimes.at(k)>=rangeTimes.at(index)+3){
                    if(rangeAngles.at(k) > baselineAverage-margin && rangeAngles.at(k)<baselineAverage+margin)
                        return 3; //cannot hold limb against gravity, falls within 3 seconds
                    else
                        return 2; //can hold against gravity
                }
            }

        }
    }
    return -1; //patient's movements did not match to any point value on stroke scale

}
double Matching::average(vector<double> nums){
    double average = 0;
    for(int i=0;i<nums.size();i++){
        average+=nums.at(i);
    }
    return average/nums.size();
}
double Matching::averageVals(vector<double>nums, int start){
    double average = 0;
    for(int i=0;i<20;i++,start++)
        average+=nums.at(start);
    return average/20;
}

int Matching::findMin(int a, int b, int c){
    if(a<= b && a<=c)
        return a;
    else if (b<=a && b<=c)
        return b;
    else if (c<=a && c<=b)
        return c;
}
int Matching::findMaxIndex(){
    double max = rangeAngles.at(0);
    int index = 0;
    for(int i=0;i<rangeAngles.size();i++){
        if(rangeAngles.at(i)>max){
            max = rangeAngles.at(i);
            index = i;
        }

    }
    return index;
}
