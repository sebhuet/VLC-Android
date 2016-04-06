#include <stdlib.h>
#include <jni.h>

#define VLC_JNI_VERSION JNI_VERSION_1_2
static JavaVM *myVm;

jstring
Java_org_videolan_medialibrary_Medialibrary_stringFromJNI( JNIEnv* env,
                                                  jobject thiz )
{
    return (*env)->NewStringUTF(env, "Hello VLC from JNI !");
}

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JNIEnv* env = NULL;
    // Keep a reference on the Java VM.
    myVm = vm;

    if ((*vm)->GetEnv(vm, (void**) &env, VLC_JNI_VERSION) != JNI_OK)
        return -1;
    //LOGD("JNI interface loaded.");
    return VLC_JNI_VERSION;
}