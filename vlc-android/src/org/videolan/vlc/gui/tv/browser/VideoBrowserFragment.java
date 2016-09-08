/*
 * *************************************************************************
 *  VideoBrowserFragment.java
 * **************************************************************************
 *  Copyright © 2016 VLC authors and VideoLAN
 *  Author: Geoffrey Métais
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *  ***************************************************************************
 */

package org.videolan.vlc.gui.tv.browser;

import android.annotation.TargetApi;
import android.os.Build;
import android.support.v17.leanback.widget.Presenter;
import android.support.v17.leanback.widget.Row;
import android.support.v17.leanback.widget.RowPresenter;

import org.videolan.medialibrary.Medialibrary;
import org.videolan.medialibrary.media.MediaWrapper;
import org.videolan.vlc.VLCApplication;
import org.videolan.vlc.media.MediaUtils;

@TargetApi(Build.VERSION_CODES.JELLY_BEAN_MR1)
public class VideoBrowserFragment extends SortedBrowserFragment {

    private MediaWrapper[] mVideos;

    @Override
    protected void browse() {
        VLCApplication.runBackground(new Runnable() {
            @Override
            public void run() {
                mVideos = Medialibrary.getInstance(VLCApplication.getAppContext()).nativeGetVideos();
                mHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        MediaWrapper media;
                        for (int i = 0; i < mVideos.length; ++i) {
                            media = mVideos[i];
                            addMedia(media);
                            mMediaIndex.put(media.getLocation(), i);
                        }
                        mHandler.sendEmptyMessage(UPDATE_DISPLAY);
                    }
                });
            }
        });

    }

    @Override
    public void onItemClicked(Presenter.ViewHolder viewHolder, Object item, RowPresenter.ViewHolder viewHolder1, Row row) {
        MediaWrapper media = (MediaWrapper) item;
        MediaUtils.openArray(getActivity(), mVideos, mMediaIndex.get(media.getLocation()));
    }
}
