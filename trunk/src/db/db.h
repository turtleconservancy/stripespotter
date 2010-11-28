#ifndef DB_H_INCLUDED
#define DB_H_INCLUDED

#include <wx_pch.h>
#include "sqlite3.h"
using namespace std;

class PhotoDatabase {
    public:
        PhotoDatabase();
        ~PhotoDatabase();
        bool open();
        int addPicture(const char *animalName, int imgQuality, int flank, const char *notes, const char *sighting,
                       const char *roi, const char *originalPath, const char *imgDate, const char *imgTime,
                       const char *imgExp, const char *imgFocal, const char *imgAperture, const char *camera
        );
        int getNumAnimals() { return num_animals; };
        int getNumPhotos() { return num_photos; };
        wxString getPhotoCountString(const char* animalID);
        wxString getError();
        vector<pair<wxString, wxString> > getBestPhotoList();
        bool queryAnimalName(const char *);
        bool dumpDatabase(const wxString &fname);

    private:
        sqlite3 *db;
        int num_animals;
        int num_photos;
        sqlite3_stmt *stmt_insert_image;
        sqlite3_stmt *stmt_insert_best_image;
        sqlite3_stmt *stmt_has_best_image;
        sqlite3_stmt *stmt_get_best_image_list;
        sqlite3_stmt *stmt_get_photo_count;
};


#endif // DB_H_INCLUDED
