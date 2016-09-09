package org.videolan.medialibrary.media;

public class Album {

    private long id;
    private String title;
    private int releaseYear;
    private String artworkMrl;
    private long albumArtistId;
    private int nbTracks;
    private int duration;

    public Album(long id, String title, int releaseYear, String artworkMrl, long albumArtistId, int nbTracks, int duration) {
        this.id = id;
        this.title = title;
        this.releaseYear = releaseYear;
        this.artworkMrl = artworkMrl;
        this.albumArtistId = albumArtistId;
        this.nbTracks = nbTracks;
        this.duration = duration;
    }

    public long getId() {
        return id;
    }

    public String getTitle() {
        return title;
    }

    public int getReleaseYear() {
        return releaseYear;
    }

    public String getArtworkMrl() {
        return artworkMrl;
    }

    public Artist getAlbumArtist() {
        //TODO
        return null;
    }

    public int getNbTracks() {
        return nbTracks;
    }

    public int getDuration() {
        return duration;
    }

    public MediaWrapper[] getTracks() {
        return nativeGetTracksFromAlbum(id);
    }

    private native MediaWrapper[] nativeGetTracksFromAlbum(long id);
}
