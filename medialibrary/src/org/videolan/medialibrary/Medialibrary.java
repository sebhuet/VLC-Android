package org.videolan.medialibrary;

import android.content.Context;
import android.os.Environment;
import android.util.Log;

import org.videolan.medialibrary.media.MediaWrapper;

public class Medialibrary {

    private static final String TAG = "VLC/JMedialibrary";
    public long mInstanceID;
    private Context mContext;

    private static Medialibrary sInstance;

    static {
        System.loadLibrary("c++_shared");
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

    public static synchronized Medialibrary getInstance(Context context) {
        if (sInstance == null)
            sInstance = new Medialibrary(context);
        return sInstance;
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

    public static void onMediaDeleted(long[] ids) {
        for (long id : ids)
            Log.d(TAG, "onMediaDeleted: "+id);
    }

    public static void onDiscoveryStarted(String entryPoint) {
         Log.d(TAG, "onDiscoveryStarted: "+entryPoint);
    }

    public static void onDiscoveryCompleted(String entryPoint) {
         Log.d(TAG, "onDiscoveryCompleted: "+entryPoint);
    }

    public static void onParsingStatsUpdated(int percent) {
         Log.d(TAG, "onParsingStatsUpdated: "+percent);
    }


    // Native methods
    public native void nativeInit(String path, String libPath);
    public native void nativeRelease();
    public native void nativeDiscover(String path);
    public native MediaWrapper[] nativeGetVideos();
    public native MediaWrapper[] nativeGetAudio();
}