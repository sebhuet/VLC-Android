package org.videolan.medialibrary;

public class Medialibrary {

    public Medialibrary() {
        System.loadLibrary("sqlite3");
        System.loadLibrary("medialibrary");
        System.loadLibrary("mla");
    }

    public native String stringFromJNI();
}