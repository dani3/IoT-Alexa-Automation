package com.danim.iotalexa.Fragments;

import android.annotation.SuppressLint;
import android.graphics.Color;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
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

    private LivingRoomStatus mLivingRoomStatus;

    private ScaleAnimation mPulseAnimation;

    private TextView mTemperatureTextView;
    private TextView mTemperatureStateTextView;
    private TextView mHumidityTextView;
    private ImageView mFootLampImageView;
    private ImageView mAmbilightImageView;
    private ProgressBar mTemperatureProgressBar;

    private View mContainer;
    private View mLoadingView;
    private View mErrorTextView;

    public LivingRoomFragment()
    {
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

        View footLampContainer  = fragment.findViewById(R.id.living_room_foot_lamp_container);
        View ambilightContainer = fragment.findViewById(R.id.living_room_ambilight_container);

        mTemperatureTextView      = fragment.findViewById(R.id.living_room_temperature);
        mTemperatureStateTextView = fragment.findViewById(R.id.living_room_temperature_state);
        mHumidityTextView         = fragment.findViewById(R.id.living_room_humidity);
        mFootLampImageView        = fragment.findViewById(R.id.living_room_foot_lamp_image);
        mAmbilightImageView       = fragment.findViewById(R.id.living_room_ambilight_image);
        mTemperatureProgressBar   = fragment.findViewById(R.id.living_room_temperature_progress);
        mContainer                = fragment.findViewById(R.id.living_room_container);
        mLoadingView              = fragment.findViewById(R.id.living_room_loading);
        mErrorTextView            = fragment.findViewById(R.id.living_room_error);

        mContainer.setVisibility(View.INVISIBLE);
        mErrorTextView.setVisibility(View.INVISIBLE);
        mLoadingView.setVisibility(View.VISIBLE);

        footLampContainer.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                _toggleLamp(mFootLampImageView, Constants.NODEV1_IP);
            }
        });

        ambilightContainer.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                _toggleLamp(mAmbilightImageView, Constants.NODEV2_IP);
            }
        });

        mPulseAnimation = new ScaleAnimation(
                1.f, 1.2f, 1.f, 1.2f, Animation.RELATIVE_TO_SELF, 0.5f, Animation.RELATIVE_TO_SELF, 0.5f);

        mPulseAnimation.setDuration(310);
        mPulseAnimation.setRepeatCount(Animation.INFINITE);
        mPulseAnimation.setRepeatMode(Animation.REVERSE);

        return fragment;
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState)
    {
        super.onActivityCreated(savedInstanceState);

        mErrorWithDevice.set(false);

        mLivingRoomStatus.setTemperature(22.5f);
        mLivingRoomStatus.setHumidity(34);

        if (mDevicesConnected.incrementAndGet() == Constants.LIVING_ROOM_NUMBER_DEVICES)
        {
            _initializeViews();
        }
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
                    Utils.normalizeTemperature(mLivingRoomStatus.getTemperature()), false);

            mTemperatureStateTextView.setTextColor(Color.parseColor(Utils.getTemperatureColor(mLivingRoomStatus.getTemperature())));
        }
        else
        {
            mContainer.setVisibility(View.INVISIBLE);
            mErrorTextView.setVisibility(View.VISIBLE);
            mLoadingView.setVisibility(View.INVISIBLE);
        }
    }

    /**
     * Function to toggle a device on or off.
     */
    private void _toggleLamp(final ImageView device, final String deviceIp)
    {
        String url = deviceIp + Constants.TOGGLE_LIGHT;

        Log.d(Constants.TAG, "Connecting to: " + url);

        StringRequest stringRequest = new StringRequest(url, new Response.Listener<String>()
        {
            @Override
            public void onResponse(String response)
            {
                mPulseAnimation.cancel();
                mPulseAnimation.reset();

                YoYo.with(Techniques.Tada)
                    .duration(750)
                    .playOn(device);
            }
        }
        , new Response.ErrorListener()
        {
            @Override
            public void onErrorResponse(VolleyError error)
            {
                Log.e(Constants.TAG, "Error connecting: " + error.getMessage());

                mPulseAnimation.cancel();
                mPulseAnimation.reset();

                YoYo.with(Techniques.Shake)
                    .duration(375)
                    .playOn(device);
            }
        });

        device.startAnimation(mPulseAnimation);

        VolleySingleton.getInstance(getContext()).addToRequestQueue(stringRequest);
    }
}
