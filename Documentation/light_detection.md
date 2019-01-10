# Photoresistor

The easiest way to measure a resistive sensor is to connect one end to Power and the other to a pull-down resistor to ground. Then the point between the fixed pulldown resistor and the variable photocell resistor is connected to the analog input. The way this works is that as the resistance of the photocell decreases, the total resistance of the photocell and the pulldown resistor decreases from over 600KΩ to 10KΩ. That means that the current flowing through both resistors increases which in turn causes the voltage across the fixed 10KΩ resistor to increase.

If you're planning to have the sensor in a bright area and use a 10KΩ pulldown, it will quickly saturate. That means that it will hit the 'ceiling' of 5V and not be able to differentiate between kinda bright and really bright. In that case, you should replace the 10KΩ pulldown with a 1KΩ pulldown. In that case, it will not be able to detect dark level differences as well but it will be able to detect bright light differences better.

---

## References

* [Using a photocell](https://learn.adafruit.com/photocells/using-a-photocell)