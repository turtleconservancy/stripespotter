#include "db.h"

PhotoDatabase::PhotoDatabase() {
    db = NULL;
    num_animals = 0;
    num_photos = 0;
    stmt_insert_image = NULL;
}
PhotoDatabase::~PhotoDatabase() {
    if(db)
        sqlite3_close(db);
    if(stmt_insert_image) {
        // destroy prepared statements
        sqlite3_finalize(stmt_insert_image);
        sqlite3_finalize(stmt_insert_best_image);
        sqlite3_finalize(stmt_has_best_image);
        sqlite3_finalize(stmt_get_best_image_list);
        sqlite3_finalize(stmt_get_photo_count);
    }
}

wxString PhotoDatabase::getPhotoCountString(const char* animalID) {
    wxString result = _("DATABASE ERROR.");
    sqlite3_reset(stmt_get_photo_count);
    int ret =  sqlite3_bind_text(stmt_get_photo_count, 1, animalID, strlen(animalID), NULL);
    int photos[4] = { 0, 0, 0, 0 };
    if(ret != SQLITE_OK)
        return result;
    while(sqlite3_step(stmt_get_photo_count) == SQLITE_ROW) {
        unsigned imgType = (unsigned) sqlite3_column_int(stmt_get_photo_count, 0);
        unsigned imgCount = (unsigned) sqlite3_column_int(stmt_get_photo_count, 1);
        if(imgType < 4)
            photos[imgType] = imgCount;
    }

    result.Printf(_("%d photos: %d best, %d good, %d OK, %d bad"), photos[0]+photos[1]+photos[2]+photos[3],
            photos[0], photos[1], photos[2], photos[3]);
    return result;
}

//
// Save image metadata to CSV file
//
bool PhotoDatabase::dumpDatabase(const wxString &fname) {
    sqlite3_stmt *stmt;
    wxFile fnout((const wxChar*)fname, wxFile::write);
    if(!fnout.IsOpened())
        return false;
    if(sqlite3_prepare_v2(db, "SELECT * FROM photo_info", -1, &stmt, NULL) != SQLITE_OK) {
        fnout.Close();
        return false;
    }

    const char flanks[][6] = { "left", "right", "front", "rear" };
    const char quality[][6] = { "bad", "ok", "good", "best" };

    fnout.Write(_("#imgindex,original_filepath,roi,animal_name,sighting_id,flank,notes,photo_quality,sighting_date,sighting_time,exposure_time,focal_length,aperture_Fnumber,camera_info\n"));
    while(sqlite3_step(stmt) == SQLITE_ROW) {
        char buf[4096]; // well above sqlite table limits, so we're ok with buffers

        sprintf(buf, "%d%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s",
            sqlite3_column_int(stmt, 0), 1,
            sqlite3_column_text(stmt, 1), 1,
            sqlite3_column_text(stmt, 2), 1,
            sqlite3_column_text(stmt, 3), 1,
            sqlite3_column_text(stmt, 4), 1,
            flanks[sqlite3_column_int(stmt, 5)], 1,
            sqlite3_column_text(stmt, 6), 1,
            quality[sqlite3_column_int(stmt, 7)], 1,
            sqlite3_column_text(stmt, 8), 1,
            sqlite3_column_text(stmt, 9), 1,
            sqlite3_column_text(stmt, 10), 1,
            sqlite3_column_text(stmt, 11), 1,
            sqlite3_column_text(stmt, 12), 1,
            sqlite3_column_text(stmt, 13));

        // convert CR/LF and commas to spaces
        for(int x = 0; buf[x]; x++)
            if(buf[x] == '\n' || buf[x] == '\r' || buf[x] == ',')
                buf[x] = ' ';
            else
                if(buf[x] == 1)
                    buf[x] = ',';

        // add our terminating CRLF
        sprintf(buf+strlen(buf), "\r\n");

        // write
        fnout.Write(buf, strlen(buf));
    }

    fnout.Close();
    return true;
}

