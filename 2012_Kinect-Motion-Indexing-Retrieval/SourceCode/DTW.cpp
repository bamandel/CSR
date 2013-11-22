/* 
 * File:   DTW.cpp
 * Author: Chelsea
 * 
 * Created on June 19, 2012, 9:58 AM
 *
 * Open Source code for DTW algorithm to compare point vectors of unequal
 * or equal sizes.  Translated from Java to C++.
 *
 * Source : http://www.koders.com/java/fid647E612AEB89260F5C952B17879C93F00A21CD80.aspx?s=AddOpts
 */

#include <sstream>

#include "DTW.h"

DTW::DTW(vector<double> sample,vector<double> templete) {
    seq1 = sample;
    seq2 = templete;

    n = seq1.size();
    m = seq2.size();
    K = 1;
    warpingPath.resize(n+m);
    for (int i = 0; i < n+m; ++i)
        warpingPath[i].resize(2);
    
    warpingDistance = 0.0;
    compute();
}

DTW::DTW(const DTW& orig) {
}

DTW::~DTW() {
}
void DTW::compute(){
    double accumulatedDistance = 0.0;
    vector<vector<double> > d;
    vector<vector<double> > D;
    d.resize(n);
    D.resize(n);
    for (int i = 0; i < n; ++i){
        d[i].resize(m);
        D[i].resize(m);
    }


    for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                    d[i][j] = distanceBetween(seq1[i], seq2[j]);
            }
    }
    D[0][0] = d[0][0];

    for (int i = 1; i < n; i++) {
            D[i][0] = d[i][0] + D[i - 1][0];
    }

    for (int j = 1; j < m; j++) {
            D[0][j] = d[0][j] + D[0][j - 1];
    }

    for (int i = 1; i < n; i++) {
            for (int j = 1; j < m; j++) {
                    accumulatedDistance = min(min(D[i-1][j], D[i-1][j-1]), D[i][j-1]);
                    accumulatedDistance += d[i][j];
                    D[i][j] = accumulatedDistance;
            }
    }
    accumulatedDistance = D[n - 1][m - 1];

    int i = n - 1;
    int j = m - 1;
    int minIndex = 1;

    warpingPath[K - 1][0] = i;
    warpingPath[K - 1][1] = j;

    while ((i + j) != 0) {
            if (i == 0) {
                    j -= 1;
            } else if (j == 0) {
                    i -= 1;
            } else {	// i != 0 && j != 0
                    double array []= { D[i - 1][j], D[i][j - 1], D[i - 1][j - 1] };
                    minIndex = getIndexOfMinimum(array);

                    if (minIndex == 0) {
                            i -= 1;
                    } else if (minIndex == 1) {
                            j -= 1;
                    } else if (minIndex == 2) {
                            i -= 1;
                            j -= 1;
                    }
            } // end else
            K++;
            warpingPath[K - 1][0] = i;
            warpingPath[K - 1][1] = j;
    } // end while
    warpingDistance = accumulatedDistance / K;
    reversePath(warpingPath);
}
/**
	 * Changes the order of the warping path (increasing order)
	 * 
	 * @param path	the warping path in reverse order
	 */
void DTW::reversePath(vector<vector<int> > path){
    vector<vector<int> > newPath;
    newPath.resize(K);
    for (int i = 0; i < K; ++i){
        newPath[i].resize(2);
    }

    for (int i = 0; i < K; i++) {
            for (int j = 0; j < 2; j++) {
                    newPath[i][j] = path[K - i - 1][j];
            }
    }
    warpingPath = newPath;
}

double DTW::getDistance(){
    return warpingDistance;
}

double DTW::distanceBetween(double p1, double p2){
    return (p1 - p2) * (p1 - p2);
}

int DTW::getIndexOfMinimum(double array[]){
   int index = 0;
    double val = array[0];
    for (int i = 1; i < 3; i++) {
            if (array[i] < val) {
                    val = array[i];
                    index = i;
            }
    }
    return index;
}

string DTW::toString(){
    stringstream ss;//create a stringstream
    ss << warpingDistance;//add number to the stream
    string retVal = "Warping Distance: " + ss.str() + "\n";
		retVal += "Warping Path: {";
		for (int i = 0; i < K; i++) {
                    ss.clear();
                    ss<<warpingPath[i][0] << ", "+warpingPath[i][1];
			retVal += "(" + ss.str() + ")";
			retVal += (i == K - 1) ? "}" : ", ";

		}
		return retVal;
}
