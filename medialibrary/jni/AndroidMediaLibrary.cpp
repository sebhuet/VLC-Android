#include "AndroidMediaLibrary.h"
#define LOG_TAG "VLC/JNI/AndroidMediaLibrary"
#include "log.h"

AndroidMediaLibrary::AndroidMediaLibrary(const std::string& appDirPath)
    : ml( NewMediaLibrary() )
{
    ml->setLogger( new AndroidMediaLibraryLogger );
    ml->setVerbosity(medialibrary::LogLevel::Verbose);
    ml->initialize(appDirPath + "/vlc_media.db", appDirPath + "/thumbs", this);
    LOGD("init db: initialized");
}

AndroidMediaLibrary::~AndroidMediaLibrary()
{
    delete ml;
}

void AndroidMediaLibrary::onMediaAdded( std::vector<medialibrary::MediaPtr> media )
{
    LOGD("onMediaAdded %s", media.at(0)->title().c_str());
}
void AndroidMediaLibrary::onMediaUpdated( std::vector<medialibrary::MediaPtr> media )
{
    LOGD("onMediaUpdated %s", media.at(0)->title().c_str());
}
void AndroidMediaLibrary::onMediaDeleted( std::vector<int64_t> ids )
{

}

void AndroidMediaLibrary::onArtistsAdded( std::vector<medialibrary::ArtistPtr> artists )
{

}
void AndroidMediaLibrary::onArtistsModified( std::vector<medialibrary::ArtistPtr> artist )
{

}
void AndroidMediaLibrary::onArtistsDeleted( std::vector<int64_t> ids )
{

}

void AndroidMediaLibrary::onAlbumsAdded( std::vector<medialibrary::AlbumPtr> albums )
{

}
void AndroidMediaLibrary::onAlbumsModified( std::vector<medialibrary::AlbumPtr> albums )
{

}
void AndroidMediaLibrary::onAlbumsDeleted( std::vector<int64_t> ids )
{

}
void AndroidMediaLibrary::onTracksAdded( std::vector<medialibrary::AlbumTrackPtr> tracks )
{

}
void AndroidMediaLibrary::onTracksDeleted( std::vector<int64_t> trackIds )
{

}
void AndroidMediaLibrary::onDiscoveryStarted( const std::string& entryPoint )
{

}
void AndroidMediaLibrary::onDiscoveryCompleted( const std::string& entryPoint )
{

}
void AndroidMediaLibrary::onParsingStatsUpdated( uint32_t percent)
{

}