#include <Arduino.h>
#include "sps30.h"
#include "hw_i2c/sample-implementations/arduino/sensirion_hw_i2c_implementation.cpp"

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(100);
  }
}

void loop() {
  struct sps30_measurement measurement;
  s16 ret;

  while (sps30_probe() != 0) {
    Serial.write("probe failed\n");
    delay(1000);
  }

  /* start measurement and wait for 10s to ensure the sensor has a
     stable flow and possible remaining particles are cleaned out */
  if (sps30_start_measurement() != 0) {
    Serial.write("error starting measurement\n");
  }
  delay(10000);

  while (1) {
    delay(1000);
    ret = sps30_read_measurement(&measurement);

    if (ret < 0) {
      Serial.write("read measurement failed\n");
    } else {
      Serial.write("MC 1.0: ");  Serial.println(measurement.mc_1p0, DEC);
      Serial.write("MC 2.5: ");  Serial.println(measurement.mc_2p5, DEC);
      Serial.write("MC 4.0: ");  Serial.println(measurement.mc_4p0, DEC);
      Serial.write("MC 10.0: ");  Serial.println(measurement.mc_10p0, DEC);
      Serial.write("NC 0.5: ");  Serial.println(measurement.nc_0p5, DEC);
      Serial.write("NC 1.0: ");  Serial.println(measurement.nc_1p0, DEC);
      Serial.write("NC 2.5: ");  Serial.println(measurement.nc_2p5, DEC);
      Serial.write("NC 4.0: ");  Serial.println(measurement.nc_4p0, DEC);
      Serial.write("NC 10.0: ");  Serial.println(measurement.nc_10p0, DEC);
      Serial.write("TPS: ");  Serial.println(measurement.typical_particle_size, DEC);
    }
  }
  sps30_stop_measurement();
}
