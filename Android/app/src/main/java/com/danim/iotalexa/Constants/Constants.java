package com.danim.iotalexa.Constants;

import android.Manifest;

public class Constants
{
    public static final String TAG = "IoT Alexa";

    public static final String TYPE_FACE = "CaviarDreams.ttf";

    public static final int REQUEST_TIMEOUT = 5000;

    public static final String ACCUWEATHER_API_GET_LOCATION_KEY = "http://dataservice.accuweather.com/locations/v1/cities/search?apikey=?1&q=?2";
    public static final String ACCUWEATHER_API_CURRENT_CONDITIONS = "http://dataservice.accuweather.com/currentconditions/v1/?1?apikey=?2";
    public static final String ACCUWEATHER_API_KEY = "e81b1VV0ojIS0TWqyrzUGlvSPXrLIUxF";
    public static final String ACCUWEATHER_CITY = "Copenhagen";

    public static final int PERMISSION_ALL = 1;
    public static final String[] PERMISSIONS = { Manifest.permission.INTERNET };
}