// Insert a new animal image into the DB, return the image ID, or -1 on error.
int PhotoDatabase::addPicture(const char *animalName, int imgQuality, int flank, const char *notes, const char *sighting,
                       const char *roi, const char *originalPath, const char *imgDate, const char *imgTime,
                       const char *imgExp, const char *imgFocal, const char *imgAperture, const char *camera

) {
    // Insert information into main photo database
    sqlite3_reset(stmt_insert_image);
    //imgindex,animal_name,photo_quality,flank,notes,sighting_id
    int ret  = sqlite3_bind_text(stmt_insert_image, 1,  animalName, strlen(animalName), NULL);
    ret     &= sqlite3_bind_int(stmt_insert_image,  2,  imgQuality);
    ret     &= sqlite3_bind_int(stmt_insert_image,  3,  flank);
    ret     &= sqlite3_bind_text(stmt_insert_image, 4,  notes, strlen(notes), NULL);
    ret     &= sqlite3_bind_text(stmt_insert_image, 5,  sighting, strlen(sighting), NULL);
    ret     &= sqlite3_bind_text(stmt_insert_image, 6,  roi, strlen(roi), NULL);
    ret     &= sqlite3_bind_text(stmt_insert_image, 7,  originalPath, strlen(originalPath), NULL);
    ret     &= sqlite3_bind_text(stmt_insert_image, 8,  imgDate, strlen(imgDate), NULL);
    ret     &= sqlite3_bind_text(stmt_insert_image, 9,  imgTime, strlen(imgTime), NULL);
    ret     &= sqlite3_bind_text(stmt_insert_image, 10, imgExp, strlen(imgExp), NULL);
    ret     &= sqlite3_bind_text(stmt_insert_image, 11, imgFocal, strlen(imgFocal), NULL);
    ret     &= sqlite3_bind_text(stmt_insert_image, 12, imgAperture, strlen(imgAperture), NULL);
    ret     &= sqlite3_bind_text(stmt_insert_image, 13, camera, strlen(camera), NULL);
    if(ret != SQLITE_OK) {
        return -1;
    }
    ret = sqlite3_step(stmt_insert_image);
    int imgindex = (int) sqlite3_last_insert_rowid(db);
    if(imgindex < 1)
        return -1;

    // Insert information into the best photo table
    bool hasBestImage = false;
    if(imgQuality != 3) {
        // not labeled "BEST" by the user, but is it the best we have for this animal?
        sqlite3_reset(stmt_has_best_image);
        ret =  sqlite3_bind_text(stmt_has_best_image, 1, animalName, strlen(animalName), NULL);
        if(ret != SQLITE_OK)
            return -1;
        ret = sqlite3_step(stmt_has_best_image);
        if(ret != SQLITE_ROW)
            return -1;
        hasBestImage = sqlite3_column_int(stmt_has_best_image, 0);
    }
    if(imgQuality == 3 || !hasBestImage) {
        sqlite3_reset(stmt_insert_best_image);
        ret =  sqlite3_bind_text(stmt_insert_best_image, 1, animalName, strlen(animalName), NULL);
        ret &= sqlite3_bind_int(stmt_insert_best_image, 2, imgindex);
        if(ret != SQLITE_OK)
            return -1;
        ret = sqlite3_step(stmt_insert_best_image);
        if(ret == SQLITE_ERROR)
            return -1;
    }

    return imgindex;
}

