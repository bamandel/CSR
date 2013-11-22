/*
 * File:   Retrieval.cpp
 * Author: Chelsea
 *
 * Created on June 15, 2012, 10:19 AM
 */

#include "Retrieval.h"

Retrieval::Retrieval() {
}

Retrieval::Retrieval(const Retrieval& orig) {
}

Retrieval::~Retrieval() {
}
/*
 * Finds the best match of all candidate trees to the query tree.  All points from query's
 * 20 joint text files are compared against all points from the 20 joint text files of a library tree.
 * The sums of the computed DTW between these trees are kept track of.
 *
 */
double Retrieval::matchQuery(MotionIndexTreeNode* query,MotionIndexTreeNode * libraryTree){
    if(query != NULL){
    vector<double> libVecX;
    vector<double> libVecY;
    vector<double> libVecZ;
    vector<double> queryVecX;
    vector<double> queryVecY;
    vector<double> queryVecZ;
        ifstream infile;
        double x=0;
        double y=0;
        double z = 0;
        int count;
        infile.open (libraryTree->getPath().c_str(), ifstream::in);
        string line;
        if (infile)
        {
            string token;
            stringstream iss;
            while ( getline(infile, line) )
            {
                count = 0;
                iss << line;
                while ( getline(iss, token, ',') )
                {
                    if(count==0)
                      x=atof(token.c_str());
                    else if (count==1)
                        y=atof(token.c_str());
                    else if (count==2)
                        z=atof(token.c_str());
                    count++;
                }
                libVecX.push_back(x);
                libVecY.push_back(y);
                libVecZ.push_back(z);
                iss.clear();
            }
        }

        infile.close();
        ifstream infile2;
        infile2.open (query->getPath().c_str(), ifstream::in);
        x,y,z=0;
        if (infile2)
        {
            string token;
            stringstream iss;
            while ( getline(infile2, line) )
            {
                count = 0;
                iss << line;
                while ( getline(iss, token, ',') )
                {
                    if(count==0)
                      x=atof(token.c_str());
                    else if (count==1)
                        y=atof(token.c_str());
                    else if (count==2)
                        z=atof(token.c_str());
                    count++;
                }
                queryVecX.push_back(x);
                queryVecY.push_back(y);
                queryVecZ.push_back(z);
                iss.clear();
            }
        }
        infile2.close();
        DTW dtw(libVecX,queryVecX);
	sum += dtw.getDistance();
        DTW dtwy(queryVecY,libVecY);
        sum+= dtwy.getDistance();
        DTW dtwZ(queryVecZ,libVecZ);
        sum+= dtwZ.getDistance();
        
        queryVecX.clear();
        libVecX.clear();
        queryVecY.clear();
        libVecY.clear();
        queryVecZ.clear();
        libVecZ.clear();

        matchQuery(query->getRightSibling(),libraryTree->getRightSibling());
        matchQuery(query->getChild(),libraryTree->getChild());
    }
    
    return sum;
}
double Retrieval::getSum(){
    return sum;
}
void Retrieval::setSum(double no){
    sum = no;
}


