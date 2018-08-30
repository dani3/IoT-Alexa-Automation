package com.danim.iotalexa.Helpers;

import com.danim.iotalexa.Constants.Constants;

public class Utils
{
    /**
     * Metodo que codifica una URL.
     * @param url: URL a codificar.
     * @return URL codificada.
     */
    public static String fixUrl(final String url)
    {
        return url.replaceAll(" ", "%20");
    }

    /**
     * Function that returns the temperature state given the temperature.
     * @param temperature: temperature value as a float
     * @return string representing the temperature state.
     */
    public static String getTemperatureState(float temperature)
    {
        if (temperature < 15.f)
        {
            return Constants.TEMPERATURE_STATES.STRING_COLD.get();
        }
        else if (temperature < 17.f)
        {
            return Constants.TEMPERATURE_STATES.STRING_CHILLY.get();
        }
        else if (temperature < 19.f)
        {
            return Constants.TEMPERATURE_STATES.STRING_COOL.get();
        }
        else if (temperature < 22.f)
        {
            return Constants.TEMPERATURE_STATES.STRING_MILD.get();
        }
        else if (temperature < 26.f)
        {
            return Constants.TEMPERATURE_STATES.STRING_WARM.get();
        }
        else
        {
            return Constants.TEMPERATURE_STATES.STRING_HOT.get();
        }
    }

    /**
     * Function that returns the temperature color given the temperature.
     * @param temperature: temperature value as a float
     * @return hex value representing the temperature color.
     */
    public static String getTemperatureColor(float temperature)
    {
        if (temperature < 15.f)
        {
            return "#53E6FA";
        }
        else if (temperature < 17.f)
        {
            return "#35CDC7";
        }
        else if (temperature < 19.f)
        {
            return "#36D1B3";
        }
        else if (temperature < 22.f)
        {
            return "#3FE363";
        }
        else if (temperature < 26.f)
        {
            return "#FFF746";
        }
        else
        {
            return "#FFC433";
        }
    }

    /**
     * Function that normalizes the temperature between 0 and 100.
     * @param temperature: temperature value.
     * @return value between 0 and 100.
     */
    public static int normalizeTemperature(float temperature)
    {
        return (int)(((temperature - 14) / (40 - 12)) * 100);
    }
}
