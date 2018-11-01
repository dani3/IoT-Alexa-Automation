package com.danim.iotalexa.Beans;

/**
 * Struct to store the status of the Living Room. It will manage these devices:
 *
 *  - Sensors Device for temperature and humidity.
 *  - Desk Lamp Device.
 *  - Foot Lamp Device
 */
public class LivingRoomStatus
{
    private float mTemperature;
    private int mHumidity;
    private boolean mDeskLamp;
    private boolean mFootLamp;

    public LivingRoomStatus()
    {
        mTemperature = 20.5f;
        mHumidity    = 32;
        mDeskLamp    = false;
        mFootLamp    = false;
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

    public boolean isFootLampOn()
    {
        return mFootLamp;
    }

    public void setFootLamp(boolean footLamp)
    {
        this.mFootLamp = footLamp;
    }
}
