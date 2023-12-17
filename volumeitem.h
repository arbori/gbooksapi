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
class FavoriteRepository {
private:
    long objectid; 

    sqlite3* db;
    std::string data_file;
    bool is_open = false;

    int exit_status = 0;

public:
    FavoriteRepository(std::string &data_file);

    virtual ~FavoriteRepository();

    void updateFavorite(VolumeItem* volumeItem);

    bool isFavorite(const std::string& id);
};

#endif //__VOLUMEITEM_H__
