/* 
 * File:   Matching.h
 * Author: Chelsea
 *
 * Created on June 25, 2012, 1:05 PM
 */
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <math.h>

#ifndef MATCHING_H
#define	MATCHING_H
using namespace std;
class Matching {
public:
    Matching(string);
    Matching(const Matching& orig);
    virtual ~Matching();
    double matchAngles(string, string);
    int scanFile(string);
    void checkBend(string,string);
    void getCoordinates(string,string);

private:
    int point;
    vector<double> angles;
    vector<double> times;
    double baselineAverage;
    vector<double> rangeAngles;
    vector<double> rangeTimes;
    double startTime;
    double endTime;
    string exercise;
    vector<double> wristX;
    vector<double> wristY;
    vector<double> wristZ;
    vector<double> elbowX;
    vector<double> elbowY;
    vector<double> elbowZ;
    vector<double> shoulderX;
    vector<double> shoulderY;
    vector<double> shoulderZ;
    double average(vector<double>);
    double averageVals(vector<double>, int);
    int findMaxIndex();
    int findMin(int,int,int);

};

#endif	/* MATCHING_H */

