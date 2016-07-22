
AndroidMediaLibrary::AndroidMediaLibrary( MediaLibrary* medialibrary )
    : ml( medialibrary )
{
}

AndroidMediaLibrary::~AndroidMediaLibrary()
{
    delete ml;
}