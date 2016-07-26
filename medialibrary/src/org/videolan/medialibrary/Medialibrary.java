package org.videolan.medialibrary;

import android.content.Context;

public class Medialibrary {

    public long mInstanceID;
    private Context mContext;

    private static Medialibrary mInstance;

    static {
        System.loadLibrary("sqlite3");
        System.loadLibrary("medialibrary");
        System.loadLibrary("mla");
    }

    public Medialibrary(Context context) {
        nativeInit(context.getExternalFilesDir(null).getAbsolutePath());
        mContext = context.getApplicationContext();
    }

    public static Medialibrary getInstance(Context context) {
        if (mInstance == null)
            mInstance = new Medialibrary(context);
        return mInstance;
    }

    public long getId() {
        return mInstanceID;
    }

    // Native methods
    public native void nativeInit(String path);
}