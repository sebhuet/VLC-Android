package org.videolan.medialibrary.media;

import android.os.Parcel;
import android.os.Parcelable;

import org.videolan.medialibrary.Medialibrary;

public class Artist extends MediaLibraryItem {

    private String name;
    private String shortBio;
    private String artworkMrl;
    private String musicBrainzId;

    public Artist(long mId, String name, String shortBio, String artworkMrl, String musicBrainzId) {
        this.mId = mId;
        this.name = name;
        this.shortBio = shortBio;
        this.artworkMrl = artworkMrl;
        this.musicBrainzId = musicBrainzId;
    }

    public long getId() {
        return mId;
    }

    public String getTitle() {
        return name;
    }

    public String getShortBio() {
        return shortBio;
    }

    public String getArtworkMrl() {
        return artworkMrl;
    }

    public String getMusicBrainzId() {
        return musicBrainzId;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setShortBio(String shortBio) {
        this.shortBio = shortBio;
    }

    public void setArtworkMrl(String artworkMrl) {
        this.artworkMrl = artworkMrl;
    }

    public Album[] getAlbums(Medialibrary ml) {
        return nativeGetAlbumsFromArtist(ml, mId);
    }

    public MediaWrapper[] getMedia(Medialibrary ml) {
        return nativeGetMediaFromArtist(ml, mId);
    }

    private native Album[] nativeGetAlbumsFromArtist(Medialibrary ml, long mId);
    private native MediaWrapper[] nativeGetMediaFromArtist(Medialibrary ml, long mId);

    @Override
    public void writeToParcel(Parcel parcel, int i) {
        parcel.writeLong(mId);
        parcel.writeString(name);
        parcel.writeString(shortBio);
        parcel.writeString(artworkMrl);
        parcel.writeString(musicBrainzId);
    }

    public static Parcelable.Creator<Artist> CREATOR
            = new Parcelable.Creator<Artist>() {
        public Artist createFromParcel(Parcel in) {
            return new Artist(in);
        }

        public Artist[] newArray(int size) {
            return new Artist[size];
        }
    };

    private Artist(Parcel in) {
        this.mId = in.readLong();
        this.name = in.readString();
        this.shortBio = in.readString();
        this.artworkMrl = in.readString();
        this.musicBrainzId = in.readString();
    }
}
