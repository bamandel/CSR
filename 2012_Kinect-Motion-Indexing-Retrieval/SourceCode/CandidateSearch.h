/* 
 * File:   CandidateSearch.h
 * Author: Chelsea
 *
 * Created on June 12, 2012, 2:24 PM
 */
#include "MotionIndexTree.h"
#include "Retrieval.h"
#include "Matching.h"
#include <iostream>
#include <vector>
#include <map>
#include <stdio.h>
#include <malloc.h>
#ifndef CANDIDATESEARCH_H
#define	CANDIDATESEARCH_H

using namespace std;
class CandidateSearch {

public:
    CandidateSearch(string);
    CandidateSearch(const CandidateSearch& orig);
    virtual ~CandidateSearch();
    vector<string>findFileNames(string);
    void searchLibrary(string);
    void traverseTrees(MotionIndexTreeNode *);
    void makeMaps(MotionIndexTreeNode *);
    void findCandidates();
    string browseFolder();
    vector<MotionIndexTree *>libraryTrees;
    MotionIndexTree * queryTree;
private:
    string exerciseName;
    vector<double> sums;
    string pathname;
    int count;
    void pickExercise(string, MotionIndexTree *);
    int threshold;
    int findMin();
};

#endif	/* CANDIDATESEARCH_H */

