 /* 
 Project Name - Smart Helmet
 Author - Bhagirath Shelar 
 Last updated on - 
 Perfectly Run on -   
 */
 
#include <DHT.h>
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.
#include <MQ135.h>

#define PIN_MQ135 A2
#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#define buzzer 8
#define IRSensor 9
#define LED 10
MQ135 mq135_sensor(PIN_MQ135);
DHT dht;
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"

const int PulseWire = A0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0


int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore
float temperature = 21.0; // Assume current temperature. Recommended to measure with DHT22
float humidity = 25.0; // Assume current humidity. Recommended to measure with DHT22
float rzero = mq135_sensor.getRZero();
float correctedRZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
float resistance = mq135_sensor.getResistance();
float ppm = mq135_sensor.getPPM();
float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);

void setup() {
  Serial.begin(9600);
  dht.setup(2); // data pin 2
  pinMode(buzzer,OUTPUT);
  pinMode(IRSensor, INPUT); // IR Sensor pin INPUT
  pinMode(LED, OUTPUT); // LED Pin Output
  digitalWrite(buzzer,HIGH);
  digitalWrite(LED,HIGH); 
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);
  Serial.println("Welcome");
  Serial.println("Thanks for using Smart Helmet");

}

void loop(){
  hum_temp();
  delay(5);
  pulse_count();
  delay(5);
  gas_det();
  delay(5);
  motion_det();
  delay(100);
}

void hum_temp(){
  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

//  Serial.print(dht.getStatusString());
//  Serial.print("\t");
//  Serial.print(humidity, 1);
//  Serial.print("\t\t");
//  Serial.print(temperature, 1);
//  Serial.print("\t\t");
//  Serial.println(dht.toFahrenheit(temperature), 1);
  
 if ( temperature>45)
 {
  digitalWrite(buzzer,LOW);
  Serial.println("Temperature Increased");
  }
 else
  {digitalWrite (buzzer,HIGH);
  Serial.println("temperature is Fine");
  }
  delay(200);
  digitalWrite(buzzer, HIGH);
}

void pulse_count(){
 Serial.println("Reading Pulses");
 int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
                                               
if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened". 
 Serial.println("♥  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
 Serial.print("BPM: ");                        // Print phrase "BPM: " 
 Serial.println(myBPM);                        // Print the value inside of myBPM. 
}
  delay(50);
}

void gas_det() {
  float rzero = mq135_sensor.getRZero();
  float correctedRZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
  float resistance = mq135_sensor.getResistance();
  float ppm = mq135_sensor.getPPM();
  float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);

//  Serial.print("MQ135 RZero: ");
//  Serial.print(rzero);
//  Serial.print("\t Corrected RZero: ");
//  Serial.print(correctedRZero);
//  Serial.print("\t Resistance: ");
//  Serial.print(resistance);
  Serial.print("\t PPM: ");
  Serial.println(ppm);
//  Serial.print("\t Corrected PPM: ");
//  Serial.print(correctedPPM);
//  Serial.println("ppm");
  delay(50);
}

void motion_det(){
  int sensorStatus = digitalRead(IRSensor); // Set the GPIO as Input

  if (sensorStatus == 1)
  {
    digitalWrite(LED, HIGH); // LED High
    digitalWrite(buzzer, HIGH);
    Serial.println("NO OBSTACLE FOUND");
  }
  else
  {
    digitalWrite(LED, LOW); // LED LOW
    digitalWrite(buzzer,LOW); 
    Serial.println("OBSTACLE AHEAD!");
    delay(10); 
  }
delay(200);
digitalWrite(buzzer, HIGH);
digitalWrite(LED,HIGH);
}
