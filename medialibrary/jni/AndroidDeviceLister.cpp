#include "AndroidDeviceLister.h"

#include <mutex>

#define LOG_TAG "VLC/JNI/AndroidDeviceLister"
#include "log.h"

static std::mutex m_mutex;

AndroidDeviceLister::AndroidDeviceLister()
{
}

std::vector<std::tuple<std::string, std::string, bool>>
AndroidDeviceLister::devices() const
{
    std::lock_guard<std::mutex> guard(m_mutex);
    return m_devices;
}

void AndroidDeviceLister::addDevice(std::string uuid, std::string path, bool removable)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    m_devices.push_back(std::make_tuple(uuid, path, removable));
}

