/*
 * Copyright (c) 2010 Jordan Frank, HumanSense Project, McGill University
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 * See LICENSE for more information
 *
 * BuildTree.cpp
 */

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "ANNx.h"
#include "Utils.h"
#include "BuildTree.h"
#include "TDEModel.h"
#include <iostream>

void buildTree(const char* in_file, int m, int p, int d) {
	std::cout << "inside build tree" << std::endl;
	srand((unsigned)time(NULL));
	TDEModel *tdeModel;
    Settings settings = { ULONG_MAX, 0, 0xff, 1, 1, 2, 2, NULL, NULL, NULL, 0 };
    char stin=0;
    uint i, j;
    settings.infile = (char*)in_file;
    check_alloc(settings.outfile=(char*)calloc(strlen(settings.infile)+5,sizeof(char)));
    strcpy(settings.outfile,settings.infile);
    strcat(settings.outfile,".dmp");
    // test_outfile(settings.outfile);

    settings.delay = d;
    settings.embdim = m;
    settings.pcaembdim = p;

    std::cout << "before DumpTree" << std::endl;
    system("PAUSE");

    tdeModel = new TDEModel(&settings);
    tdeModel->DumpTree(settings.outfile);

    std::cout << "after DumpTree" << std::endl;
    system("PAUSE");
/*
    ANNpoint ap = tdeModel->getDataPoint(0);
    uint N = 1000;
    ANNpointArray pts = annAllocPts(N, settings.embdim);;
    tdeModel->simulateTrajectory(ap, pts, settings.embdim, N);

    // DUMP Manifold and Trajectory
    FILE* dump = fopen("/sdcard/hs/trajectory.csv","w");
    for (i = 0; i < N; i++) {
    	fprintf(dump, FLOAT_OUT, pts[i][0]);
    	for (j = 1; j < settings.embdim; j++) {
    		fprintf(dump, "\t");
    		fprintf(dump, FLOAT_OUT, pts[i][j]);
    	}
    	fprintf(dump, "\n");
    }
    fclose(dump);
    annDeallocPt(ap);
    delete [] pts;
*/
    delete tdeModel;
    annClose(); //Removing this causes a memory leak but will not compile without removing it

    if (settings.column != NULL) free(settings.column);
    if (settings.outfile != NULL) free(settings.outfile);

    std::cout << "leaving buildtree" << std::endl;
    system("pause");

    return;
}
