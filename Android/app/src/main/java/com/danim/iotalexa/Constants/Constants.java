package com.danim.iotalexa.Constants;

import android.Manifest;

public class Constants
{
    public static final String TAG = "IoT Alexa";

    public static final int REQUEST_TIMEOUT = 5000;

    public static final String ACCUWEATHER_API_GET_LOCATION_KEY = "http://dataservice.accuweather.com/locations/v1/cities/search?apikey=?1&q=?2";
    public static final String ACCUWEATHER_API_CURRENT_CONDITIONS = "http://dataservice.accuweather.com/currentconditions/v1/?1?apikey=?2";
    public static final String ACCUWEATHER_API_KEY = "e81b1VV0ojIS0TWqyrzUGlvSPXrLIUxF";
    public static final String ACCUWEATHER_CITY = "Copenhagen";

    public static final int PERMISSION_ALL = 1;
    public static final String[] PERMISSIONS = { Manifest.permission.INTERNET };

    public static final String NODEV1_IP = "http://192.168.1.4";
    public static final String NODEV2_IP = "http://192.168.1.5";

    public static final String TOGGLE_LIGHT  = "/toggleLight";
    public static final String ROOT          = "/";

    public enum TEMPERATURE_STATES
    {
          STRING_COLD("COLD")
        , STRING_CHILLY("CHILLY")
        , STRING_COOL("COOL")
        , STRING_MILD("MILD")
        , STRING_WARM("WARM")
        , STRING_HOT("HOT");

        private final String state;

        TEMPERATURE_STATES(final String state)
        {
            this.state = state;
        }

        @Override
        public String toString()
        {
            return state;
        }

        public String get()
        {
            return state;
        }
    }
}
