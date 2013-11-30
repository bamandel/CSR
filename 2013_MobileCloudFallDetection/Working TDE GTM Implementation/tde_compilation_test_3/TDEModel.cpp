/*
 * Copyright (c) 2010 Jordan Frank, HumanSense Project, McGill University
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 * See LICENSE for more information
 *
 * TDEModel.cpp
 *
 */
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "ANN.h"
#include <cxcore.h>
#include "Utils.h"
#include "BuildTree.h"
#include "TDEModel.h"
#include <iostream>
#define SIM_NEIGHBOURS 4
using namespace std;



TDEModel::TDEModel(Settings* settings) {


    ANNcoord *data, *projecteddata;

    length = settings->length;
    embdim = settings->embdim;
    delay = settings->delay;
    use_pca = (settings->embdim > settings->pcaembdim);

    get_embedding(settings, data, length);

    cout << "after get_embedding inside TDEModel" << endl;
    
    if (use_pca) {
    	computePCABases(data, length, embdim, settings->pcaembdim);
    	projecteddata = projectData(data, length, embdim);
    	delete [] data;
    	data = projecteddata;
    }
    else {
    	avg = NULL;
    	bases = NULL;
    }
    
    cout << "after if/else" << endl;
    
    dataPts = annAllocPts(length, settings->pcaembdim);
    convert_to_ann_points(dataPts, data, length, settings->pcaembdim);
    kdTree = new ANNkd_tree(dataPts,length,settings->pcaembdim); //cause of LNK error
    
    cout << "after new kdTree" << endl;
    
    settings->length = length;
    
    cout << "before delete data" << endl;
    
    delete [] data;

    cout << "after delete data inside TDE" << endl;
 }

TDEModel::TDEModel(FILE* model_file) {
	int avgsize, basesrows, basescols, i, j;
	ANNcoord *ptr;

	fscanf(model_file, "%u", &delay); // *model_file >> delay;
	fscanf(model_file, "%u", &embdim); // *model_file >> embdim;
	fscanf(model_file, "%u", &avgsize); // *model_file >> avgsize;
	//_//_android_log_print(ANDROID_LOG_DEBUG, HS_TAG, "\tDelay: %u", delay);
	//_//_android_log_print(ANDROID_LOG_DEBUG, HS_TAG, "\tEmbed dim: %u", embdim);
	//_//_android_log_print(ANDROID_LOG_DEBUG, HS_TAG, "\tReduced dim: %u", avgsize);
	if (avgsize > 0) {
		use_pca = 1;
		avg = cvCreateMat(1,avgsize,MAT_TYPE);
		ptr = (ANNcoord*)avg->data.ptr;
		for (i = 0; i < avgsize; i++) {
			fscanf(model_file, FLOAT_SCAN, &*ptr++); // *model_file >> *ptr++;
		}
	}
	else {
		use_pca = 0;
		avg = NULL;
		bases = NULL;
	}
	fscanf(model_file, "%u", &basesrows);
	fscanf(model_file, "%u", &basescols);
	// _//_android_log_print(ANDROID_LOG_DEBUG, HS_TAG, "\tBases Rows: %u", basesrows);
	// _//_android_log_print(ANDROID_LOG_DEBUG, HS_TAG, "\tBases Cols: %u", basescols);

	if (use_pca) {
		bases = cvCreateMat(basesrows, basescols, MAT_TYPE);
		ANNcoord* ptr = (ANNcoord*)bases->data.ptr;
		for (i = 0; i < basesrows; i++) {
			for (j = 0; j < basescols; j++) {
				fscanf(model_file, FLOAT_SCAN, &*ptr++); // *model_file >> *ptr++;
			}
		}
	}
	//_//_android_log_write(ANDROID_LOG_DEBUG, HS_TAG, "\tDone Loading Data.");

	kdTree = new ANNkd_tree(model_file); //cause of LNK error
	dataPts = kdTree->thePoints();
	length = kdTree->nPoints();
}

TDEModel::~TDEModel() {
	if (avg != NULL) cvReleaseMat(&avg);
	if (bases != NULL) cvReleaseMat(&bases);
	delete kdTree;
	annDeallocPts(dataPts);
}

void TDEModel::DumpTree(char* outfile) {

	FILE *fout = fopen(outfile, "w");

	cout << " outfile inside TDEModel = " << outfile << endl;

	fprintf(fout, "%u\n", delay); // fout << delay << endl;
	fprintf(fout, "%u\n", embdim); // fout << embdim << endl;
    if (avg == NULL) {
    	fprintf(fout, "0\n"); // fout << 0 << endl;
    }
    else {
    	fprintf(fout, "%d\n", avg->cols); // fout << avg->cols << endl;
		fprintf(fout, FLOAT_OUT, CV_MAT_ELEM(*avg, ANNcoord, 0, 0)); // fout << CV_MAT_ELEM(*avg, double, 0, 0);
    	for (int i = 1; i < avg->cols; i++) {
    		fprintf(fout, " " FLOAT_OUT, CV_MAT_ELEM(*avg, ANNcoord, 0, i)); // fout << " " << CV_MAT_ELEM(*avg, double, 0, i);
    	}
    }
    fprintf(fout, "\n"); // fout << endl;
    if (bases == NULL) {
    	fprintf(fout, "0 0\n"); // fout << "0 0";
    }
    else {
    	fprintf(fout, "%d %d\n", bases->rows, bases->cols); // fout << bases->rows << " " << bases->cols << endl;
    	for (int i = 0; i < bases->rows; i++) {
    		fprintf(fout, FLOAT_OUT, CV_MAT_ELEM(*bases, ANNcoord, i, 0)); // fout << CV_MAT_ELEM(*bases, double, i, 0);
    		for (int j = 1; j < bases->cols; j++) {
        		fprintf(fout, " " FLOAT_OUT, CV_MAT_ELEM(*bases, ANNcoord, i, j)); // fout << CV_MAT_ELEM(*bases, double, i, 0);
    		}
    		fprintf(fout, "\n"); // fout << endl;
    	}
    }
    
    cout << "before close and Dump inside TDEModel" << endl;
    
    kdTree->Dump(ANNtrue, fout);
    
    cout << "after tree Dump inside TDEModel" << endl;
    
    fclose(fout); // fout.close();
    
    cout << "after dump and close inside TDEModel" << endl;
}

