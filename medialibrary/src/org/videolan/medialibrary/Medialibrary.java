package org.videolan.medialibrary;

import android.content.Context;
import android.os.Environment;
import android.util.Log;

import org.videolan.medialibrary.media.MediaWrapper;

public class Medialibrary {

    private static final String TAG = "VLC/JMedialibrary";
    public long mInstanceID;
    private Context mContext;

    private static Medialibrary mInstance;

    static {
        System.loadLibrary("sqlite3");
        System.loadLibrary("medialibrary");
        System.loadLibrary("mla");
    }

    public Medialibrary(Context context) {
        nativeInit(context.getExternalFilesDir(null).getAbsolutePath(), Environment.getExternalStorageDirectory().getAbsolutePath());
        mContext = context.getApplicationContext();
        nativeDiscover(Environment.getExternalStorageDirectory().getAbsolutePath());
    }

    @Override
    protected void finalize() throws Throwable {
        nativeRelease();
        super.finalize();
    }

    public static Medialibrary getInstance(Context context) {
        if (mInstance == null)
            mInstance = new Medialibrary(context);
        return mInstance;
    }

    public long getId() {
        return mInstanceID;
    }

    public static void onMediaAdded(MediaWrapper[] mediaList) {
        for (MediaWrapper media : mediaList)
            Log.d(TAG, "onMediaAdded: "+media.getTitle());
    }

    public static void onMediaUpdated(MediaWrapper[] mediaList) {
        for (MediaWrapper media : mediaList)
            Log.d(TAG, "onMediaUpdated: "+media.getTitle());
    }

    // Native methods
    public native void nativeInit(String path, String libPath);
    public native void nativeRelease();
    public native void nativeDiscover(String path);
    public native MediaWrapper[] nativeGetVideos();
    public native MediaWrapper[] nativeGetAudio();
}