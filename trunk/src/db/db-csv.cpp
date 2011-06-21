#include <wx/wxprec.h>
#include "db-csv.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


PhotoDatabase::PhotoDatabase() {
	userCSVHeader = "#imgindex,original_filepath,roi,animal_name,sighting_id,flank,notes,photo_quality,sighting_date,sighting_time,exposure_time,focal_length,aperture_Fnumber,camera_info,sex,age,sighting_location,group_size,gps_lat,gps_lon,reproductive_status\n";
}
PhotoDatabase::~PhotoDatabase() {
	for(map<int,PhotoInfo*>::iterator it=photo_to_info.begin(); it!=photo_to_info.end(); it++)
		delete it->second;
	photo_to_info.clear();
	animal_to_photos.clear();
}

//
// Parse a comma-separated line into a PhotoInfo structure
//
bool PhotoInfo::parse(char *buf, PhotoInfo &inf) {
	if(buf[0]=='#')
		return false;
	char *f_start = buf;
	char *line_end = buf+strlen(buf);	// points to the terminating \0 byte
	int field = 0;
	string tmp;
	inf.other_info.clear();

	do {
		char *f_end = strchr(f_start, ',');
		if(!f_end)
			f_end = line_end;
		*f_end = 0;

		// the current field is a null-terminated string
		// pointed to by f_start
		switch(field) {
			case 0:
				// photo number
				inf.photo_id = atoi(f_start);
				if(inf.photo_id < 1)
					return false;
				break;
			case 1:
				// original filename
				inf.original_filename = f_start;
				break;
			case 2:
				// region of interest
				inf.roi = f_start;
				break;
			case 3:
				// animal name
				inf.animal_name = f_start;
				break;
			case 4:
				// sighting ID
				inf.sighting_id = f_start;
				break;
			case 5:
				// flank
				tmp = f_start;
				if(tmp == "left")
					inf.flank = FL_LEFT;
				else {
					if(tmp == "right")
						inf.flank = FL_RIGHT;
					else {
						if(tmp == "rear")
							inf.flank = FL_REAR;
						else {
							if(tmp == "front")
								inf.flank = FL_FRONT;
							else
								return false;
						}
					}
				}
				break;
			case 6:
				// notes
				inf.notes = f_start;
				break;
			case 7:
				// photo quality
				tmp = f_start;
				if(tmp == "good")
					inf.quality = PQ_GOOD;
				else {
					if(tmp == "ok")
						inf.quality = PQ_OK;
					else {
						if(tmp == "best")
							inf.quality = PQ_BEST;
						else {
							if(tmp == "bad")
								inf.quality = PQ_BAD;
							else
								return false;
						}
					}
				}
				break;
			case 8:
				// sighting date
				inf.date = f_start;
				break;
			case 9:
				// sighting time
				inf.time = f_start;
				break;
			case 10:
				// image exposure
				inf.photo_exposure = f_start;
				break;
			case 11:
				// focal length
				inf.focal_length = f_start;
				break;
			case 12:
				// aperture
				inf.aperture = f_start;
				break;
			case 13:
				// camera info
				inf.camera_info = f_start;
				break;
            case 14:
                // sex
				tmp = f_start;
				if(tmp == "female")
					inf.sex = FEMALE;
				else {
					if(tmp == "male")
						inf.sex = MALE;
					else {
						if(tmp == "unknown")
							inf.sex = UNKNOWN;
                        else
                            return false;
					}
				}
                break;
            case 15:
                // age
                inf.age = f_start;
                break;
            case 16:
                // sighting location
                inf.sighting_location = f_start;
                break;
            case 17:
                // group size
                inf.group_size = atoi(f_start);
                break;
            case 18:
                // gps lat
                inf.gps_lat = f_start;
                break;
            case 19:
                // gps lon
                inf.gps_lon = f_start;
                break;
            case 20:
                // reproductive status
                inf.reproductive = f_start;
                break;
			default:
				inf.other_info.push_back(f_start);
				break;
		}
		field++;
		f_start = f_end+1;
	} while(f_start < line_end);
	return true;
}


