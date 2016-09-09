package org.videolan.medialibrary.media;

public class Artist {

    private long id;
    private String name;
    private String shortBio;
    private String artworkMrl;
    private String musicBrainzId;

    public Artist(long id, String name, String shortBio, String artworkMrl, String musicBrainzId) {
        this.id = id;
        this.name = name;
        this.shortBio = shortBio;
        this.artworkMrl = artworkMrl;
        this.musicBrainzId = musicBrainzId;
    }

    public long getId() {
        return id;
    }

    public String getName() {
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

    public Album[] getAlbums() {
        return nativeGetAlbumsFromArtist(id);
    }

    public MediaWrapper[] getMedia() {
        return nativeGetMediaFromArtist(id);
    }

    private native Album[] nativeGetAlbumsFromArtist(long id);
    private native MediaWrapper[] nativeGetMediaFromArtist(long id);
}
