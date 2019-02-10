package com.danim.iotalexa.Fragments;

import android.annotation.SuppressLint;
import android.graphics.Color;
import android.graphics.DashPathEffect;
import android.graphics.Paint;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.AccelerateDecelerateInterpolator;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.danim.iotalexa.Beans.LivingRoomStatus;
import com.danim.iotalexa.Helpers.Utils;
import com.danim.iotalexa.R;
import com.db.chart.animation.Animation;
import com.db.chart.model.LineSet;
import com.db.chart.renderer.AxisRenderer;
import com.db.chart.util.Tools;
import com.db.chart.view.LineChartView;

import java.text.DecimalFormat;
import java.util.Calendar;
import java.util.Locale;

/**
 * A placeholder fragment containing a simple view.
 */
public class LivingRoomFragment extends android.support.v4.app.Fragment
{
    private final int FIRST = 4;
    private final int LAST  = 52;
    private final String[] mTemperatureLabels =
            {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
             "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};

    private final float[][] mTemperatureValues =
            {{ 20.f, 20.f, 20.f, 20.f,
                    22.2f, 22.1f, 22.f, 21.8f, 21.2f, 21.2f, 21.6f, 21.2f, 21.8f, 22.5f, 22.2f, 22.3f,
                    22.2f, 22.5f, 22.f, 21.8f, 21.2f, 21.2f, 21.6f, 21.2f, 21.1f, 20.5f, 19.9f, 19.8f,
                    19.2f, 19.5f, 19.8f, 20.f, 20.2f, 19.8f, 20.2f, 20.9f, 21.1f, 21.5f, 21.9f, 21.2f,
                    22.2f, 22.7f, 22.f, 21.8f, 21.2f, 21.2f, 21.6f, 21.2f, 21.1f, 20.5f, 19.9f, 19.8f,
               20.f, 20.f, 20.f, 20.f },
             { 20.f, 20.f, 20.f, 20.f,
                    20.f, 20.f, 20.f, 20.f, 20.f, 20.f, 20.f, 20.f, 20.f, 20.f, 20.f, 20.f,
                    20.f, 20.f, 20.f, 20.f, 20.f, 20.f, 20.f, 20.f, 20.f, 20.f, 20.f, 20.f,
                    20.f, 20.f, 20.f, 20.f, 20.f, 20.f, 20.f, 20.f, 20.f, 20.f, 20.f, 20.f,
                    20.f, 20.f, 20.f, 20.f, 20.f, 20.f, 20.f, 20.f, 20.f, 20.f, 20.f, 20.f,
               20.f, 20.f, 20.f, 20.f }};

    private final String[] mHumidityLabels =
            {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
                    "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};

    private final float[][] mHumidityValues =
            {{ 20, 20, 20, 20,
                    22, 23, 24, 28, 26, 30, 30, 30, 27, 28, 26, 25,
                    22, 22, 22, 22, 24, 28, 26, 30, 25, 25, 24, 27,
                    22, 24, 22, 22, 28, 28, 23, 26, 23, 24, 27, 27,
                    22, 22, 22, 22, 24, 28, 23, 24, 27, 24, 26, 30,
               20, 20, 20, 20 },
             { 20, 20, 20, 20,
                    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
                    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
                    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
                    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
               20, 20, 20, 20 }};

    private LivingRoomStatus mLivingRoomStatus;

    private LineChartView mTemperatureChart;
    private LineChartView mHumidityChart;
    private Animation mTemperatureChartAnimation;
    private Animation mHumidityChartAnimation;

    private TextView mTemperatureTextView;
    private TextView mTemperatureStateTextView;
    private TextView mHumidityTextView;
    private ProgressBar mTemperatureProgressBar;

    private View mContainer;
    private View mLoadingView;
    private View mErrorTextView;

    public LivingRoomFragment()
    {
        mLivingRoomStatus = new LivingRoomStatus();
    }

    /**
     * Returns a new instance of this fragment for the given section
     * number.
     */
    public static LivingRoomFragment newInstance()
    {
        return new LivingRoomFragment();
    }

    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
    {
        View fragment = inflater.inflate(R.layout.fragment_main, container, false);

        mTemperatureTextView      = fragment.findViewById(R.id.living_room_temperature);
        mTemperatureStateTextView = fragment.findViewById(R.id.living_room_temperature_state);
        mHumidityTextView         = fragment.findViewById(R.id.living_room_humidity);
        mTemperatureProgressBar   = fragment.findViewById(R.id.living_room_temperature_progress);
        mContainer                = fragment.findViewById(R.id.living_room_container);
        mLoadingView              = fragment.findViewById(R.id.living_room_loading);
        mErrorTextView            = fragment.findViewById(R.id.living_room_error);
        mTemperatureChart         = fragment.findViewById(R.id.temperature_chart);
        mHumidityChart            = fragment.findViewById(R.id.humidity_chart);

        mContainer.setVisibility(View.INVISIBLE);
        mErrorTextView.setVisibility(View.INVISIBLE);
        mLoadingView.setVisibility(View.VISIBLE);

        mTemperatureChartAnimation = new Animation();
        mTemperatureChartAnimation.setDuration(375);
        mTemperatureChartAnimation.setInterpolator(new AccelerateDecelerateInterpolator());

        mHumidityChartAnimation = new Animation();
        mHumidityChartAnimation.setDuration(375);
        mHumidityChartAnimation.setInterpolator(new AccelerateDecelerateInterpolator());

        return fragment;
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState)
    {
        super.onActivityCreated(savedInstanceState);

        mLivingRoomStatus.setTemperature(22.5f);
        mLivingRoomStatus.setHumidity(34);

        _initializeViews();
    }

    /**
     * Method that initializes everything once all the devices are connected.
     */
    @SuppressLint("SetTextI18n")
    private void _initializeViews()
    {
        mContainer.setVisibility(View.VISIBLE);
        mErrorTextView.setVisibility(View.INVISIBLE);
        mLoadingView.setVisibility(View.INVISIBLE);

        mTemperatureTextView.setText(
                String.format(Locale.US, "%.1f", mLivingRoomStatus.getTemperature()));
        mHumidityTextView.setText(
                Integer.toString(mLivingRoomStatus.getHumidity()));
        mTemperatureStateTextView.setText(
                Utils.getTemperatureState(mLivingRoomStatus.getTemperature()));
        mTemperatureProgressBar.setProgress(
                Utils.normalizeTemperature(mLivingRoomStatus.getTemperature()), false);

        mTemperatureStateTextView.setTextColor(Color.parseColor(Utils.getTemperatureColor(mLivingRoomStatus.getTemperature())));

        _initializeTemperatureChart();
        _initializeHumidityChart();
    }

    private void _initializeTemperatureChart()
    {
        // Display the hours on the x-axis
        Calendar calendar = Calendar.getInstance();
        int currentHour   = calendar.get(Calendar.HOUR_OF_DAY);
        int currentMinute = calendar.get(Calendar.MINUTE);

        currentHour = (currentMinute < 30) ? currentHour - 1 : currentHour;

        mTemperatureLabels[LAST - 6]      = ((currentHour < 10) ? "0" : "") + Integer.toString(currentHour) + ":00";
        currentHour = (currentHour - 10 < 0) ? 24 - 10 + currentHour : currentHour - 10;
        mTemperatureLabels[LAST - 6 - 18] = ((currentHour < 10) ? "0" : "") + Integer.toString(currentHour) + ":00";
        currentHour = (currentHour - 10 < 0) ? 24 - 10 + currentHour : currentHour - 10;
        mTemperatureLabels[LAST - 6 - 37] = ((currentHour < 10) ? "0" : "") + Integer.toString(currentHour) + ":00";

        // Set the data
        LineSet dataset = new LineSet(mTemperatureLabels, mTemperatureValues[0]);
        dataset.setColor(getResources().getColor(R.color.colorAccentDarkPurple, getResources().newTheme()))
               .setThickness(Tools.fromDpToPx(3))
               .setSmooth(true)
               .beginAt(FIRST)
               .endAt(LAST);

        mTemperatureChart.addData(dataset);

        // Threshold line
        Paint thresPaint = new Paint();
        thresPaint.setColor(Color.parseColor("#0079ae"));
        thresPaint.setStyle(Paint.Style.STROKE);
        thresPaint.setAntiAlias(true);
        thresPaint.setStrokeWidth(Tools.fromDpToPx(.75f));
        thresPaint.setPathEffect(new DashPathEffect(new float[]{10, 10}, 0));

        // Grid lines
        Paint gridPaint = new Paint();
        gridPaint.setColor(Color.parseColor("#cccccc"));
        gridPaint.setStyle(Paint.Style.STROKE);
        gridPaint.setAntiAlias(true);
        gridPaint.setStrokeWidth(Tools.fromDpToPx(.75f));

        // No decimals
        DecimalFormat decimalFormat = new DecimalFormat();
        decimalFormat.setMaximumFractionDigits(0);

        mTemperatureChart.setXLabels(AxisRenderer.LabelPosition.OUTSIDE)
                         .setYLabels(AxisRenderer.LabelPosition.OUTSIDE)
                         .setAxisLabelsSpacing(20)
                         .setLabelsFormat(decimalFormat)
                         .setGrid(0, 3, gridPaint)
                         .setValueThreshold(21f, 21f, thresPaint)
                         .setAxisBorderValues(15, 30)
                         .show(mTemperatureChartAnimation);
    }

    private void _initializeHumidityChart()
    {
        Calendar calendar = Calendar.getInstance();
        int currentHour   = calendar.get(Calendar.HOUR_OF_DAY);
        int currentMinute = calendar.get(Calendar.MINUTE);

        currentHour = (currentMinute < 30) ? currentHour - 1 : currentHour;

        mHumidityLabels[LAST - 6]      = ((currentHour < 10) ? "0" : "") + Integer.toString(currentHour) + ":00";
        currentHour = (currentHour - 10 < 0) ? 24 - 10 + currentHour : currentHour - 10;
        mHumidityLabels[LAST - 6 - 18] = ((currentHour < 10) ? "0" : "") + Integer.toString(currentHour) + ":00";
        currentHour = (currentHour - 10 < 0) ? 24 - 10 + currentHour : currentHour - 10;
        mHumidityLabels[LAST - 6 - 37] = ((currentHour < 10) ? "0" : "") + Integer.toString(currentHour) + ":00";

        LineSet dataset = new LineSet(mHumidityLabels, mHumidityValues[0]);
        dataset.setColor(getResources().getColor(R.color.colorAccentDarkPurple, getResources().newTheme()))
                .setThickness(Tools.fromDpToPx(3))
                .setSmooth(true)
                .beginAt(FIRST)
                .endAt(LAST);

        mHumidityChart.addData(dataset);

        Paint thresPaint = new Paint();
        thresPaint.setColor(Color.parseColor("#0079ae"));
        thresPaint.setStyle(Paint.Style.STROKE);
        thresPaint.setAntiAlias(true);
        thresPaint.setStrokeWidth(Tools.fromDpToPx(.75f));
        thresPaint.setPathEffect(new DashPathEffect(new float[]{10, 10}, 0));

        Paint gridPaint = new Paint();
        gridPaint.setColor(Color.parseColor("#cccccc"));
        gridPaint.setStyle(Paint.Style.STROKE);
        gridPaint.setAntiAlias(true);
        gridPaint.setStrokeWidth(Tools.fromDpToPx(.75f));

        DecimalFormat decimalFormat = new DecimalFormat();
        decimalFormat.setMaximumFractionDigits(0);

        mHumidityChart.setXLabels(AxisRenderer.LabelPosition.OUTSIDE)
                      .setYLabels(AxisRenderer.LabelPosition.OUTSIDE)
                      .setAxisLabelsSpacing(20)
                      .setLabelsFormat(decimalFormat)
                      .setGrid(0, 3, gridPaint)
                      .setValueThreshold(35, 35, thresPaint)
                      .setAxisBorderValues(0, 100)
                      .show(mHumidityChartAnimation);
    }
}
