package com.danim.iotalexa.Helpers;

import android.annotation.SuppressLint;
import android.content.Context;
import android.util.Log;
import android.view.View;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.TextView;

import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonArrayRequest;
import com.danim.iotalexa.Constants.Constants;
import com.danim.iotalexa.R;
import com.danim.iotalexa.Singletons.VolleySingleton;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public class WeatherHelper
{
    private static int mLocationKey;

    /**
     * Function that loads the current weather.
     * @param context: context necessary for general purposes.
     * @param city: current city.
     * @param temperatureTextView: textview containing the temperature.
     * @param statusTextView: textview containig the status.
     * @param containerView: whole weather container.
     * @param loadingView: loading view.
     * @param errorTextView: textview to show an error.
     */
    public static void loadForecastInfo(
              final Context context
            , final String city
            , final TextView temperatureTextView
            , final TextView statusTextView
            , final View containerView
            , final View loadingView
            , final View errorTextView)
    {
        String url = Utils.fixUrl(
                Constants.ACCUWEATHER_API_GET_LOCATION_KEY.replace("?1", Constants.ACCUWEATHER_API_KEY).replace("?2", city));

        Log.d(Constants.TAG, "Connecting to: " + url + " to retrieve the Location Key");

        errorTextView.setVisibility(View.INVISIBLE);
        containerView.setVisibility(View.INVISIBLE);
        loadingView.setVisibility(View.VISIBLE);

        JsonArrayRequest jsonArrayRequest = new JsonArrayRequest(
              Request.Method.GET
            , url
            , null
            , new Response.Listener<JSONArray>()
            {
                @Override
                public void onResponse(JSONArray response)
                {
                    try
                    {
                        mLocationKey = response.getJSONObject(0).getInt("Key");

                        Log.d(Constants.TAG, "Location Key retrieved for Copenhagen: " + mLocationKey);

                        _updateForecastGUI(context, temperatureTextView, statusTextView, containerView, loadingView, errorTextView);

                    } catch (JSONException e) {
                        mLocationKey = -1;

                        // Start the animations
                        AlphaAnimation alphaAnimation = new AlphaAnimation(1.f, .0f);
                        alphaAnimation.setDuration(75);
                        alphaAnimation.setStartOffset(1000);

                        alphaAnimation.setAnimationListener(new Animation.AnimationListener()
                        {
                            @Override
                            public void onAnimationStart(Animation animation) {}

                            @Override
                            public void onAnimationEnd(Animation animation)
                            {
                                loadingView.setVisibility(View.INVISIBLE);
                                errorTextView.setVisibility(View.VISIBLE);
                            }

                            @Override
                            public void onAnimationRepeat(Animation animation) {}
                        });

                        loadingView.startAnimation(alphaAnimation);

                        e.printStackTrace();
                    }
                }
            }
            , new Response.ErrorListener()
            {
                @Override
                public void onErrorResponse(VolleyError error)
                {
                    Log.e(Constants.TAG, "Error retrieving Location Key: " + error.getMessage());

                    mLocationKey = -1;
                }
            });

        VolleySingleton.getInstance(context).addToRequestQueue(jsonArrayRequest);
    }

    private static void _updateForecastGUI(
              final Context context
            , final TextView temperatureTextView
            , final TextView statusTextView
            , final View containerView
            , final View loadingView
            , final View errorTextView)
    {
        String url = Utils.fixUrl(
                Constants.ACCUWEATHER_API_CURRENT_CONDITIONS.replace("?1", Integer.toString(mLocationKey)).replace("?2", Constants.ACCUWEATHER_API_KEY));

        Log.d(Constants.TAG, "Connecting to: " + url + " to retrieve the Current Conditions");

        JsonArrayRequest jsonArrayRequest = new JsonArrayRequest(
              Request.Method.GET
            , url
            , null
            , new Response.Listener<JSONArray>()
            {
                @SuppressLint("SetTextI18n")
                @Override
                public void onResponse(JSONArray response)
                {
                    try
                    {
                        Log.d(Constants.TAG, "Current Conditions retrieved");

                        JSONObject jsonObject = response.getJSONObject(0);

                        // Everything is ok, update the TextViews
                        temperatureTextView.setText(
                                Integer.toString(jsonObject.getJSONObject("Temperature").getJSONObject("Metric").getInt("Value")));
                        statusTextView.setText(
                                jsonObject.getString("WeatherText"));

                        // Start the animations
                        AlphaAnimation alphaAnimation = new AlphaAnimation(1.f, .0f);
                        alphaAnimation.setDuration(75);
                        alphaAnimation.setStartOffset(1000);

                        alphaAnimation.setAnimationListener(new Animation.AnimationListener()
                        {
                            @Override
                            public void onAnimationStart(Animation animation) {}

                            @Override
                            public void onAnimationEnd(Animation animation)
                            {
                                loadingView.setVisibility(View.INVISIBLE);

                                containerView.setVisibility(View.VISIBLE);
                                containerView.startAnimation(
                                        AnimationUtils.loadAnimation(context, R.anim.slide_from_bottom));
                            }

                            @Override
                            public void onAnimationRepeat(Animation animation) {}
                        });

                        loadingView.startAnimation(alphaAnimation);

                    } catch (JSONException e) {
                        // Start the animations
                        AlphaAnimation alphaAnimation = new AlphaAnimation(1.f, .0f);
                        alphaAnimation.setDuration(75);
                        alphaAnimation.setStartOffset(1000);

                        alphaAnimation.setAnimationListener(new Animation.AnimationListener()
                        {
                            @Override
                            public void onAnimationStart(Animation animation) {}

                            @Override
                            public void onAnimationEnd(Animation animation)
                            {
                                loadingView.setVisibility(View.INVISIBLE);

                                errorTextView.setVisibility(View.VISIBLE);
                            }

                            @Override
                            public void onAnimationRepeat(Animation animation) {}
                        });

                        loadingView.startAnimation(alphaAnimation);

                        e.printStackTrace();
                    }
                }
            }
            , new Response.ErrorListener()
            {
                @Override
                public void onErrorResponse(VolleyError error)
                {
                    Log.e(Constants.TAG, "Error retrieving Current Conditions: " + error.getMessage());
                }
            }
        );

        VolleySingleton.getInstance(context).addToRequestQueue(jsonArrayRequest);
    }
}
