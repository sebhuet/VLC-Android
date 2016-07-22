package org.videolan.medialibrary;

public class Medialibrary {

    public long mInstanceID;

    private static Medialibrary mInstance;

    static {
        System.loadLibrary("sqlite3");
        System.loadLibrary("medialibrary");
        System.loadLibrary("mla");
    }

    public Medialibrary() {
        nativeInit();
    }

    public static Medialibrary getInstance() {
        if (mInstance == null)
            mInstance = new Medialibrary();
        return mInstance;
    }

    public long getId() {
        return mInstanceID;
    }

    // Native methods
    public native String nativeInit();
}