package com.danim.iotalexa.Fragments;

import android.annotation.SuppressLint;
import android.content.res.ColorStateList;
import android.graphics.Color;
import android.graphics.ColorMatrix;
import android.graphics.ColorMatrixColorFilter;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.AccelerateDecelerateInterpolator;
import android.view.animation.Animation;
import android.view.animation.AnimationSet;
import android.view.animation.ScaleAnimation;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.danim.iotalexa.Beans.LivingRoomStatus;
import com.danim.iotalexa.Helpers.Utils;
import com.danim.iotalexa.R;

import java.util.Locale;

/**
 * A placeholder fragment containing a simple view.
 */
public class LivingRoomFragment extends android.support.v4.app.Fragment
{
    private LivingRoomStatus mLivingRoomStatus;

    private TextView mTemperatureTextView;
    private TextView mTemperatureStateTextView;
    private TextView mHumidityTextView;
    private ImageView mDeskLampImageView;
    private ProgressBar mTemperatureProgressBar;
    private ProgressBar mDeskLampProgressBar;

    public LivingRoomFragment() {}

    /**
     * Returns a new instance of this fragment for the given section
     * number.
     */
    public static LivingRoomFragment newInstance()
    {
        return new LivingRoomFragment();
    }

    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
    {
        View fragment = inflater.inflate(R.layout.fragment_main, container, false);

        View deskLampContainer = fragment.findViewById(R.id.living_room_desk_lamp_container);

        mTemperatureTextView      = fragment.findViewById(R.id.living_room_temperature);
        mTemperatureStateTextView = fragment.findViewById(R.id.living_room_temperature_state);
        mHumidityTextView         = fragment.findViewById(R.id.living_room_humidity);
        mDeskLampImageView        = fragment.findViewById(R.id.living_room_desk_lamp_image);
        mTemperatureProgressBar   = fragment.findViewById(R.id.living_room_temperature_progress);
        mDeskLampProgressBar      = fragment.findViewById(R.id.living_room_desk_lamp_progress);

        mDeskLampProgressBar.setProgressTintList(ColorStateList.valueOf(Color.YELLOW));

        deskLampContainer.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                mLivingRoomStatus.setDeskLamp(!mLivingRoomStatus.isDeskLampOn());

                _turnOnOffLamp(mLivingRoomStatus.isDeskLampOn());
            }
        });

        return fragment;
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState)
    {
        super.onActivityCreated(savedInstanceState);

        // TODO initialize views
        new RetrieveData().execute();
    }

    private class RetrieveData extends AsyncTask<Void, Void, Void>
    {
        @Override
        protected Void doInBackground(Void... voids)
        {
            mLivingRoomStatus = new LivingRoomStatus(22.5f, 17, false);

            return null;
        }

        @SuppressLint("SetTextI18n")
        @Override
        protected void onPostExecute(Void unused)
        {
            mTemperatureTextView.setText(String.format(Locale.US, "%.1f", mLivingRoomStatus.getTemperature()));
            mHumidityTextView.setText(Integer.toString(mLivingRoomStatus.getHumidity()));
            mTemperatureStateTextView.setText(Utils.getTemperatureState(mLivingRoomStatus.getTemperature()));
            mTemperatureProgressBar.setProgress(Utils.normalizeTemperature(mLivingRoomStatus.getTemperature()), true);

            mTemperatureStateTextView.setTextColor(Color.parseColor(Utils.getTemperatureColor(mLivingRoomStatus.getTemperature())));

            _convertImageBW(!mLivingRoomStatus.isDeskLampOn());
            mDeskLampProgressBar.setProgress((mLivingRoomStatus.isDeskLampOn()) ? 100 : 0, true);
        }
    }

    /**
     * Function to control the desk lamp
     * @param on: switch on and off.
     */
    private void _turnOnOffLamp(boolean on)
    {
        _convertImageBW(!on);

        mDeskLampProgressBar.setProgress((on) ? 100 : 0, true);

        AnimationSet animationSet = new AnimationSet(true);
        animationSet.setInterpolator(new AccelerateDecelerateInterpolator());

        ScaleAnimation scaleUp = new ScaleAnimation(
                1.f, 1.1f, 1.0f, 1.1f, Animation.RELATIVE_TO_SELF, .5f, Animation.RELATIVE_TO_SELF, .5f);
        scaleUp.setDuration(75);

        ScaleAnimation scaleDown = new ScaleAnimation(
                1.1f, 1.f, 1.1f, 1.f, Animation.RELATIVE_TO_SELF, .5f, Animation.RELATIVE_TO_SELF, .5f);
        scaleDown.setDuration(75);
        scaleDown.setStartOffset(75);

        animationSet.setFillAfter(true);

        animationSet.addAnimation(scaleUp);
        animationSet.addAnimation(scaleDown);

        mDeskLampImageView.startAnimation(animationSet);
    }

    /**
     * Function to convert an image to BW or color.
     * @param bw: black and white or color.
     */
    private void _convertImageBW(boolean bw)
    {
        ColorMatrix matrix = new ColorMatrix();
        matrix.setSaturation((bw) ? 0 : 5);

        ColorMatrixColorFilter filter = new ColorMatrixColorFilter(matrix);
        mDeskLampImageView.setColorFilter(filter);
    }
}