//
// Open the CSV database file 'data/SightingData.csv'
//
bool PhotoDatabase::open(wxProgressDialog *wxPD) {
	// Open or create the photo database
	FILE *fp, *check;
	if(!(fp = fopen("SightingData.csv", "r")))
		if(!(fp = fopen("SightingData.csv", "w+")))
			return false;

	// Read and parse database, ensuring that all the associated image files exist
	char buf[4096]; int line = 0;
	while(fgets(buf, 4096, fp)) {
		// if this is line #1 of the file, save it for when we re-write the
		// database, in case the user has added extra CSV fields.
		if(++line == 1) {
			userCSVHeader = buf;
			continue;
		}

	    if(wxPD)
            wxPD->Pulse();
		PhotoInfo pi; line++;
		if(PhotoInfo::parse(buf, pi)) {
			// Ensure that the image files exist
#ifdef WIN32
			sprintf(buf, "images\\img-%07d.jpg", pi.photo_id);
#else
            sprintf(buf, "images/img-%07d.jpg", pi.photo_id);
#endif
			if(!(check = fopen(buf, "r"))) {
				wxString errMsg;
#ifdef WIN32
				errMsg.Printf(_("Cannot find image file images\\img-%07d.jpg"), pi.photo_id);
#else
				errMsg.Printf(_("Cannot find image file images/img-%07d.jpg"), pi.photo_id);
#endif
				wxMessageBox(errMsg, _("Picture file missing"));
			} else {
				fclose(check);

				// Save in memory data structures
				PhotoInfo *mem_pi = new PhotoInfo;
				*mem_pi = pi;
				photo_to_info[pi.photo_id] = mem_pi;
				animal_to_photos[pi.animal_name].push_back(mem_pi);
				if(pi.sighting_id.size())
					sighting_to_photo[pi.sighting_id] = mem_pi;
			}
		}
	}
	fclose(fp);

	if(!readStripeCodes())
		wxMessageBox(_("Cannot find data\\StripeCodes.txt -- animal identification will not work"), _("Error"));
	else {
		// consistency check of StripeCodes
		// if we have an image loaded and verified, we should have a StripeCode
		// for that image.
		int nImagesWithoutStripeCodes = 0;
		for(map<int,PhotoInfo*>::iterator it=photo_to_info.begin();it!=photo_to_info.end();it++)
			if(stripeCodes.find(it->first)==stripeCodes.end())
				nImagesWithoutStripeCodes++;
		if(nImagesWithoutStripeCodes) {
			wxString errMsg;
			errMsg.Printf(_("%d out of %d photos did not have automatic search information. Those photos will be excluded from automatic identification."), nImagesWithoutStripeCodes, (int)photo_to_info.size());
			wxMessageBox(errMsg, _("Some missing information."));
		}
	}

    return true;
}


//
// Save image metadata to CSV file
//
bool PhotoDatabase::dumpDatabase() {
	FILE *fp;
	if(!(fp = fopen("SightingData.csv", "w+")))
		return false;
	fprintf(fp, "%s", userCSVHeader.c_str());
//	fprintf(fp, "#imgindex,original_filepath,roi,animal_name,sighting_id,flank,notes,photo_quality,sighting_date,sighting_time,exposure_time,focal_length,aperture_Fnumber,camera_info,sex,age,sighting_location,group_size,gps_lat,gps_lon,reproductive_status\n");
	for(map<int,PhotoInfo*>::iterator it=photo_to_info.begin();it!=photo_to_info.end();it++)
		fprintf(fp, "%s\n", it->second->toString().c_str());
	fclose(fp);

	return writeStripeCodes();
}

