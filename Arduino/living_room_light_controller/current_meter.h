
// If the current value (milliamps) is greater than this amount then the device is on
const int ON_CURRENT_THRESHOLD = 16;

// Amplitude current
float amplitude_current;
// Effective current
float effective_value;
int sensor_max;
int current_sensor_pin;

extern int deviceState;

void pinInit(int gpio_num)
{
  current_sensor_pin = gpio_num;
  
  pinMode(current_sensor_pin, INPUT);
}

/*
 * Sample for 1000ms and get the maximum value from the SIG pin
 */
int getMaxValue()
{
  // Value read from the sensor
  int sensorValue;
  int sensorMax = 0;
  uint32_t start_time = millis();

  // Sample for 1000ms
  while ((millis() - start_time) < 1000)
  {
    sensorValue = analogRead(current_sensor_pin);
    if (sensorValue > sensorMax)
    {
      // Record the maximum sensor value
      sensorMax = sensorValue;
    }
  }

  return sensorMax;
}

void currentMeter()
{
  sensor_max = getMaxValue();
  // The VCC on the Grove interface of the sensor is 5v
  amplitude_current = (float) sensor_max / 1024 * 5 / 200 * 1000000;
  effective_value = amplitude_current / 1.414;

  Serial.println("The effective value of the current is(in mA)");
  Serial.println(effective_value, 1);

  if (effective_value > ON_CURRENT_THRESHOLD)
  {
    Serial.println("Switch is on");
    deviceState = 1;
  }
  else
  {
    Serial.println("Switch is off");
    deviceState = 0;
  }
}
