package org.videolan.medialibrary.media;

import android.os.Parcel;
import android.os.Parcelable;

import org.videolan.medialibrary.Medialibrary;

public class Genre extends MediaLibraryItem {

    private String name;

    public Genre(long mId, String name) {
        this.mId = mId;
        this.name = name;
    }

    public long getId() {
        return mId;
    }

    public String getTitle() {
        return name;
    }

    public Album[] getAlbums(Medialibrary ml) {
       return nativeGetAlbumsFromGenre(ml, mId);
    }

    public Artist[] getArtists(Medialibrary ml) {
        return nativeGetArtistsFromGenre(ml, mId);
    }

    public MediaWrapper[] getTracks(Medialibrary ml) {
        return nativeGetTracksFromGenre(ml, mId);
    }

    private native Album[] nativeGetAlbumsFromGenre(Medialibrary ml, long mId);
    private native Artist[] nativeGetArtistsFromGenre(Medialibrary ml, long mId);
    private native MediaWrapper[] nativeGetTracksFromGenre(Medialibrary ml, long mId);

    @Override
    public void writeToParcel(Parcel parcel, int i) {
        parcel.writeLong(mId);
        parcel.writeString(name);
    }

    public static Parcelable.Creator<Genre> CREATOR
            = new Parcelable.Creator<Genre>() {
        public Genre createFromParcel(Parcel in) {
            return new Genre(in);
        }

        public Genre[] newArray(int size) {
            return new Genre[size];
        }
    };

    private Genre(Parcel in) {
        this.mId = in.readLong();
        this.name = in.readString();
    }
}
