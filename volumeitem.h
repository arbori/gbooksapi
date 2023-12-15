#include <string>
#include <functional>
#include <sqlite3.h> 

#include <iostream>

#ifndef __VOLUMEITEM_H__
#define __VOLUMEITEM_H__

class VolumeItem {
public:
    std::string id;
    std::string title;
    std::string smallThumbnail;
    std::string thumbnail;
    bool favorite;

    VolumeItem() {}

    VolumeItem(std::string id, std::string title, std::string smallThumbnail, std::string thumbnail, bool favorite) :
        id(id), title(title), smallThumbnail(smallThumbnail), thumbnail(thumbnail), favorite(favorite)
    {
    }
};

static int callback_insert(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;

   for(i = 0; i<argc; i++) {
      std::cout << azColName[i] << " = " << argv[i] ? argv[i] : "NULL\n";
   }

   std::cout << std::endl;
   
   return 0;
}

static int callback_select(void *count, int argc, char **argv, char **azColName){
    int i;
    char* output;

    if(argc == 1 && (argv[0])) {
        *((int*) count) = (int) strtol(argv[0], &output, 10);
    }

    return 0;
}

class FavoriteRepository {
private:
    long objectid; 

    sqlite3* db;
    std::string data_file;
    bool is_open = false;

    int exit_status = 0;

public:
    FavoriteRepository(std::string &data_file) {
        std::cout << "Starting FavoriteRepository with data file " << data_file << "...\n";

        this->objectid = reinterpret_cast<long>(this);

        this->data_file.assign(data_file);

        this->exit_status = sqlite3_open(this->data_file.c_str(), &db);

        is_open = (exit_status == EXIT_SUCCESS);

        if(is_open) {
            std::cout << "The FavoriteRepository object with handler " << this->objectid << " has been created.\n";
        } 
        else {
            std::cout << "The FavoriteRepository object with handler " << this->objectid << " could not opened data file.\n";
        }
    }

    virtual ~FavoriteRepository() {
        std::cout << "The FavoriteRepository object with handler " << this->objectid << " is been deleted...\n";

        if(is_open) {
            std::cout << "The ~FavoriteRepository object with handler " << this->objectid << " is closing data file...\n";

            sqlite3_close(db);
        }

        std::cout << "Destructor ~FavoriteRepository has been completed.\n";
    }

    void updateFavorite(VolumeItem* volumeItem) {
        char *zErrMsg = 0;

        std::string sql = "insert into favorite(id, title, smallThumbnail, thumbnail) values ('";
        sql.append(volumeItem->id)
            .append("', '")
            .append(volumeItem->title)
            .append("', '")
            .append(volumeItem->smallThumbnail)
            .append("', '")
            .append(volumeItem->thumbnail)
            .append("'");

        int rc = sqlite3_exec(db, sql.c_str(), callback_insert, 0, &zErrMsg);

        if( rc != SQLITE_OK ){
            std::cout << "SQL error: " << zErrMsg << "\n";
            sqlite3_free(zErrMsg);
        } else {
            std::cout << "updateFavorite: Records created successfully\n";
        }
    }

    bool isFavorite(const std::string& id) {
        char *zErrMsg = 0;
        const int count = 0;

        std::string sql = "SELECT count(1) as count from favorite where id = '";
        sql.append(id).append("'");

        /* Execute SQL statement */
        int rc = sqlite3_exec(db, sql.c_str(), callback_select, (void*) &count, &zErrMsg);
   
        if( rc != SQLITE_OK ) {
            std::cout << "SQL error: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);

            return false;
        } else {
            std::cout << "isFavorite: Operation done successfully\n";

            return count == 1;
        }        
    } 
};

#endif //__VOLUMEITEM_H__
