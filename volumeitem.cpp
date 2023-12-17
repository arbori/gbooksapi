#include "volumeitem.h"

/////////////////////////////////////////////////////////////////////
// Auxiliar functions
/////////////////////////////////////////////////////////////////////
static int callback_insert(void *NotUsed, int argc, char **argv, char **azColName);

static int callback_select(void *count, int argc, char **argv, char **azColName);

int insertFavorite(sqlite3* db, VolumeItem* volumeItem);

int deleteFavorite(sqlite3* db, const std::string& id);

/////////////////////////////////////////////////////////////////////
// FavoriteRepository class implementation
/////////////////////////////////////////////////////////////////////
FavoriteRepository::FavoriteRepository(std::string &data_file) {
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

FavoriteRepository::~FavoriteRepository() {
    std::cout << "The FavoriteRepository object with handler " << this->objectid << " is been deleted...\n";

    if(is_open) {
        std::cout << "The ~FavoriteRepository object with handler " << this->objectid << " is closing data file...\n";

        sqlite3_close(db);
    }

    std::cout << "Destructor ~FavoriteRepository has been completed.\n";
}

void FavoriteRepository::updateFavorite(VolumeItem* volumeItem) {
    std::cout << "FavoriteRepository::updateFavorite(VolumeItem* volumeItem).\n";

    int rc = insertFavorite(db, volumeItem);

    if(rc  != SQLITE_OK && (rc = deleteFavorite(db, volumeItem->id)) != SQLITE_OK) {
        std::cout << "Favorite was not updated.\n";
    } else {
        std::cout << "updateFavorite: Favorite updated successfully\n";
    }
}

bool FavoriteRepository::isFavorite(const std::string& id) {
    std::cout << "FavoriteRepository::isFavorite(const std::string& id).\n";

    char *zErrMsg = 0;
    int count = -13;

    std::string sql = "SELECT count(1) as count from favorite where id = '";
    sql.append(id).append("'");

    /* Execute SQL statement */
    int rc = sqlite3_exec(db, sql.c_str(), callback_select, (void*) &count, &zErrMsg);

    if( rc != SQLITE_OK ) {
        std::cout << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);

        return false;
    } else {
        return count == 1;
    }        

    return true;
}

static int callback_insert(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;

   for(i = 0; i<argc; i++) {
      std::cout << azColName[i] << " = " << argv[i] ? argv[i] : "NULL\n";
   }

   std::cout << std::endl;

   return 0;
}

static int callback_select(void *count, int argc, char **argv, char **azColName) {
    int i;
    char* output;

    if(argc == 1 && (argv[0])) {
        *((int*) count) = (int) strtol(argv[0], &output, 10);
    }

    return 0;
}

int insertFavorite(sqlite3* db, VolumeItem* volumeItem) {
    char *zErrMsg = 0;

    std::string sql = "insert into favorite(id, title, smallThumbnail, thumbnail) values ('";
    sql.append(volumeItem->id)
        .append("', '")
        .append(volumeItem->title)
        .append("', '")
        .append(volumeItem->smallThumbnail)
        .append("', '")
        .append(volumeItem->thumbnail)
        .append("')");

    int rc = sqlite3_exec(db, sql.c_str(), callback_insert, 0, &zErrMsg);

    sqlite3_free(zErrMsg);

    return rc;
}

int deleteFavorite(sqlite3* db, const std::string& id) {
    char *zErrMsg = 0;

    std::string sql = "delete from favorite where id = '";
    sql.append(id).append("'");

    int rc = sqlite3_exec(db, sql.c_str(), callback_insert, 0, &zErrMsg);

    sqlite3_free(zErrMsg);

    return rc;
}

