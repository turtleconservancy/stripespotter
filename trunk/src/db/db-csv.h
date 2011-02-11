#ifndef __DB_H
#define __DB_H
#include <StripeCode.h>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <wx/progdlg.h>
using namespace std;

enum FLANK {
    FL_LEFT = 0,
    FL_RIGHT,
    FL_REAR,
    FL_FRONT
};

enum QUALITY {
    PQ_BAD = 0,
    PQ_OK,
    PQ_GOOD,
    PQ_BEST
};

enum SEX {
    MALE    = 0,
    FEMALE,
    UNKNOWN,
    YESPLEASE       // harhar
};

typedef struct _PhotoInfo {
    int photo_id;
    string original_filename;
    string roi;
    string animal_name;
    string sighting_id;
    FLANK flank;
    QUALITY quality;
    string notes;
    string date;
    string time;
    string photo_exposure;
    string focal_length;
    string aperture;
    string camera_info;
    string age;
    SEX sex;
    string reproductive;
    string gps_lat;
    string gps_lon;
    string sighting_location;
    int group_size;

    _PhotoInfo() {
        sex = UNKNOWN;
        flank = FL_LEFT;
        quality = PQ_OK;
        group_size = 0;
        photo_id = 0;
    }

    string toString() {
        stringstream ss;
        string qualities[] = { "bad", "ok", "good", "best" };
        string flanks[] = { "left", "right", "rear", "front" };
        string sexes[]  = { "male", "female", "unknown" };

        ss  << photo_id << (char)1
            << original_filename << (char)1
            << roi << (char)1
            << animal_name << (char)1
            << sighting_id << (char)1
            << flanks[flank] << (char)1
            << notes << (char)1
            << qualities[quality] << (char)1
            << date << (char)1
            << time << (char)1
            << photo_exposure << (char)1
            << focal_length << (char)1
            << aperture << (char)1
            << camera_info << (char)1
            << sexes[sex] << (char)1
            << age << (char)1
            << sighting_location << (char)1
            << group_size << (char)1
            << gps_lat << (char)1
            << gps_lon << (char)1
            << reproductive ;

        string st = ss.str();
        for(unsigned i = 0; i < st.size(); i++)
            if(st[i] == (char)1)
                st[i] = ',';
            else
                if(st[i]==',' || st[i]=='\'' || st[i]=='\r' ||
                   st[i]=='\n'|| st[i]=='"')
                    st[i] = ' ';
        return st;
    }
    static bool parse(char *buf, _PhotoInfo &inf);
} PhotoInfo;

class PhotoDatabase {
    public:
        PhotoDatabase();
        ~PhotoDatabase();
        bool open(wxProgressDialog *wxPD = NULL);
        bool dumpDatabase();
        int addPicture(PhotoInfo *pi);
        bool writeStripeCodes();
        bool readStripeCodes();
        bool queryAnimalName(const char* query);
        int getNumPhotos() { return (int) photo_to_info.size(); }
        int getNumAnimals() { return (int) animal_to_photos.size(); }

        map<int, PhotoInfo*> photo_to_info;
        map<string, vector<PhotoInfo*> > animal_to_photos;
        map<string,PhotoInfo*> sighting_to_photo;

        map<int,StripeCode> stripeCodes;
        map<int,MultiScaleHistogram> MRHistograms;


};

#endif
