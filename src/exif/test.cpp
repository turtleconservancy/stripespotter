#include <stdio.h>
#include "exif.h"

int main(int argc, char *argv[]) {
	// Read the JPEG file into a buffer
	FILE *fp = fopen(argv[1], "rb");
	if(!fp) { printf("Can't open file.\n"); return -1; }
	fseek(fp, 0, SEEK_END);
	unsigned long fsize = ftell(fp);
	rewind(fp);
	unsigned char *buf = new unsigned char[fsize];
	if(fread(buf, 1, fsize, fp) != fsize) {
		printf("Can't read file.\n");
		return -2;
	}
	fclose(fp);

	// Parse exif
	EXIFInfo result;
	ParseEXIF(buf, fsize, result);

	// Dump whatever information is available
	if(result.cameraModel)
		printf("Camera model      : %s\n", result.cameraModel);
	if(result.cameraMake)
		printf("Camera make       : %s\n", result.cameraMake);
	if(result.focalLength) 
		printf("Lens focal length : %umm\n", result.focalLength);
	if(result.FStop) 
		printf("Lens F-number     : f/%g\n", result.FStop);
	if(result.exposureTime) 
		printf("Exposure          : 1/%gs\n", 1.0/result.exposureTime);
	if(result.imgDescription)
		printf("Image description : %s\n", result.imgDescription);
	if(result.dateTimeModified)
		printf("Date/time modified: %s\n", result.dateTimeModified);
	if(result.dateTimeOriginal)
		printf("Date/time original: %s\n", result.dateTimeOriginal);

	delete[] buf;
	return 0;
}
