package com.danim.iotalexa.Beans;

public class LivingRoomStatus
{
    private float mTemperature;
    private int mHumidity;
    private boolean mDeskLamp;

    public LivingRoomStatus(float temperature, int humidity, boolean deskLamp)
    {
        mTemperature = temperature;
        mHumidity    = humidity;
        mDeskLamp    = deskLamp;
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

    public boolean isDeskLampOn()
    {
        return mDeskLamp;
    }

    public void setDeskLamp(boolean deskLamp)
    {
        this.mDeskLamp = deskLamp;
    }
}
