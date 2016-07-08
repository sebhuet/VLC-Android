#include <stdlib.h>
#include <jni.h>
#include <medialibrary/IMediaLibrary.h>

#define VLC_JNI_VERSION JNI_VERSION_1_2

static JavaVM *myVm;
static const char *classPathName = "org/videolan/medialibrary/Medialibrary";

jstring
init(JNIEnv* env, jobject thiz )
{
    medialibrary::IMediaLibrary* ml = NewMediaLibrary();
    return env->NewStringUTF("ml instanciated");
}

static JNINativeMethod methods[] = {
  {"nativeInit", "()Ljava/lang/String;", (void*)init },
};

/*jstring
Java_org_videolan_medialibrary_Medialibrary_stringFromJNI( JNIEnv* env,
                                                  jobject thiz )
{
    return (*env)->NewStringUTF("Hello VLC from JNI !");
}*/

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JNIEnv* env = NULL;
    // Keep a reference on the Java VM.
    myVm = vm;

    if (vm->GetEnv((void**) &env, VLC_JNI_VERSION) != JNI_OK)
        return -1;

    jclass clazz;
    clazz = env->FindClass(classPathName);
    if (clazz == NULL) {
        //ALOGE("Native registration unable to find class '%s'", className);
        return -1;
    }
    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) < 0) {
        //ALOGE("RegisterNatives failed for '%s'", className);
        return -1;
    }

    //LOGD("JNI interface loaded.");
    return VLC_JNI_VERSION;
}

/*
 * Utils
 */
/*static IMediaLibrary *
VLCJniObject_getInstanceInternal(JNIEnv *env, jobject thiz)
{
    return (vlcjni_object*)(intptr_t) (*env)->GetLongField(env, thiz,
                                                fields.VLCObject.mInstanceID);
}

vlcjni_object *
VLCJniObject_getInstance(JNIEnv *env, jobject thiz)
{
    IMediaLibrary *p_obj = VLCJniObject_getInstanceInternal(env, thiz);
    if (!p_obj)
        throw_IllegalStateException(env, "can't get VLCObject instance");
    return p_obj;
}


static void
VLCJniObject_setInstance(JNIEnv *env, jobject thiz, vlcjni_object *p_obj)
{
    (*env)->SetLongField(env, thiz,
                         fields.VLCObject.mInstanceID,
                         (jlong)(intptr_t)p_obj);
}*/