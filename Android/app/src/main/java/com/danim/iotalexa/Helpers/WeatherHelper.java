package com.danim.iotalexa.Helpers;

import android.annotation.SuppressLint;
import android.content.Context;
import android.util.Log;
import android.widget.TextView;

import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonArrayRequest;
import com.danim.iotalexa.Constants.Constants;
import com.danim.iotalexa.Singletons.VolleySingleton;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public class WeatherHelper
{
    private static int mLocationKey;

    public static void loadForecastInfo(final Context context, String city, final TextView temperatureTextView, final TextView statusTextView)
    {
        String url = Utils.fixUrl(
                Constants.ACCUWEATHER_API_GET_LOCATION_KEY.replace("?1", Constants.ACCUWEATHER_API_KEY).replace("?2", city));

        Log.d(Constants.TAG, "Connecting to: " + url + " to retrieve the Location Key");

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

                        updateForecastGUI(context, temperatureTextView, statusTextView);

                    } catch (JSONException e) {
                        mLocationKey = -1;

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

    private static void updateForecastGUI(final Context context, final TextView temperatureTextView, final TextView statusTextView)
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
                        JSONObject jsonObject = response.getJSONObject(0);

                        Log.d(Constants.TAG, "Current Conditions retrieved");

                        temperatureTextView.setText(Integer.toString(jsonObject.getJSONObject("Temperature").getJSONObject("Metric").getInt("Value")));
                        statusTextView.setText(jsonObject.getString("WeatherText"));

                    } catch (JSONException e) {
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
