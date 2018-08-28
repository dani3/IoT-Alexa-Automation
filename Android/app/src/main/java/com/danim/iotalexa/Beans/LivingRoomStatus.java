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

    public float getmTemperature()
    {
        return mTemperature;
    }

    public void setmTemperature(float mTemperature)
    {
        this.mTemperature = mTemperature;
    }

    public int getmHumidity()
    {
        return mHumidity;
    }

    public void setmHumidity(int mHumidity)
    {
        this.mHumidity = mHumidity;
    }

    public boolean ismDeskLamp()
    {
        return mDeskLamp;
    }

    public void setmDeskLamp(boolean mDeskLamp)
    {
        this.mDeskLamp = mDeskLamp;
    }
}
