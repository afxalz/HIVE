#include <NewPing.h>
#include <AccelStepper.h>
#include <math.h>
                                                                //#include <Wire.h>

#define SONAR_NUM     6 // Number or sensors.
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.

unsigned int pingdis[6]={2,2,2,2,2,2},lastping=0;         // Where the ping distances are stored.
uint8_t currentSensor = 0;
double angle=0;   

      

NewPing sonar[SONAR_NUM] = {           // Sensor object array.
  NewPing(10,11, MAX_DISTANCE),//0    // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(A2,A3, MAX_DISTANCE),//1           
  NewPing(A4,A5, MAX_DISTANCE),//2           
  NewPing(6,7, MAX_DISTANCE),//3
  NewPing(8,9, MAX_DISTANCE),//4
  NewPing(0,1, MAX_DISTANCE),//5           
  };

#define HALFSTEP 8

// Motor pin definitions
#define motorPin1  2    // IN1 on the ULN2003 driver 1
#define motorPin2  3     // IN2 on the ULN2003 driver 1
#define motorPin3  4     // IN3 on the ULN2003 driver 1
#define motorPin4  5     // IN4 on the ULN2003 driver 1

#define motorPin5  A1     // IN1 on the ULN2003 driver 1
#define motorPin6  A0    // IN2 on the ULN2003 driver 1
#define motorPin7  13    // IN3 on the ULN2003 driver 1
#define motorPin8  12   // IN4 on the ULN2003 driver 1


// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper stepper1(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper stepper2(HALFSTEP, motorPin5, motorPin7, motorPin6, motorPin8);


int checkdis(int last_ping){

	
	for (uint8_t i = 0; i < SONAR_NUM; i++) {       // Loop through all the sensors.
		Serial.println("sensing");
		while((millis()-last_ping)<=50);
		
		//sonar[currentSensor].timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
		currentSensor = i;                          // Sensor being accessed.
   pingdis[currentSensor]=sonar[currentSensor].ping_cm();
   if(pingdis[currentSensor]==0){
    pingdis[currentSensor]=50;
   }
		//sonar[currentSensor].ping_timer(echoCheck);
                                                  
 /*   Serial.print(i);                            //Data to the processing environment      
    Serial.print(",");
    Serial.print(pingdis[currentSensor]);
    Serial.print(".");*/
    
	}

  Serial.println("Distances measured by sensors: ");
 
 for(uint8_t i = 0; i<SONAR_NUM; i++) {
  Serial.print(i);
  Serial.print("-----");
	Serial.println(pingdis[i]);
 }

 
	return millis();  
}



/*void echoCheck() {									 // If ping received, set the sensor distance to array.
Serial.println("echo");
	//pingdis[currentSensor]=50;		
	if (sonar[currentSensor].check_timer()){
		pingdis[currentSensor]= sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
   Serial.print("measured=");
   Serial.println(pingdis[currentSensor]);
	}
		
}	*/

double calcDir(){

	double x_dis=0;
	double y_dis=0;
	double angle=0;

	for (uint8_t i = 0; i < SONAR_NUM; i++) { 
		x_dis=((1.00/pingdis[i])*cos(((i*60)*1000) / 572)) + x_dis;		//Superposition of all distance vectors
 
		y_dis=((1.00/pingdis[i])*sin(((i*60)*1000) / 572)) + y_dis;
  
	}

	angle=(atan(y_dis/x_dis))* 57296 / 1000;							//Resultant vector dir range = +180 to -180


	if(x_dis<0 && y_dis>0){												//handle second quadrant angle

		angle=angle+180;
	}
	if(x_dis<0 && y_dis<0){												//handle fourth quadrant angle

		angle=angle-180;
	}

  Serial.print("Angle: ");
  Serial.println(angle);
  
	return angle;
}


void move(double angle){												//Move to a desired direction



	if(angle!=0){

																		            Serial.println("Turning");
		stepper1.setCurrentPosition(0);
		stepper2.setCurrentPosition(0);

		 stepper1.setSpeed(200);
		 stepper2.setSpeed(200);
		  
		stepper1.moveTo(30/**angle*/);										//calculate the real number for an arbitary angle
		stepper2.moveTo(-30/**angle*/);
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
		stepper2.moveTo(4096);
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
																			//Wire.begin();
	delay(50);
	lastping=checkdis(millis());

}





void loop() {
  
	lastping=checkdis(lastping);
	angle=calcDir();
	move(angle);
																			/*	Wire.beginTransmission(1); // transmit to device #8
																				Wire.write(angle);        // sends five bytes
																				Wire.endTransmission();    // stop transmitting
																			*/
	delay(2000);

}

