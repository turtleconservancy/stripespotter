/**********************************************************************
 * Various feature extraction techniques for a set of images created
 * using the StripeSpotter GUI. If images *not* created with the GUI
 * are to be used, you must create a .csv file and directory structure
 * similar to the GUI.
 *
 * Input:  data directory created by StripeSpotter GUI
 * Output: plain-text dataset file
 *
 * Currently implemented methods:
 *
 * -- StripeCodes
 * -- Multi-scale histograms of features, as described in
 *    [S.Ravela and L.R. Gamble, 2004]. Scales start from the original
 *    image size and decrease in one-octave steps.
 *
 * USAGE:
 *
 * DataCompiler DATA_DIRECTORY > stripecodes.txt
 *
 **********************************************************************/
#include "pch.h"
#include "StripeCode.h"
using namespace std;

map<string, vector<int> > animal_to_photos;

int main(int argc, char *argv[]) {
	if(argc < 2)
		return printf("Usage: compiler <StripeSpotter data directory>\nOutput: StripeCodes.txt and MRHistograms.txt\n");
	if(chdir(argv[1]))
		return printf("Cannot access directory '%s'\n", argv[1]);
	wxInitAllImageHandlers();
	fprintf(stderr, "StripeCode dataset compiler. Copyright (c) 2010 Mayank Lahiri (mlahiri@gmail.com).\n\n");

	// read and parse metadata file
	FILE *fp = fopen("SightingData.csv", "r");
	if(!fp)
		return printf("Cannot open 'SightingData.csv'\n");
	char buf[4096];
	while(fgets(buf, 4096, fp)) {
		if(buf[0]=='#')
            continue;
		int photoID = atoi(buf);
		if(photoID > 0) {
			char *p = buf;
			for(int i = 0; i < 3 && *p; p=strchr(p,','), p++, i++);
			if(*p) {
				// a neat destructive tokenizer
				char *r = p;
				for(char *q = p; *q && (*q!=',' || (*q=0) ); q++, r++);
				for(char *q = ++r; *q && (*q!=',' || (*q=0) ); q++, r++);
				for(char *q = ++r; *q && (*q!=',' || (*q=0) ); q++);
				string animal(p);
				animal += r; // append the flank to the animal name

				// record different flanks as different animals
				animal_to_photos[animal].push_back(photoID);
			}
		}
	}
	fclose(fp);

	// create output file
	FILE *fpStripeCode = fopen("StripeCodes.txt", "w+");
	FILE *fpMRHistograms = fopen("MRHistograms.txt", "w+");
	if(!fpStripeCode || !fpMRHistograms) {
        printf("Cannot create output files!\n");
        return 1;
	}

	// read and StripeCode all images
	double runAvgTimeSC = 0, numTime=0, runAvgTimeMRH = 0;
	for(map<string, vector<int> >::iterator a = animal_to_photos.begin();
											a != animal_to_photos.end(); a++)
		for(vector<int>::iterator photo = a->second.begin(); photo != a->second.end(); photo++) {
			wxString fname;
			fname.Printf(_("images/roi-%07d.jpg"), *photo);
			wxImage img(fname);
			if(!img.IsOk())
				return fprintf(stderr, "CANNOT load images/roi-%07d.jpg\n", *photo);
			fprintf(stderr, "Loaded 'images/roi-%07d.jpg' animal '%s'\n", *photo, a->first.c_str());

			// TODO: random crop support for experiments

			// read StripeCode features from image
			StripeCode featuresSC;
			startClocks();
			if(!featuresSC.read(&img))
                return fprintf(stderr, "Error: cannot extract features from 'images/roi-%07d.jpg' for animal '%s'\n", *photo, a->first.c_str());
            runAvgTimeSC += stopClocks();

            // read multi-scale histograms from image
            MultiScaleHistogram featuresMRH;
			startClocks();
			if(!featuresMRH.read(&img))
                return fprintf(stderr, "Error: cannot extract features from 'images/roi-%07d.jpg' for animal '%s'\n", *photo, a->first.c_str());
            runAvgTimeMRH += stopClocks();

            // done
            numTime++;
            fprintf(stderr, "Extraction time (running average): STRIPECODE %f sec MRHISTOGRAMS %f sec\n", runAvgTimeSC/numTime, runAvgTimeMRH/numTime);
            fprintf(fpStripeCode, "ANIMAL %s %d\n%s\n", a->first.c_str(), *photo, featuresSC.toString().c_str());
            fprintf(fpMRHistograms, "ANIMAL %s %d\n%s\n", a->first.c_str(), *photo, featuresMRH.toString().c_str());
		}

    fclose(fpStripeCode);
    fclose(fpMRHistograms);

	return 0;
}
