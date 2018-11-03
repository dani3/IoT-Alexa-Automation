package com.danim.iotalexa.Fragments;

import android.annotation.SuppressLint;
import android.content.res.ColorStateList;
import android.graphics.Color;
import android.graphics.ColorMatrix;
import android.graphics.ColorMatrixColorFilter;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.design.widget.Snackbar;
import android.util.Log;
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

import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.daimajia.androidanimations.library.Techniques;
import com.daimajia.androidanimations.library.YoYo;
import com.danim.iotalexa.Beans.LivingRoomStatus;
import com.danim.iotalexa.Constants.Constants;
import com.danim.iotalexa.Helpers.Utils;
import com.danim.iotalexa.R;
import com.danim.iotalexa.Singletons.VolleySingleton;

import java.util.Locale;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * A placeholder fragment containing a simple view.
 */
public class LivingRoomFragment extends android.support.v4.app.Fragment
{
    private AtomicInteger mDevicesConnected;
    private AtomicBoolean mErrorWithDevice;

    private String mUrlDeskLamp;
    private LivingRoomStatus mLivingRoomStatus;

    private TextView mTemperatureTextView;
    private TextView mTemperatureStateTextView;
    private TextView mHumidityTextView;
    private ImageView mDeskLampImageView;
    private ImageView mFootLampImageView;
    private ProgressBar mTemperatureProgressBar;
    private ProgressBar mDeskLampProgressBar;
    private ProgressBar mFootLampProgressBar;

    private View mContainer;
    private View mLoadingView;
    private View mErrorTextView;

    public LivingRoomFragment()
    {
        mUrlDeskLamp = Constants.LIVING_ROOM_DESK_LAMP_IP + Constants.GET_STATUS;

        mLivingRoomStatus = new LivingRoomStatus();

        mDevicesConnected = new AtomicInteger(0);
        mErrorWithDevice  = new AtomicBoolean(false);
    }

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
        View footLampContainer = fragment.findViewById(R.id.living_room_foot_lamp_container);

        mTemperatureTextView      = fragment.findViewById(R.id.living_room_temperature);
        mTemperatureStateTextView = fragment.findViewById(R.id.living_room_temperature_state);
        mHumidityTextView         = fragment.findViewById(R.id.living_room_humidity);
        mDeskLampImageView        = fragment.findViewById(R.id.living_room_desk_lamp_image);
        mFootLampImageView        = fragment.findViewById(R.id.living_room_foot_lamp_image);
        mTemperatureProgressBar   = fragment.findViewById(R.id.living_room_temperature_progress);
        mDeskLampProgressBar      = fragment.findViewById(R.id.living_room_desk_lamp_progress);
        mFootLampProgressBar      = fragment.findViewById(R.id.living_room_foot_lamp_progress);
        mContainer                = fragment.findViewById(R.id.living_room_container);
        mLoadingView              = fragment.findViewById(R.id.living_room_loading);
        mErrorTextView            = fragment.findViewById(R.id.living_room_error);

        mContainer.setVisibility(View.INVISIBLE);
        mErrorTextView.setVisibility(View.INVISIBLE);
        mLoadingView.setVisibility(View.VISIBLE);

        mDeskLampProgressBar.setProgressTintList(ColorStateList.valueOf(Color.YELLOW));
        mFootLampProgressBar.setProgressTintList(ColorStateList.valueOf(Color.YELLOW));

