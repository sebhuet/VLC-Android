#include <stdlib.h>
#include <string>
#include <jni.h>
#include <medialibrary/IMediaLibrary.h>
#define LOG_TAG "VLC/JNI/MediaLibrary"
#include "log.h"
#include "utils.h"
#include "AndroidMediaLibrary.h"

#define VLC_JNI_VERSION JNI_VERSION_1_2

static JavaVM *myVm;
fields ml_fields;
#define CLASSPATHNAME "org/videolan/medialibrary/Medialibrary"


static inline void throw_IllegalStateException(JNIEnv *env, const char *p_error)
{
    env->ThrowNew(ml_fields.IllegalStateException.clazz, p_error);
}

static inline void throw_IllegalArgumentException(JNIEnv *env, const char *p_error)
{
    env->ThrowNew(ml_fields.IllegalArgumentException.clazz, p_error);
}

static AndroidMediaLibrary *
MediaLibrary_getInstanceInternal(JNIEnv *env, jobject thiz)
{
    return (AndroidMediaLibrary*)(intptr_t) env->GetLongField(thiz,
                                                ml_fields.MediaLibrary.instanceID);
}

AndroidMediaLibrary *
MediaLibrary_getInstance(JNIEnv *env, jobject thiz)
{
    AndroidMediaLibrary *p_obj = MediaLibrary_getInstanceInternal(env, thiz);
    if (!p_obj)
        throw_IllegalStateException(env, "can't get VLCObject instance");
    return p_obj;
}


static void
MediaLibrary_setInstance(JNIEnv *env, jobject thiz, AndroidMediaLibrary *p_obj)
{
    env->SetLongField(thiz,
                         ml_fields.MediaLibrary.instanceID,
                         (jlong)(intptr_t)p_obj);
}

void
init(JNIEnv* env, jobject thiz, jstring appPath )
{
    const char *path = env->GetStringUTFChars(appPath, JNI_FALSE);
    const std::string& stringPath(path);
    AndroidMediaLibrary *aml = new  AndroidMediaLibrary( stringPath );
    MediaLibrary_setInstance(env, thiz, aml);
    env->ReleaseStringUTFChars(appPath, path);
}

void release(JNIEnv* env, jobject thiz)
{
    AndroidMediaLibrary *aml = MediaLibrary_getInstance(env, thiz);
    delete aml;
    MediaLibrary_setInstance(env, thiz, NULL);
}

void
discover(JNIEnv* env, jobject thiz, jstring mediaPath )
{
    //std::string & folderPath ();
    //GetJStringContent(env, path, folderPath);
    const char *path = env->GetStringUTFChars(mediaPath, JNI_FALSE);
    const std::string& stringPath(path);
    AndroidMediaLibrary *aml = MediaLibrary_getInstance(env, thiz);
    if (!aml) {
        env->ReleaseStringUTFChars(mediaPath, path);
        throw std::runtime_error( "discover: Failed to get MediaLibrary instance" );
    }
    aml->discover(stringPath);
    env->ReleaseStringUTFChars(mediaPath, path);
}

static JNINativeMethod methods[] = {
  {"nativeInit", "(Ljava/lang/String;)V", (void*)init },
  {"nativeRelease", "()V", (void*)release },
  {"nativeDiscover", "(Ljava/lang/String;)V", (void*)discover },
};

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JNIEnv* env = NULL;
    // Keep a reference on the Java VM.
    myVm = vm;

    if (vm->GetEnv((void**) &env, VLC_JNI_VERSION) != JNI_OK)
        return -1;

#define GET_CLASS(clazz, str, b_globlal) do { \
    (clazz) = env->FindClass((str)); \
    if (!(clazz)) { \
        LOGE("FindClass(%s) failed", (str)); \
        return -1; \
    } \
    if (b_globlal) { \
        (clazz) = (jclass) env->NewGlobalRef((clazz)); \
        if (!(clazz)) { \
            LOGE("NewGlobalRef(%s) failed", (str)); \
            return -1; \
        } \
    } \
} while (0)

#define GET_ID(get, id, clazz, str, args) do { \
    (id) = env->get((clazz), (str), (args)); \
    if (!(id)) { \
        LOGE(#get"(%s) failed", (str)); \
        return -1; \
    } \
} while (0)

    GET_CLASS(ml_fields.IllegalStateException.clazz,
              "java/lang/IllegalStateException", true);
    GET_CLASS(ml_fields.IllegalArgumentException.clazz,
              "java/lang/IllegalArgumentException", true);

    GET_CLASS(ml_fields.MediaLibrary.clazz, CLASSPATHNAME, true);
    if (env->RegisterNatives(ml_fields.MediaLibrary.clazz, methods, sizeof(methods) / sizeof(methods[0])) < 0) {
        LOGE("RegisterNatives failed for '%s'", CLASSPATHNAME);
        return -1;
    }

    GET_ID(GetFieldID,
           ml_fields.MediaLibrary.instanceID,
           ml_fields.MediaLibrary.clazz,
           "mInstanceID", "J");

#undef GET_CLASS
#undef GET_ID

    LOGD("JNI interface loaded.");
    return VLC_JNI_VERSION;
}

void JNI_OnUnload(JavaVM* vm, void* reserved)
{
    LOGD("JNI interface unloaded.");
    JNIEnv* env = NULL;

    if (vm->GetEnv((void**) &env, VLC_JNI_VERSION) != JNI_OK)
        return;

    env->DeleteGlobalRef(ml_fields.MediaLibrary.clazz);
}