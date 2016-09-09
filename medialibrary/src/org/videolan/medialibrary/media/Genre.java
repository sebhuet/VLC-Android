package org.videolan.medialibrary.media;

public class Genre {

    private long id;
    private String name;

    public Genre(long id, String name) {
        this.id = id;
        this.name = name;
    }

    public long getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public Album[] getAlbums() {
       return nativeGetAlbumsFromGenre(id);
    }

    public Artist[] getArtists() {
        return nativeGetArtistsFromGenre(id);
    }

    public MediaWrapper[] getTracks() {
        return nativeGetTracksFromGenre(id);
    }

    private native Album[] nativeGetAlbumsFromGenre(long id);
    private native Artist[] nativeGetArtistsFromGenre(long id);
    private native MediaWrapper[] nativeGetTracksFromGenre(long id);
}
