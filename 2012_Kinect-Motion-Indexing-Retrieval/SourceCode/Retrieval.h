/* 
 * File:   Retrieval.h
 * Author: Chelsea
 *
 * Created on June 15, 2012, 10:19 AM
 */
#include "MotionIndexTree.h"
#include "DTW.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "vectordtw.h"
#ifndef RETRIEVAL_H
#define	RETRIEVAL_H

class Retrieval {
public:
    Retrieval();
    Retrieval(const Retrieval& orig);
    virtual ~Retrieval();
    double matchQuery(MotionIndexTreeNode*, MotionIndexTreeNode *);
    void setSum(double);
    double getSum();
private:
    string pathname;
    MotionIndexTree * queryTree;
    
    double sum;
};

#endif	/* RETRIEVAL_H */

