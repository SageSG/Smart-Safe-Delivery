/*************************************************************************
   BMA250 Tutorial:
   This example program will show the basic method of printing out the
   accelerometer values from the BMA250 to the Serial Monitor, and the
   Serial Plotter

   Hardware by: TinyCircuits
   Code by: Laverena Wienclaw for TinyCircuits

   Initiated: Mon. 11/1/2018
   Updated: Tue. 11/2/2018
 ************************************************************************/
const int buzz = 7;
const int led = 10;

#include <Wire.h>         // For I2C communication with sensor
#include "BMA250.h"       // For interfacing with the accel. sensor


#include <SPI.h>
#include <STBLE.h>

//Debug output adds extra flash and memory requirements!
#ifndef BLE_DEBUG
#define BLE_DEBUG true
#endif

uint8_t ble_rx_buffer[21];
uint8_t ble_rx_buffer_len = 0;
uint8_t ble_connection_state = false;
#define PIPE_UART_OVER_BTLE_UART_TX_TX 0

uint8_t sendBuffer[15] = "Object stolen\0";
      uint8_t sendLength = 15;
      uint8_t sendBuffer1[15] = "Normal\0";

// Accelerometer sensor variables for the sensor and its values
BMA250 accel_sensor;
int x, y, z;
double temp;

#if defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#else
#define SerialMonitorInterface Serial
#endif

void setup() {
  SerialMonitorInterface.begin(115200);
  Wire.begin();
  pinMode(led, OUTPUT);
  SerialMonitorInterface.print("Initializing BMA...");
  // Set up the BMA250 acccelerometer sensor
  accel_sensor.begin(BMA250_range_2g, BMA250_update_time_64ms);
  while (!SerialMonitorInterface); //This line will block until a serial monitor is opened with TinyScreen+!
  BLEsetup();
}

void loop() {
  accel_sensor.read();//This function gets new data from the acccelerometer
  digitalWrite(led, LOW);
  // Get the acceleration values from the sensor and store them into global variables
  // (Makes reading the rest of the program easier)

  x = accel_sensor.X;
  y = accel_sensor.Y;
  z = accel_sensor.Z;

  temp = ((accel_sensor.rawTemp * 0.5) + 24.0);

  // If the BMA250 is not found, nor connected correctly, these values will be produced
  // by the sensor
  if (x == -1 && y == -1 && z == -1) {
    // Print error message to Serial Monitor
    SerialMonitorInterface.print("ERROR! NO BMA250 DETECTED!");
  }

  else { // if we have correct sensor readings:
    showSerial();                 //Print to Serial Monitor or Plotter
    if (!lib_aci_send_data(PIPE_UART_OVER_BTLE_UART_TX_TX, (uint8_t*)sendBuffer1, sendLength))
      {
        SerialMonitorInterface.println(F("TX dropped!"));
      }
    if (x > 50 || x < -50 || y > 50 || y < -50) {
      SerialMonitorInterface.print("AHHHHH");
      SerialMonitorInterface.println("No Object In Range");
      if (!lib_aci_send_data(PIPE_UART_OVER_BTLE_UART_TX_TX, (uint8_t*)sendBuffer, sendLength))
      {
        SerialMonitorInterface.println(F("TX dropped!"));
      }
      digitalWrite(led, HIGH);
      tone(buzz, 2000);
      delay(100);
      noTone(buzz);
      delay(100);

      tone(buzz, 2000);
      delay(100);
      noTone(buzz);
      delay(100);

      tone(buzz, 2000);
      delay(100);
      noTone(buzz);

      tone(buzz, 2000);
      delay(100);
      noTone(buzz);
      delay(100);
    }
  }

  // The BMA250 can only poll new sensor values every 64ms, so this delay
  // will ensure that we can continue to read values
  delay(250);
  // ***Without the delay, there would not be any sensor output***

  aci_loop();//Process any ACI commands or events from the NRF8001- main BLE handler, must run often. Keep main loop short.
  if (ble_rx_buffer_len) {//Check if data is available
    SerialMonitorInterface.print(ble_rx_buffer_len);
    SerialMonitorInterface.print(" : ");
    SerialMonitorInterface.println((char*)ble_rx_buffer);
    ble_rx_buffer_len = 0;//clear afer reading
  }
  if (SerialMonitorInterface.available()) {//Check if serial input is available to send
    delay(10);//should catch input
    uint8_t sendBuffer[21];
    uint8_t sendLength = 0;
    while (SerialMonitorInterface.available() && sendLength < 19) {
      sendBuffer[sendLength] = SerialMonitorInterface.read();
      sendLength++;
    }
    if (SerialMonitorInterface.available()) {
      SerialMonitorInterface.print(F("Input truncated, dropped: "));
      if (SerialMonitorInterface.available()) {
        SerialMonitorInterface.write(SerialMonitorInterface.read());
      }
    }
    sendBuffer[sendLength] = '\0'; //Terminate string
    sendLength++;
    if (!lib_aci_send_data(PIPE_UART_OVER_BTLE_UART_TX_TX, (uint8_t*)sendBuffer, sendLength))
    {
      SerialMonitorInterface.println(F("TX dropped!"));
    }
  }
}

// Prints the sensor values to the Serial Monitor, or Serial Plotter (found under 'Tools')
void showSerial() {
  SerialMonitorInterface.print("X = ");
  SerialMonitorInterface.print(x);

  SerialMonitorInterface.print("  Y = ");
  SerialMonitorInterface.print(y);

  SerialMonitorInterface.print("  Z = ");
  SerialMonitorInterface.print(z);

  SerialMonitorInterface.print("  Temperature(C) = ");
  SerialMonitorInterface.println(temp);
}
