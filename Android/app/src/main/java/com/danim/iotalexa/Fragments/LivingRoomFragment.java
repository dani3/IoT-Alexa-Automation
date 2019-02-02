package com.danim.iotalexa.Fragments;

import android.annotation.SuppressLint;
import android.graphics.Color;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
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
    private ProgressBar mTemperatureProgressBar;

    private View mContainer;
    private View mLoadingView;
    private View mErrorTextView;

    public LivingRoomFragment()
    {
        mLivingRoomStatus = new LivingRoomStatus();
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

        mTemperatureTextView      = fragment.findViewById(R.id.living_room_temperature);
        mTemperatureStateTextView = fragment.findViewById(R.id.living_room_temperature_state);
        mHumidityTextView         = fragment.findViewById(R.id.living_room_humidity);
        mTemperatureProgressBar   = fragment.findViewById(R.id.living_room_temperature_progress);
        mContainer                = fragment.findViewById(R.id.living_room_container);
        mLoadingView              = fragment.findViewById(R.id.living_room_loading);
        mErrorTextView            = fragment.findViewById(R.id.living_room_error);

        mContainer.setVisibility(View.INVISIBLE);
        mErrorTextView.setVisibility(View.INVISIBLE);
        mLoadingView.setVisibility(View.VISIBLE);

        return fragment;
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState)
    {
        super.onActivityCreated(savedInstanceState);

        mLivingRoomStatus.setTemperature(22.5f);
        mLivingRoomStatus.setHumidity(34);

        _initializeViews();
    }

    /**
     * Method that initializes everything once all the devices are connected.
     */
    @SuppressLint("SetTextI18n")
    private void _initializeViews()
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
}