// Insert a new animal image into the DB, return the image ID, or -1 on error.
int PhotoDatabase::addPicture(PhotoInfo *pi) {
	// Figure out a photo ID to assign
	pi->photo_id	= 1;
	if(photo_to_info.size())
		pi->photo_id = (photo_to_info.rbegin()->first) + 1;
	while(photo_to_info.find(pi->photo_id) != photo_to_info.end())
		pi->photo_id++;

	// Add to data structures
	photo_to_info[pi->photo_id] = pi;
	animal_to_photos[pi->animal_name].push_back(pi);
	if(!pi->sighting_id.empty())
		sighting_to_photo[pi->sighting_id] = pi;

	return pi->photo_id;
}


#include <iostream>
bool PhotoDatabase::writeStripeCodes() {

    // begin chayant
	for(map<int,StripeCode>::iterator it=stripeCodes.begin();it!=stripeCodes.end();it++) {
        printf("photoid %d\n", it->first);
    }
    printf("END\n\n");

    for (map<int, PhotoInfo*>::iterator it=photo_to_info.begin();it!=photo_to_info.end();it++) {
        printf("photo id %d name %s\n", it->first, it->second->animal_name.c_str());
    }
    printf("END\n\n");
    return false;
    // end chayant

	FILE *fp;
	if(!(fp = fopen("StripeCodes.txt", "w+")))
		return false;

	for(map<int,StripeCode>::iterator it=stripeCodes.begin();it!=stripeCodes.end();it++) {
		fprintf(fp, "ANIMAL %s %d\n", photo_to_info.at(it->first)->animal_name.c_str(), it->first);
		fprintf(fp, "%s\n", it->second.toString().c_str());
	}

	fclose(fp);

	// write multi-resolution histograms
	if(!(fp = fopen("MRHistograms.txt", "w+")))
		return false;
	for(map<int,MultiScaleHistogram>::iterator it=MRHistograms.begin();it!=MRHistograms.end();it++) {
		fprintf(fp, "ANIMAL %d\n", it->first);
		fprintf(fp, "%s\n", it->second.toString().c_str());
	}
	fclose(fp);
	return true;
}

//
// Read StripeCodes and MRHistograms
//
bool PhotoDatabase::readStripeCodes() {
	FILE *fp;
	if(!(fp = fopen("StripeCodes.txt", "r")))
		if(!(fp = fopen("StripeCodes.txt", "w+")))
			return false;

	stripeCodes.clear();
	char buf[4096];
	while(fgets(buf, 4096, fp))
		if(buf[0]=='A' && buf[1]=='N' && buf[2]=='I') {
			char *c = buf+strlen(buf);
			while(c > buf && *c != 'I' && *(--c) != ' ' );
			int anum = atoi(c);
			if(anum >= 0) {
				StripeCode sc;
				if(sc.read(fp))
					stripeCodes[anum] = sc;
				else {
					wxString status;
					status.Printf(_("Error reading StripeCode for photo %d"), anum);
					wxMessageBox(status, _("Error"));
				}
			}
		}
	fclose(fp);

	// Read multi-resolution histograms
	if(!(fp = fopen("MRHistograms.txt", "r")))
		if(!(fp = fopen("MRHistograms.txt", "w+")))
			return false;
	MRHistograms.clear();
	while(fgets(buf, 4096, fp))
		if(buf[0]=='A' && buf[1]=='N' && buf[2]=='I') {
			char *c = buf+strlen(buf);
			while(c > buf && *c != 'I' && *(--c) != ' ' );
			int anum = atoi(c);
			if(anum >= 0) {
				MultiScaleHistogram mrh;
				if(mrh.read(fp))
					MRHistograms[anum] = mrh;
				else {
					wxString status;
					status.Printf(_("Error reading multi-resolution histogram for photo %d"), anum);
					wxMessageBox(status, _("Error"));
				}
			}
		}
	fclose(fp);

	return true;
}

//
// Returns true if an animal with the specified name exists
// in the database
//
bool PhotoDatabase::queryAnimalName(const char* query) {
	string q(query);
	return (animal_to_photos.find(q)!=animal_to_photos.end());
}
