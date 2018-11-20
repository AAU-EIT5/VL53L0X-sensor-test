#include <VL53L0X.h>
#include <Wire.h>

VL53L0X sensor0;
VL53L0X sensor1;

const unsigned int sensor_xshut[] = {8, 7};
int distances[2][100];
uint32_t count=0;
unsigned long startTime;
unsigned long Time;
char databuffer;

void setup() 
{

  Serial.begin(115200);
  Wire.begin();

  for(int i=0; i<sizeof(sensor_xshut)/sizeof(sensor_xshut[0]); i++)
  {
    pinMode(sensor_xshut[i], OUTPUT);
    digitalWrite(sensor_xshut[i], LOW);
  }

  Serial.println("Enabled sensor 0");
  pinMode(sensor_xshut[0], INPUT); // Enable the first sensor
  delay(150);

  Serial.println("Init sensor 0");
  sensor0.setTimeout(500);
  sensor0.setAddress(0xA0);
  sensor0.init();

  Serial.println("Enabled sensor 1");
  pinMode(sensor_xshut[1], INPUT); // Enable the second sensor
  delay(150);

  sensor1.setTimeout(500);
  sensor1.setAddress(0xB0);
  sensor1.init();

  sensor0.setMeasurementTimingBudget(20000);
  sensor1.setMeasurementTimingBudget(20000);


  Serial.println("Starting continous ranging");
  sensor0.startContinuous();
  sensor1.startContinuous();
}

unsigned long last_send = 0;

void loop()
{
  startTime = micros();
  for(int i =0; i<100; i++){
    distances[0][i] = sensor0.readRangeContinuousMillimeters();
    if (sensor0.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

    distances[1][i] = sensor1.readRangeContinuousMillimeters();
    if (sensor1.timeoutOccurred()) { Serial.print(" TIMEOUT"); }    
  }
  
  Time = micros() - startTime;

  for(int i = 0; i < 100; i++){
   Serial.print(distances[0][i]); Serial.print("\t"); Serial.println(distances[1][i]);
  }
  Serial.print("time: "); Serial.println(Time);
  while(!Serial.available()){
   
  }
  while(Serial.available()){
    databuffer = Serial.read();
  }   
}
