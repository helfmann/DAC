#include <Wire.h>
#include <Adafruit_MCP4725.h>

#define DAC1 4 //13
#define DAC2 5 //12
#define DAC3 6 //8
#define DAC4 7 //7

#define pulseHigh(pin) {digitalWrite(pin, HIGH); digitalWrite(pin, LOW); }

Adafruit_MCP4725 dac;
String input;
String DAC_SELECT;
String data;
String CND_TYPE;
void setup() {
  // put your setup code here, to run once:
  pinMode(DAC1, OUTPUT);
  pinMode(DAC2, OUTPUT);
  pinMode(DAC3, OUTPUT);
  pinMode(DAC4, OUTPUT);
  Serial.begin(9600);
  Serial.println("Hello there!");
  dac.begin(0x62);
  dac.setVoltage(0, true);
  dac.begin(0x63);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (input == 0) {
    Serial.println("What DC Voltage or Pressure would you like to set the system at?");
    delay(5000);
  }

  if (Serial.available() > 0) {
    input = Serial.readString();
    DAC_SELECT = input.substring(0, 1);
    int SELECT = DAC_SELECT.toInt();
    CND_TYPE = input.substring(1, 2);
    data = input.substring(2, 7);
    if (input == "stop") {
      dac.begin(0x62);
      dac.setVoltage(0, false);
      dac.begin(0x63);
      Serial.println("STOPPED");
    }
    else {
      switch (SELECT) {
        case 1:
          digitalWrite(DAC1, HIGH);
          break;
        case 2:
          digitalWrite(DAC2, HIGH);
          break;
        case 3:
          digitalWrite(DAC3, HIGH);
          break;
        case 4:
          digitalWrite(DAC4, HIGH);
          break;
        case 9:
          digitalWrite(DAC1, HIGH);
          digitalWrite(DAC2, HIGH);
          digitalWrite(DAC3, HIGH);
          digitalWrite(DAC4, HIGH);
      }
      if (CND_TYPE == "v") {
        float  voltage = data.toFloat();

        if (voltage > 9.9634 || voltage < 0) {
          Serial.println("The DAC cant provide that voltage level, Sorry...");
          voltage = 0;
          dac.setVoltage(voltage, false);

        }
        else {
          voltage = - 1.33332 + voltage * 413.973996;
          Serial.print("Generating DC Voltage of: ");
          Serial.print(input);
          Serial.println();
          dac.setVoltage(voltage, false);
          delay(10);


        }
      }
      if (CND_TYPE == "p") {
        float  pressure = data.toFloat();
        
        if (pressure > 55 || pressure < 0) {
          Serial.println("Either that pressure is too low or you want to blow up the machine, please dont...");
        float      voltage = 0;
          dac.setVoltage(voltage, false);

        }
        else {
         float  voltage =  0.01776 + 0.14722 * pressure;
          voltage = - 1.33332 + voltage * 413.973996;
          Serial.print("Generating Pressure: ");
          Serial.print(input);
          Serial.println();
          dac.setVoltage(voltage, false);
          delay(10);


        }
      }
      digitalWrite(DAC1, LOW);
      digitalWrite(DAC2, LOW);
      digitalWrite(DAC3, LOW);
      digitalWrite(DAC4, LOW);
    }

  }
}