void TDEModel::getKNN(ANNpoint ap, int k, ANNidxArray nn_idx, ANNdistArray dists) {

	//cout << "inside getKNN before annk search" << endl;

	kdTree->annkSearch(ap, k, nn_idx, dists);
//    for (uint i = 0; i < k; i++) {
//            cout << "Point " << i+1 << ": [" << dataPts[nn_idx[i]][0] << "," << dataPts[nn_idx[i]][1] << "," << dataPts[nn_idx[i]][2] << "], Dist: " << sqrt(dists[i]) << endl;
//    }
}
/*
void TDEModel::simulateTrajectory(ANNpoint s0, ANNpointArray trajectory, uint dim, ulong N) {
    ANNidxArray nn_idx;
    ANNdistArray dists;
    ulong i;
    uint j, k;
    MTRand r;

	// +1 in case one of the neighbours is the last point in the model.
    nn_idx = new ANNidx[SIM_NEIGHBOURS+1];
    dists = new ANNdist[SIM_NEIGHBOURS+1];

    for (i = 0; i < dim; i++) {
    	trajectory[0][i] = s0[i];
    }

    for (i = 1; i < N; i++) {
    	getKNN(trajectory[i-1], SIM_NEIGHBOURS+1, nn_idx, dists);
    	for (j = 0; j < dim; j++) {
    		trajectory[i][j] = 0.0;
    		for (k = 0; k < SIM_NEIGHBOURS; k++) {
    			if (nn_idx[k] == ANN_NULL_IDX) break;
    			else if (nn_idx[k] == (int)length-1) nn_idx[k] = nn_idx[SIM_NEIGHBOURS];
    			trajectory[i][j] += dataPts[nn_idx[k]+1][j];
    		}
    		trajectory[i][j] = trajectory[i][j] / (ANNcoord)k + r.randNorm(0.0, 0.1);
    	}
    }
}
*/
ANNpoint TDEModel::getDataPoint(int idx) {
	if (idx > length) {
	
	}
	return dataPts[idx];
}

void TDEModel::computePCABases(ANNcoord *data, int rows, int cols, int numbases) {
	CvMat **embedding, *cov, *eigenvectors, *eigenvalues, *vector;
	ANNcoord *basesdata;
	uint i, j, offset;

	cov = cvCreateMat(cols, cols, MAT_TYPE);
	eigenvectors = cvCreateMat(cols,cols,MAT_TYPE);
	eigenvalues = cvCreateMat(cols,cols,MAT_TYPE);
	embedding = new CvMat*[rows];
	for (i = 0; i < rows; i++) {
		vector = cvCreateMatHeader(1, cols, MAT_TYPE);
		cvInitMatHeader(vector, 1, cols, MAT_TYPE, data + i * cols);
		embedding[i] = vector;
	}
	avg = cvCreateMat(1,cols,MAT_TYPE);
	cvCalcCovarMatrix((const CvArr **)embedding, rows, cov, avg, CV_COVAR_NORMAL);
	cvSVD(cov, eigenvalues, eigenvectors, 0, CV_SVD_MODIFY_A);

	basesdata = new ANNcoord[cols*numbases];
	for (i = 0, offset = 0; i < cols; i++) {
		for (j = 0; j < numbases; j++) {
			basesdata[offset++] = ((ANNcoord*)eigenvectors->data.ptr)[i*cols+j];
		}
	}
	bases = cvCreateMatHeader(cols, numbases, MAT_TYPE);
	cvInitMatHeader(bases, cols, numbases, MAT_TYPE, basesdata);

	for (i = 0; i < rows; i++) {
    	cvReleaseMat(&embedding[i]);
    }
    delete [] embedding;
    cvReleaseMat(&cov);
    cvReleaseMat(&eigenvectors);
    cvReleaseMat(&eigenvalues);
}

ANNcoord* TDEModel::projectData(ANNcoord* data, int rows, int cols) {
	if (!use_pca) return data;
	ANNcoord* shifteddata = new ANNcoord[rows*cols];
	ANNcoord* projecteddata = new ANNcoord[rows*bases->cols];
	uint i, j, offset;

	for (i = 0, offset = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			shifteddata[offset] = data[offset] - ((ANNcoord*)avg->data.ptr)[j];
			offset++;
		}
	}
	CvMat projected = cvMat(rows, bases->cols, MAT_TYPE, projecteddata);
	CvMat dataMat = cvMat(rows, cols, MAT_TYPE, shifteddata);
	cvGEMM(&dataMat, bases, 1.0, NULL, 0.0, &projected, 0);
	/*
	for (i = 0; i < (unsigned)bases->cols; i++) {
		cout << projecteddata[i] << " " ;
	}
	cout << endl;
	*/
	delete [] shifteddata;
	return projecteddata;
}