// Open the database file 'metadata.dat'
bool PhotoDatabase::open() {
    // Open or create the database file
    if(sqlite3_open("metadata.dat", &db) != SQLITE_OK)
        return false;

    // Create the photo information tables if they doesn't already exist
    int ret = sqlite3_exec(db,
            "CREATE TABLE IF NOT EXISTS photo_info "
            " ( imgindex INTEGER PRIMARY KEY,"
            "   original_filepath VARCHAR(256),"
            "   roi VARCHAR(32),"
            "   animal_name VARCHAR(64),"
            "   sighting_id VARCHAR(128),"
            "   flank TINYINT,"
            "   notes VARCHAR(4096),"
            "   photo_quality TINYINT,"
            "   sighting_date VARCHAR(11),"
            "   sighting_time VARCHAR(6),"
            "   exposure_time VARCHAR(16),"
            "   focal_length VARCHAR(8),"
            "   aperture_Fnumber VARCHAR(8),"
            "   camera_info VARCHAR(128)"
            " )",
        NULL, NULL, NULL);
    ret &= sqlite3_exec(db,
            "CREATE TABLE IF NOT EXISTS best_photo "
            "  (animal_name VARCHAR(64) PRIMARY KEY,"
            "   imgindex INTEGER)",
        NULL, NULL, NULL);
    if(ret != SQLITE_OK)
        return false;

    // Get some basic information from the DB: number of animals
    sqlite3_stmt *stmt;
    ret = sqlite3_prepare_v2(db,
            "SELECT COUNT(DISTINCT animal_name) FROM photo_info",
            -1, &stmt, NULL);
    if(ret != SQLITE_OK)
        return false;
    ret = sqlite3_step(stmt);
    if(ret == SQLITE_ROW)
        num_animals = sqlite3_column_int(stmt, 0);
    else
        return false;
    sqlite3_finalize(stmt);

    // Get more basic information: number of photographs
    ret = sqlite3_prepare_v2(db,
            "SELECT COUNT(imgindex) FROM photo_info",
            -1, &stmt, NULL);
    if(ret != SQLITE_OK)
        return false;
    ret = sqlite3_step(stmt);
    if(ret == SQLITE_ROW)
        num_photos = sqlite3_column_int(stmt, 0);
    else
        return false;
    sqlite3_finalize(stmt);

    // Create some statement that can be re-used
    ret  = sqlite3_prepare_v2(db,
            "INSERT INTO photo_info (imgindex,animal_name,photo_quality,flank,notes,sighting_id,"
                                    "roi, original_filepath, sighting_date, sighting_time, exposure_time,"
                                    "focal_length, aperture_Fnumber, camera_info)"
                                    " VALUES(NULL,?,?,?,?,?,?,?,?,?,?,?,?,?)",
            -1, &stmt_insert_image, NULL);
    ret &= sqlite3_prepare_v2(db,
            "INSERT OR REPLACE INTO best_photo (animal_name,imgindex) VALUES(?,?)",
            -1, &stmt_insert_best_image, NULL);
    ret &= sqlite3_prepare_v2(db,
            "SELECT COUNT(*) FROM best_photo WHERE animal_name = ?",
            -1, &stmt_has_best_image, NULL);
    ret &= sqlite3_prepare_v2(db,
            "SELECT animal_name,imgindex FROM best_photo",
            -1, &stmt_get_best_image_list, NULL);
    ret &= sqlite3_prepare_v2(db,
            "SELECT photo_quality,count(1) FROM photo_info WHERE animal_name = ? GROUP BY photo_quality",
            -1, &stmt_get_photo_count, NULL);
    if(ret != SQLITE_OK)
        return false;

    return true;
}

//
// Returns true if an animal with the specified name exists
// in the database
//
bool PhotoDatabase::queryAnimalName(const char* query) {
    sqlite3_reset(stmt_has_best_image);
    int ret = sqlite3_bind_text(stmt_has_best_image, 1, query, strlen(query), NULL);
    if(ret != SQLITE_OK)
        exit(1);
    ret = sqlite3_step(stmt_has_best_image);
    if(ret != SQLITE_ROW)
        exit(1);

    return (sqlite3_column_int(stmt_has_best_image, 0) == 1);
}

//
// Returns a list of <animal name, best photo filename> pairs
//
vector<pair<wxString, wxString> > PhotoDatabase::getBestPhotoList() {
    vector<pair<wxString, wxString> > result;

    // iterate over DB query results
    sqlite3_reset(stmt_get_best_image_list);
    while(sqlite3_step(stmt_get_best_image_list) == SQLITE_ROW) {
        wxString animalName = wxString::FromUTF8((const char*)sqlite3_column_text(stmt_get_best_image_list, 0));
        int imgindex = sqlite3_column_int(stmt_get_best_image_list, 1);
        wxString thumbName;
        thumbName.Printf(_("thumb-%07d.jpg"), imgindex);
        result.push_back(pair<wxString,wxString>(animalName, thumbName));
    }
    return result;
}

wxString PhotoDatabase::getError() {
    wxString result(_("Could not connect to database."));
    if(db)
        result = wxString::FromUTF8(sqlite3_errmsg(db));
    return result;
}
