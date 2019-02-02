package com.danim.iotalexa.Fragments;

import android.os.Bundle;
import android.support.annotation.NonNull;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.ScaleAnimation;
import android.widget.ImageView;

import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.daimajia.androidanimations.library.Techniques;
import com.daimajia.androidanimations.library.YoYo;
import com.danim.iotalexa.Constants.Constants;
import com.danim.iotalexa.R;
import com.danim.iotalexa.Singletons.VolleySingleton;

public class MyDevicesFragment extends android.support.v4.app.Fragment
{
    private ScaleAnimation mPulseAnimation;

    private ImageView mFootLampImageView;
    private ImageView mAmbilightImageView;
    private ImageView mNodev1Status;
    private ImageView mNodev2Status;

    public MyDevicesFragment() {}

    /**
     * Returns a new instance of this fragment for the given section
     * number.
     */
    public static MyDevicesFragment newInstance()
    {
        return new MyDevicesFragment();
    }

    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
    {
        View fragment = inflater.inflate(R.layout.fragment_devices, container, false);

        View footLampContainer  = fragment.findViewById(R.id.living_room_foot_lamp_container);
        View ambilightContainer = fragment.findViewById(R.id.living_room_ambilight_container);

        mFootLampImageView  = fragment.findViewById(R.id.living_room_foot_lamp_image);
        mAmbilightImageView = fragment.findViewById(R.id.living_room_ambilight_image);

        mNodev1Status = fragment.findViewById(R.id.nodev1_status);
        mNodev2Status = fragment.findViewById(R.id.nodev2_status);

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

        _initializeViews();
    }

    /**
     * Method that initializes everything once all the devices are connected.
     */
    private void _initializeViews()
    {
        StringRequest nodev1Request = new StringRequest(
              Request.Method.GET
            , Constants.NODEV1_IP + Constants.ROOT
            , new Response.Listener<String>()
            {
                @Override
                public void onResponse(String response)
                {
                    mNodev1Status.setImageDrawable(getResources().getDrawable(R.drawable.status_dot_green, getResources().newTheme()));
                }
            }
            , new Response.ErrorListener()
            {
                @Override
                public void onErrorResponse(VolleyError error)
                {
                    mNodev1Status.setImageDrawable(getResources().getDrawable(R.drawable.status_dot_red, getResources().newTheme()));
                }
            });

        StringRequest nodev2Request = new StringRequest(
                Request.Method.GET
                , Constants.NODEV2_IP + Constants.ROOT
                , new Response.Listener<String>()
        {
            @Override
            public void onResponse(String response)
            {
                mNodev2Status.setImageDrawable(getResources().getDrawable(R.drawable.status_dot_green, getResources().newTheme()));
            }
        }
                , new Response.ErrorListener()
        {
            @Override
            public void onErrorResponse(VolleyError error)
            {
                mNodev2Status.setImageDrawable(getResources().getDrawable(R.drawable.status_dot_red, getResources().newTheme()));
            }
        });

        VolleySingleton.getInstance(getContext()).addToRequestQueue(nodev1Request);
        VolleySingleton.getInstance(getContext()).addToRequestQueue(nodev2Request);
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
