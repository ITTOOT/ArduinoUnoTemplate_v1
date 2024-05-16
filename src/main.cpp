/*************************************************************************
* File Name          : LedTestProectCPP.ino
* Author             : IT to OT
* Version            : V1.0
* Date               : 18/02/2024
* Description        : LED blink check to test board connection with C++
*************************************************************************/

#include <Arduino.h>
#include <Wire.h>
#include <R:\Code\C++\Arduino\!LIBRARIES\!hardwareTestLib\src\led\led.h>
#include <R:\Code\C++\Arduino\!LIBRARIES\!hardwareTestLib\src\led\led.cpp>
#include <R:\Code\C++\Arduino\!LIBRARIES\!hardwareTestLib\src\pwm\pwm.h>
#include <R:\Code\C++\Arduino\!LIBRARIES\!hardwareTestLib\src\pwm\pwm.cpp>
#include <R:\Code\C++\Arduino\!LIBRARIES\!hardwareTestLib\src\sensors\temperature\temperature.h>
#include <R:\Code\C++\Arduino\!LIBRARIES\!hardwareTestLib\src\sensors\temperature\temperature.cpp>
#include <R:\Code\C++\Arduino\!LIBRARIES\!hardwareTestLib\src\lcd\lcdI2c.h>
#include <R:\Code\C++\Arduino\!LIBRARIES\!hardwareTestLib\src\lcd\lcdI2c.cpp>
#include <R:\Code\C++\Arduino\!LIBRARIES\!hardwareTestLib\src\basicControls\controlPanel.h>
#include <R:\Code\C++\Arduino\!LIBRARIES\!hardwareTestLib\src\basicControls\controlPanel.cpp>
#include <R:\Code\C++\Arduino\!LIBRARIES\Grove_LCD_RGB_Backlight-master\rgb_lcd.h>

// Define pinouts
#define LED_PIN 2
#define BUZZ_PIN 3
#define ANALOGUE_TEMP_PIN 0

// Create objects
Led led(LED_PIN);
Pwm pwm(BUZZ_PIN, 0);

PanelControls panelControls;
GroveTempertureControls tempControls;

Lcd lcd(10, 0, 10);
rgb_lcd lcdI2c;

GroveTempControl groveTempControl(tempControls); 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  panelControls.command.enable = false;
  panelControls.command.startDelayTime = 0;


  tempControls.specBConstant = 4275;
  tempControls.specResistance = 100000;
  tempControls.analoguePinIn = A0;  
  tempControls.command.dwellTime = 100;  
  tempControls.command.stabilisationTime = 100; 

  lcd.init(lcdI2c);
}

void loop() {
  // put your main code here, to run repeatedly:

  // Declare variables  
  int adjustableValue = 0;
  int delayTime = 0;

  // Test the hardware
  // LED
  led.test();
  delay(1000);

  // PWM Buzzer
  delayTime = 100; // Declare delayTime as a modifiable lvalue
  panelControls.command.enable = true;
  pwm.enable(panelControls);
  //pwm.enableControlPanel();

  // Scale PWM
  for (int i = 0; i < 100; i++) {
    adjustableValue = i;
    pwm.adjust(BUZZ_PIN, adjustableValue, delayTime, panelControls.status.enabled);
  }
  
  // Turn off the buzzer
  adjustableValue = 0;
  pwm.adjust(BUZZ_PIN, adjustableValue, delayTime, panelControls.status.enabled);
  delay(1000);
  
  // Read the analogue temperature sensor
  tempControls.command.raise = false;
  tempControls.command.lower = false;
  tempControls.command.target.high = 30;
  tempControls.command.target.enter = true;
 
  groveTempControl.enable(panelControls);
  groveTempControl.adjust(tempControls, panelControls); 
  groveTempControl.monitor(tempControls); 
  float temperature = tempControls.status.temperature.actualProcess;

  // Display the temperature on the LCD  
  panelControls.command.startDelayTime = 100;
  int cursorPosition[] = {0, 0};
  lcd.enable(panelControls);
  lcd.display(lcdI2c, temperature, cursorPosition, panelControls);

}

// put function definitions here:


