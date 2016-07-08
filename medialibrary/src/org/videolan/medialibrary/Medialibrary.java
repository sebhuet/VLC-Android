package org.videolan.medialibrary;

public class Medialibrary {


static {
        System.loadLibrary("sqlite3");
        System.loadLibrary("medialibrary");
        System.loadLibrary("mla");
}
    public native String nativeInit();
}