        deskLampContainer.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                _turnOnOffLamp(mLivingRoomStatus.isDeskLampOn(), mDeskLampImageView, mDeskLampProgressBar, true);
            }
        });

        footLampContainer.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                //_turnOnOffLamp(mLivingRoomStatus.isFootLampOn(), mFootLampImageView, mFootLampProgressBar, false);
            }
        });

        return fragment;
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState)
    {
        super.onActivityCreated(savedInstanceState);

        Log.d(Constants.TAG, "Connecting to: " + mUrlDeskLamp);

        StringRequest deskLampRequest = new StringRequest(mUrlDeskLamp, new Response.Listener<String>()
        {
            @Override
            public void onResponse(String response)
            {
                String[] lines = response.split("\n");

                boolean on = (lines[0].contains("On"));
                float temperature = Float.parseFloat(lines[1].substring(lines[1].indexOf(" ") + 1));
                float humidity = Float.parseFloat(lines[2].substring(lines[2].indexOf(" ") + 1));

                Log.d(Constants.TAG, "Desk lamp: " + on);
                Log.d(Constants.TAG, "Temperature: " + temperature);
                Log.d(Constants.TAG, "Humidity: " + humidity);

                mLivingRoomStatus.setDeskLamp(on);
                mLivingRoomStatus.setTemperature(temperature);
                mLivingRoomStatus.setHumidity((int) humidity);

                if (mDevicesConnected.incrementAndGet() == Constants.LIVING_ROOM_NUMBER_DEVICES)
                {
                    _initializeViews();
                }
            }
        }
        , new Response.ErrorListener()
        {
            @Override
            public void onErrorResponse(VolleyError error)
            {
                Log.e(Constants.TAG, "Error connecting: " + error.getMessage());

                mErrorWithDevice.set(true);

                if (mDevicesConnected.incrementAndGet() == Constants.LIVING_ROOM_NUMBER_DEVICES)
                {
                    _initializeViews();
                }
            }
        });

        VolleySingleton.getInstance(getContext()).addToRequestQueue(deskLampRequest);
    }

    /**
     * Method that initializes everything once all the devices are connected.
     */
    @SuppressLint("SetTextI18n")
    private void _initializeViews()
    {
        if (!mErrorWithDevice.get())
        {
            mContainer.setVisibility(View.VISIBLE);
            mErrorTextView.setVisibility(View.INVISIBLE);
            mLoadingView.setVisibility(View.INVISIBLE);

            mTemperatureTextView.setText(
                    String.format(Locale.US, "%.1f", mLivingRoomStatus.getTemperature()));
            mHumidityTextView.setText(
                    Integer.toString(mLivingRoomStatus.getHumidity()));
            mTemperatureStateTextView.setText(
                    Utils.getTemperatureState(mLivingRoomStatus.getTemperature()));
            mTemperatureProgressBar.setProgress(
                    Utils.normalizeTemperature(mLivingRoomStatus.getTemperature()), true);

            mTemperatureStateTextView.setTextColor(Color.parseColor(Utils.getTemperatureColor(mLivingRoomStatus.getTemperature())));

            _convertImageBW(!mLivingRoomStatus.isDeskLampOn(), mDeskLampImageView);
            mDeskLampProgressBar.setProgress((mLivingRoomStatus.isDeskLampOn()) ? 100 : 0, true);

            _convertImageBW(!mLivingRoomStatus.isFootLampOn(), mFootLampImageView);
            mFootLampProgressBar.setProgress((mLivingRoomStatus.isFootLampOn()) ? 100 : 0, true);
        }
        else
        {
            mContainer.setVisibility(View.INVISIBLE);
            mErrorTextView.setVisibility(View.VISIBLE);
            mLoadingView.setVisibility(View.INVISIBLE);
        }
    }

    /**
     * Function to control the desk lamp
     * @param on: switch on and off.
     */
    private void _turnOnOffLamp(final boolean on, final ImageView lamp, final ProgressBar lampProgress, final boolean deskLamp)
    {
        // TODO: add foot lamp
        String url = (deskLamp) ? Constants.LIVING_ROOM_DESK_LAMP_IP + ((on) ? Constants.SWITCH_LIGHT_OFF : Constants.SWITCH_LIGHT_ON) : null;

        Log.d(Constants.TAG, "Connecting to: " + url);

        StringRequest stringRequest = new StringRequest(url, new Response.Listener<String>()
        {
            @Override
            public void onResponse(String response)
            {
                if (deskLamp)
                {
                    mLivingRoomStatus.setDeskLamp(!on);
                }
                else
                {
                    mLivingRoomStatus.setFootLamp(!on);
                }

                _convertImageBW(on, lamp);
                _animateLamp(!on, lamp, lampProgress);
            }
        }
        , new Response.ErrorListener()
        {
            @Override
            public void onErrorResponse(VolleyError error)
            {
                Log.e(Constants.TAG, "Error connecting: " + error.getMessage());

                YoYo.with(Techniques.Shake)
                    .duration(375)
                    .playOn((deskLamp) ? mDeskLampImageView : mFootLampImageView);
            }
        });

        VolleySingleton.getInstance(getContext()).addToRequestQueue(stringRequest);
    }

    private void _animateLamp(boolean on, ImageView lamp, ProgressBar lampProgress)
    {
        lampProgress.setProgress((on) ? 100 : 0, true);

        AnimationSet animationSet = new AnimationSet(true);
        animationSet.setInterpolator(new AccelerateDecelerateInterpolator());

        ScaleAnimation scaleUp = new ScaleAnimation(
                1.f, 1.05f, 1.f, 1.05f, Animation.RELATIVE_TO_SELF, .5f, Animation.RELATIVE_TO_SELF, .5f);
        scaleUp.setDuration(75);

        ScaleAnimation scaleDown = new ScaleAnimation(
                1.05f, 1.f, 1.05f, 1.f, Animation.RELATIVE_TO_SELF, .5f, Animation.RELATIVE_TO_SELF, .5f);
        scaleDown.setDuration(75);
        scaleDown.setStartOffset(75);

        animationSet.setFillAfter(true);

        animationSet.addAnimation(scaleUp);
        animationSet.addAnimation(scaleDown);

        lamp.startAnimation(animationSet);
    }

    /**
     * Function to convert an image to BW or color.
     * @param bw: black and white or color.
     */
    private void _convertImageBW(boolean bw, ImageView lamp)
    {
        ColorMatrix matrix = new ColorMatrix();
        matrix.setSaturation((bw) ? 0 : 1);

        ColorMatrixColorFilter filter = new ColorMatrixColorFilter(matrix);
        lamp.setColorFilter(filter);
    }
}
