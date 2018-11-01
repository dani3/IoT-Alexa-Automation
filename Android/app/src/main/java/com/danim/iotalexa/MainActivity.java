package com.danim.iotalexa;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.support.v4.app.ActivityCompat;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.view.ViewPager;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.animation.AnimationUtils;
import android.widget.TextView;

import com.danim.iotalexa.Constants.Constants;
import com.danim.iotalexa.Fragments.LivingRoomFragment;
import com.danim.iotalexa.Helpers.PermissionsHelper;
import com.danim.iotalexa.Helpers.WeatherHelper;

@SuppressLint("SetTextI18n")
public class MainActivity extends AppCompatActivity
{
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        _initViews();
        _initToolbar();
        _requestPermissions();
        _startAnimations();

        WeatherHelper.loadForecastInfo(
                  this
                , Constants.ACCUWEATHER_CITY
                , (TextView) findViewById(R.id.currently_temperature)
                , (TextView) findViewById(R.id.currently_status)
                , findViewById(R.id.weather_container)
                , findViewById(R.id.currently_loading)
                , findViewById(R.id.currently_error));
    }

    /**
     * Function that initializes the relevant views.
     */
    private void _initViews()
    {
        // Create the adapter that will return a fragment for each of the three
        // primary sections of the activity.
        SectionsPagerAdapter mSectionsPagerAdapter = new SectionsPagerAdapter(getSupportFragmentManager());

        // Set up the ViewPager with the sections adapter.
        ViewPager viewPager = findViewById(R.id.view_pager);
        viewPager.setAdapter(mSectionsPagerAdapter);
    }

    /**
     * Function to initialize the Toolbar.
     */
    private void _initToolbar()
    {
        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
    }

    /**
     * Function to request permissions if needed.
     */
    private void _requestPermissions()
    {
        if (!PermissionsHelper.hasPermissions(this, Constants.PERMISSIONS))
        {
            ActivityCompat.requestPermissions(this, Constants.PERMISSIONS, Constants.PERMISSION_ALL);
        }
    }

    /**
     * Function to start the animations when entering the App.
     */
    private void _startAnimations()
    {
        findViewById(R.id.toolbar_title).startAnimation(
                AnimationUtils.loadAnimation(this, R.anim.slide_from_top));
    }

    /**
     * A {@link FragmentPagerAdapter} that returns a fragment corresponding to
     * one of the sections/tabs/pages.
     */
    private class SectionsPagerAdapter extends FragmentPagerAdapter
    {
        private SectionsPagerAdapter(FragmentManager fm)
        {
            super(fm);
        }

        @Override
        public Fragment getItem(int position)
        {
            // getItem is called to instantiate the fragment for the given page.
            // Return a PlaceholderFragment (defined as a static inner class below).
            return LivingRoomFragment.newInstance();
        }

        @Override
        public int getCount()
        {
            // Show 3 total pages.
            return 1;
        }
    }
}
