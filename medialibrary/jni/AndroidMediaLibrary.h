#pragma once

class AndroidMediaLibrary : public IMediaLibraryCb
{

public:
    AndroidMediaLibrary( MediaLibrary* ml );
    ~AndroidMediaLibrary();

private:
    MediaLibrary* ml;
}
