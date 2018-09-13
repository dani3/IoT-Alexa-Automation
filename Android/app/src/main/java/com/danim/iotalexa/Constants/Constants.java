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

    public static final String LIVING_ROOM_DESK_LAMP_IP = "192.168.1.11";

    public static final String LIGHT_ON  = "/lightOn";
    public static final String LIGHT_OFF = "/lightOff";

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
