#ifndef ANDROIDDEVICELISTER_H
#define ANDROIDDEVICELISTER_H

#include <medialibrary/IDeviceLister.h>


class AndroidDeviceLister : public medialibrary::IDeviceLister
{
public:
    AndroidDeviceLister();
    std::vector<std::tuple<std::string, std::string, bool>> devices() const;
    void addDevice(std::string, std::string, bool);

private:
    std::vector<std::tuple<std::string, std::string, bool>> m_devices;
};

#endif // ANDROIDDEVICELISTER_H
