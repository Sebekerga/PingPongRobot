//Libraries init
#include "UltraDistance.h"
#include <MotorDriver.h>
#include <MPU6050.h>
#include <OLED_I2C.h>
#include <Wire.h>


#define Trig 7
#define Echo 6
#define target 50
#define a 6
#define endByte 101

//OLED init
OLED  myOLED(SDA, SCL, 8);
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];
extern uint8_t SmallFont[];

//US init
UltraDistance distance_meter(Echo, Trig);

//Motor driver init
MotorDriver driver;
int8_t power_a = 0, power_b = 0;
byte buf[3];

//MPU6050 init
MPU6050 mpu;
//Timers
unsigned long timer = 0;
float timeStep = 0.05;
//Yaw storage
float yaw = 0;
byte yaw_byte;
Vector norm;


void setup() {

  //Serial setup
  Serial.begin (115200);
  Serial.println("Pi-Dop. i'm ready");

  //OLED setup
  myOLED.begin();
  myOLED.setFont(MediumNumbers);

  //Motor driver setup
  driver.begin();

  //Setup MPU6050
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  mpu.calibrateGyro();
  mpu.setThreshold(3);
}


void loop() {  

  //Timer start
  timer = millis();

  // //Getting distance
  // long distance = distance_meter.read();

  // //Testing for range
  // if (distance >= 200 || distance <= 0){
  //   //Serial.println("Out of range");
  //   distance = 0;
  //   myOLED.setFont(SmallFont);
  //   myOLED.print("Out of range", CENTER, 30);
  // }
  // else {
  //   //Serial.print(distance);
  //   //Serial.println(" cm");
  //   myOLED.setFont(BigNumbers);    
  //   myOLED.printNumI(distance, CENTER, 30);
  // }

  // //Output distance
  // Serial.write(distance);  

  // //Motors power on/off
  // long dif = (distance - target) * a;
  // if(dif > 80) dif = 80;
  // if(dif < -80) dif = -80;
  
  // driver.speed(MOTORA, dif);
  // driver.speed(MOTORB, dif);

  //Getting Yaw
  norm = mpu.readNormalizeGyro();
  yaw = yaw + norm.ZAxis * timeStep;
  yaw_byte = (byte) yaw;
  
  Serial.write(yaw_byte);
  
  while(Serial.available() >= 3){
    if(Serial.readBytesUntil(101, buf, 3) == 3){
      power_a = buf[0];
      power_b = buf[1];
    }
  }

  Serial.print(power_a);
  Serial.print(" : ");
  Serial.print(power_b);
  Serial.print("  (");
  Serial.print(buf[0]);
  Serial.print(" : ");
  Serial.print(buf[1]);
  Serial.print(" : ");
  Serial.print(buf[2]);
  Serial.println(")");

  //Motors power on/off
  if(power_a > 80) power_a = 80;
  if(power_a < -80) power_a = -80;
  if(power_b > 80) power_b = 80;
  if(power_b < -80) power_b = -80;
  
  driver.speed(MOTORA, power_a);
  driver.speed(MOTORB, power_b);

  // myOLED.setFont(MediumNumbers);    
  // myOLED.printNumI(dif, CENTER, 0);

  // //Updating display  
  // myOLED.update();
  // myOLED.clrScr();

  //Wait to full timeStep period
  delay((timeStep*1000) - (millis() - timer));
}

