package com.danim.iotalexa.Beans;

/**
 * Struct to store the status of the Living Room. It will manage these devices:
 *
 *  - Sensor Device for temperature and humidity.
 */
public class LivingRoomStatus
{
    private float mTemperature;
    private int mHumidity;

    public LivingRoomStatus()
    {
        mTemperature = 20.5f;
        mHumidity    = 15;
    }

    public float getTemperature()
    {
        return mTemperature;
    }

    public void setTemperature(float temperature)
    {
        this.mTemperature = temperature;
    }

    public int getHumidity()
    {
        return mHumidity;
    }

    public void setHumidity(int humidity)
    {
        this.mHumidity = humidity;
    }
}
