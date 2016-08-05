#include "AndroidDeviceLister.h"

#define LOG_TAG "VLC/JNI/AndroidDeviceLister"
#include "log.h"

AndroidDeviceLister::AndroidDeviceLister()
{
}

std::vector<std::tuple<std::string, std::string, bool>>
AndroidDeviceLister::devices() const
{
    return m_devices;
}

void AndroidDeviceLister::addDevice(std::string uuid, std::string path, bool removable)
{
    m_devices.push_back(std::make_tuple(uuid, path, removable));
}

