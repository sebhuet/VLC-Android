package org.videolan.medialibrary.media;

import android.os.Parcelable;

public abstract class MediaLibraryItem implements Parcelable {

    protected long mId;

    public abstract long getId();
    public abstract String getTitle();

    public String getArtworkMrl() {
        return null;
    }

    @Override
    public int describeContents() {
        return 0;
    }

    public String getDescription() {
        return null;
    }
}
