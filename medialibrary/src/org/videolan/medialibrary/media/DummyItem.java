package org.videolan.medialibrary.media;

import android.os.Parcel;
import android.os.Parcelable;

public class DummyItem extends MediaLibraryItem {

    String title;

    public DummyItem(String title) {
        this.title = title;
    }

    @Override
    public long getId() {
        return 0;
    }

    public void setId(long id) {
        mId = id;
    }
    @Override
    public String getTitle() {
        return title;
    }

    @Override
    public void writeToParcel(Parcel parcel, int i) {
        parcel.writeString(title);
    }

    public static Parcelable.Creator<DummyItem> CREATOR
            = new Parcelable.Creator<DummyItem>() {
        public DummyItem createFromParcel(Parcel in) {
            return new DummyItem(in);
        }

        public DummyItem[] newArray(int size) {
            return new DummyItem[size];
        }
    };

    private DummyItem(Parcel in) {
        this.title = in.readString();
    }
}
