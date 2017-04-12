#include <NewPing.h>
#include <AccelStepper.h>
//#include <MultiStepper.h>


#define SONAR_NUM     6 // Number or sensors.
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.

#define HALFSTEP 8

// Motor pin definitions
#define motorPin1  2    // IN1 on the ULN2003 driver 1
#define motorPin2  3     // IN2 on the ULN2003 driver 1
#define motorPin3  4     // IN3 on the ULN2003 driver 1
#define motorPin4  5     // IN4 on the ULN2003 driver 1

#define motorPin5  30     // IN1 on the ULN2003 driver 1
#define motorPin6  31    // IN2 on the ULN2003 driver 1
#define motorPin7  32    // IN3 on the ULN2003 driver 1
#define motorPin8  33   // IN4 on the ULN2003 driver 1


// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper stepper1(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper stepper2(HALFSTEP, motorPin5, motorPin7, motorPin6, motorPin8);

unsigned long pingTimer[SONAR_NUM]; // Holds the times when the next ping should happen for each sensor.
unsigned int dis=0,lastping=0;         // Where the ping distances are stored.
uint8_t currentSensor = 0,dir=0;          // Keeps track of which sensor is active.

NewPing sonar[SONAR_NUM] = {     // Sensor object array.
  NewPing(10, 11, MAX_DISTANCE),//0    // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(12, 13, MAX_DISTANCE),//1           //
  NewPing(A0, A1, MAX_DISTANCE),//2           
  NewPing(52, 53, MAX_DISTANCE),//3
  NewPing(50, 51, MAX_DISTANCE),//4
  NewPing(48, 49, MAX_DISTANCE),//5           
  };


int checkdir(int lastping){
  int mdis=0;
  dir=0;
  for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop through all the sensors.
   while((millis()-lastping)<=35);
      sonar[currentSensor].timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
      currentSensor = i;                          // Sensor being accessed.
      sonar[currentSensor].ping_timer(echoCheck);
      Serial.print("sensor");
      Serial.println(currentSensor); 
     Serial.println(dis);
     
    if(mdis<=dis){
      mdis=dis;
      dir=currentSensor;
    }
    //delay(500);
  }
  Serial.println(mdis);
  Serial.print("Direction  ---  ");
  Serial.println(dir);
  return millis();  
}



void echoCheck() { // If ping received, set the sensor distance to array.
  if (sonar[currentSensor].check_timer())
    dis= sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
   else
   dis=200;
}

void botmove(int dir){

if(dir!=0){
/*stepper1.stop();
stepper2.stop();*/
 Serial.println("Turning");
stepper1.setCurrentPosition(0);
stepper2.setCurrentPosition(0);

 stepper1.setSpeed(400);
 stepper2.setSpeed(400);
  
stepper1.moveTo(346*dir);
stepper2.moveTo(346*dir);
Serial.println("Go-to run");

while(stepper1.distanceToGo() != 0&&stepper2.distanceToGo() != 0){
  stepper1.run();
  stepper2.run();
  
}
 Serial.println("Ran");
}
else{

stepper1.setCurrentPosition(0);
                                                                             //    Serial.println("current pos");
                                                                               //   Serial.println(stepper1.currentPosition());
                                                                               //   Serial.println(stepper2.currentPosition());
stepper2.setCurrentPosition(0);

stepper1.setSpeed(400);
stepper2.setSpeed(400);


stepper1.moveTo(4096);
stepper2.moveTo(-4096);
 Serial.println("Moving forward");
while(stepper1.distanceToGo() != 0&&stepper2.distanceToGo() != 0){
  stepper1.run();
  stepper2.run();
 // Serial.println("forward run");
}
}
 Serial.println("bot-moved");
}






void setup() {
  Serial.begin(115200);
  /*pingTimer[0] = millis() + 75;           // First ping starts at 75ms, gives time for the Arduino to chill before starting.
  for (uint8_t i = 1; i < SONAR_NUM; i++) // Set the starting time for each sensor.
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;*/



  stepper1.setMaxSpeed(800.0);
 stepper1.setAcceleration(1000.0);
 
  stepper2.setMaxSpeed(800.0);
 stepper2.setAcceleration(1000.0);


 delay(75);
 lastping=checkdir(millis());
 botmove(dir);

}

void loop() {
  
 lastping=checkdir(lastping);
 botmove(dir);
 delay(1000);
 
}


/*void oneSensorCycle() { // Sensor ping cycle complete, do something with the results.
  for (uint8_t i = 0; i < SONAR_NUM; i++) {
    Serial.print(i);
    Serial.print("=");
    Serial.print(cm[i]);
    Serial.print("cm ");
  }
  Serial.println();
}
Contact GitHub */
