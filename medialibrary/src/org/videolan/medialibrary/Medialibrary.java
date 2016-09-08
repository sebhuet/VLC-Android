package org.videolan.medialibrary;

import android.content.Context;
import android.os.Environment;
import android.util.Log;

import org.videolan.medialibrary.interfaces.DevicesDiscoveryCb;
import org.videolan.medialibrary.interfaces.MediaAddedCb;
import org.videolan.medialibrary.interfaces.MediaUpdatedCb;
import org.videolan.medialibrary.media.MediaWrapper;

import java.io.File;
import java.net.URI;

public class Medialibrary {

    private static final String TAG = "VLC/JMedialibrary";

    public static final int FLAG_MEDIA_UPDATED_AUDIO = 1 << 0;
    public static final int FLAG_MEDIA_UPDATED_VIDEO = 1 << 1;
    public static final int FLAG_MEDIA_ADDED_AUDIO = 1 << 2;
    public static final int FLAG_MEDIA_ADDED_VIDEO = 1 << 3;
    public long mInstanceID;
    private Context mContext;

    private MediaUpdatedCb mediaUpdatedCb = null;
    private MediaAddedCb mediaAddedCb = null;
    private DevicesDiscoveryCb devicesDiscoveryCb = null;

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
//        nativeBanFolder(Environment.getExternalStorageDirectory().getAbsolutePath()+"/Android/data/");
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

    public boolean isWorking() {
        return nativeIsWorking();
    }

    public boolean increasePlayCount(long mediaId) {
        return mediaId > 0 && nativeIncreasePlayCount(mediaId);
    }

    public void onMediaAdded(MediaWrapper[] mediaList) {
        if (mediaAddedCb == null)
            return;
        mediaAddedCb.onMediaAdded(mediaList);
    }

    public void onMediaUpdated(MediaWrapper[] mediaList) {
        if (mediaUpdatedCb == null)
            return;
        mediaUpdatedCb.onMediaUpdated(mediaList);
    }

    public void onMediaDeleted(long[] ids) {
        for (long id : ids)
            Log.d(TAG, "onMediaDeleted: "+id);
    }

    public void onDiscoveryStarted(String entryPoint) {
        if (devicesDiscoveryCb != null)
            devicesDiscoveryCb.onDiscoveryStarted(entryPoint);
         Log.d(TAG, "onDiscoveryStarted: "+entryPoint);
    }

    public void onDiscoveryProgress(String entryPoint) {
        if (devicesDiscoveryCb != null)
            devicesDiscoveryCb.onDiscoveryProgress(entryPoint);
         Log.d(TAG, "onDiscoveryProgress: "+entryPoint);
    }

    public void onDiscoveryCompleted(String entryPoint) {
        if (devicesDiscoveryCb != null)
            devicesDiscoveryCb.onDiscoveryCompleted(entryPoint);
         Log.d(TAG, "onDiscoveryCompleted: "+entryPoint);
    }

    public void onParsingStatsUpdated(int percent) {
        if (devicesDiscoveryCb != null)
            devicesDiscoveryCb.onParsingStatsUpdated(percent);
         Log.d(TAG, "onParsingStatsUpdated: "+percent);
    }

    public void remove (MediaWrapper mw) {
        File file = new File(URI.create(mw.getUri().toString()));
        if (file.exists() && file.canWrite())
            nativeReload(file.getParent());
    }

    public void setMediaUpdatedCb(MediaUpdatedCb mediaUpdatedCb, int flags) {
        this.mediaUpdatedCb = mediaUpdatedCb;
        nativeSetMediaUpdatedCbFlag(flags);
    }

    public void removeMediaUpdatedCb() {
        setMediaUpdatedCb(null, 0);
    }

    public void setMediaAddedCb(MediaAddedCb mediaAddedCb, int flags) {
        this.mediaAddedCb = mediaAddedCb;
        nativeSetMediaAddedCbFlag(flags);
    }

    public void setDeviceDiscoveryCb(DevicesDiscoveryCb cb) {
        this.devicesDiscoveryCb = cb;
    }

    public void removeMediaAddedCb() {
        setMediaAddedCb(null, 0);
    }


    // Native methods
    public native void nativeInit(String path, String libPath);
    public native void nativeRelease();
    private native void nativeBanFolder(String path);
    public native void nativeDiscover(String path);
    public native MediaWrapper[] nativeLastMediaPlayed();
    public native MediaWrapper[] nativeGetVideos();
    public native MediaWrapper[] nativeGetAudio();
    public native int nativeGetVideoCount();
    public native int nativeGetAudioCount();
    public native  boolean nativeIsWorking();
    public native void nativePauseBackgroundOperations();
    public native void nativeResumeBackgroundOperations();
    public native void nativeReload();
    public native void nativeReload(String entryPoint);
    private native boolean nativeIncreasePlayCount(long mediaId);
    private native void nativeSetMediaUpdatedCbFlag(int flags);
    private native void nativeSetMediaAddedCbFlag(int flags);
}
