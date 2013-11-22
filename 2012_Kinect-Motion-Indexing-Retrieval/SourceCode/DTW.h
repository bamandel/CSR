/* 
 * File:   DTW.h
 * Author: Chelsea
 *
 * Created on June 19, 2012, 9:58 AM
 *
 * Open Source code for DTW algorithm to compare point vectors of unequal
 * or equal sizes.  Translated from Java to C++.
 *
 * Source : http://www.koders.com/java/fid647E612AEB89260F5C952B17879C93F00A21CD80.aspx?s=AddOpts
 */
#include <vector>
#include <iostream>
#include <algorithm>
#ifndef DTW_H
#define	DTW_H
using namespace std;
class DTW {
public:
    DTW(vector<double>,vector<double>);
    DTW(const DTW& orig);
    virtual ~DTW();
    double getDistance();
    string toString();
protected:
    vector<double> seq1;
    vector<double> seq2;
    int n;
    int m;
    vector<vector<int> > warpingPath;
    int K;
    double warpingDistance;
    void reversePath(vector<vector<int> >);
    double distanceBetween(double, double);
    int getIndexOfMinimum(double[]);
private:
    void compute();

};

#endif	/* DTW_H */

