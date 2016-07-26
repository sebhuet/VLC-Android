#ifndef ANDROIDMEDIALIBRARY_H
#define ANDROIDMEDIALIBRARY_H

#include <vector>
#include <string>

#include <medialibrary/IMediaLibrary.h>
#include <medialibrary/Types.h>

//namespace androidmedialibrary
//{

class AndroidMediaLibrary : public medialibrary::IMediaLibraryCb
{

public:
    AndroidMediaLibrary(const std::string& path);
    ~AndroidMediaLibrary();

    void onMediaAdded( std::vector<medialibrary::MediaPtr> media );
    void onMediaUpdated( std::vector<medialibrary::MediaPtr> media ) ;
    void onMediaDeleted( std::vector<int64_t> ids ) ;

    void onArtistsAdded( std::vector<medialibrary::ArtistPtr> artists ) ;
    void onArtistsModified( std::vector<medialibrary::ArtistPtr> artist );
    void onArtistsDeleted( std::vector<int64_t> ids );

    void onAlbumsAdded( std::vector<medialibrary::AlbumPtr> albums );
    void onAlbumsModified( std::vector<medialibrary::AlbumPtr> albums );
    void onAlbumsDeleted( std::vector<int64_t> ids );
    void onTracksAdded( std::vector<medialibrary::AlbumTrackPtr> tracks );
    void onTracksDeleted( std::vector<int64_t> trackIds );
    void onDiscoveryStarted( const std::string& entryPoint );
    void onDiscoveryCompleted( const std::string& entryPoint );
    void onParsingStatsUpdated( uint32_t percent);

private:
    medialibrary::IMediaLibrary* ml;
};
//}
#endif // ANDROIDMEDIALIBRARY_H