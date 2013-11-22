/*
 * File:   CandidateSearch.cpp
 * Author: Chelsea
 *
 * Created on June 12, 2012, 2:24 PM
 */
#include "CandidateSearch.h"
#include <windows.h>
using namespace std;
/*
 * Computes indexing, candidate search, and retrieval using DTW
 *
 * param - name : represents patient's name
 */
CandidateSearch::CandidateSearch(string path) {
    count = 0;
    threshold = 3; //collection must appear in 3 or more index maps to be a candidate

    vector<string>filenames = findFileNames(path);
    for(int i=2;i<filenames.size();i++){ //for all patients in data folder:
        string pathname = path + "/" + filenames.at(i);
        vector<string>patientExercises = findFileNames(pathname);
        for(int j=2;j<patientExercises.size();j++){ //for all exercises this patient has performed
            exerciseName = patientExercises.at(j);
            string queryPath = pathname + "/" + exerciseName + "/";
            Matching match(exerciseName);
            int point;
            if(exerciseName=="rightArm" || exerciseName=="leftArm" )
                point = match.scanFile(queryPath + "angle.txt");

            else
                point = match.scanFile(queryPath + "legAngle.txt");
            if(point == -1){
                cout<<"For patient "<<filenames.at(i)<<", performing exercise "<<exerciseName<<", ";
                cout<<"data did not record for a full 10 seconds. Could not assign a point value for exercise"<<endl;
            }
            else{
                cout<<"Patient "<<filenames.at(i)<<", performing exercise "<<exerciseName<<", ";
                cout<<"scored a "<<point<<"!"<<endl;
            }
        }
        cout<<endl;  //newline for new patient
    }
}

CandidateSearch::CandidateSearch(const CandidateSearch& orig) {
}

CandidateSearch::~CandidateSearch() {
}
/*
 * Searches through patient's folder to retrieve and process
 * all exercises this patient has performed
 *
 * param - name : represents patient's name
 */
vector<string> CandidateSearch::findFileNames(string name){
    string path = name;
    vector<string>filenames;
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    char DirSpec[MAX_PATH]; // directory specification
    strcpy (DirSpec, path.c_str());
    strcat (DirSpec, "\\*");
    hFind = FindFirstFile(DirSpec, &FindFileData);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do {   filenames.push_back(std::string(FindFileData.cFileName));

        } while( FindNextFile(hFind, &FindFileData) != 0);
        FindClose(hFind);

        return filenames;
    }
}

