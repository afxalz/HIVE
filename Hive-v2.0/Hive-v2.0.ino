#include <NewPing.h>
#include <AccelStepper.h>
#include <math.h>
                                                               				 //#include <Wire.h>

#define SONAR_NUM     6 // Number or sensors.						//Macros for ping
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.
#define MIN_DISTANCE 7    // Minimum distance (in cm) to ping.

unsigned int pingdis[6]={2,2,2,2,2,2},lastping=0;					// Where the ping distances are stored.
uint8_t currentSensor = 0;
double angle=0;   

      

NewPing sonar[SONAR_NUM] = {										// Sensor object array.
  NewPing(10,11, MAX_DISTANCE),//0  								// Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(A2,A3, MAX_DISTANCE),//1           
  NewPing(A4,A5, MAX_DISTANCE),//2           
  NewPing(6,7, MAX_DISTANCE),//3
  NewPing(8,9, MAX_DISTANCE),//4
  NewPing(0,1, MAX_DISTANCE),//5           
  };

#define HALFSTEP 8

// Motor pin definitions
#define motorPin1  2   												 // IN1 on the ULN2003 driver 1
#define motorPin2  3   												  // IN2 on the ULN2003 driver 1
#define motorPin3  4    											 // IN3 on the ULN2003 driver 1
#define motorPin4  5   												  // IN4 on the ULN2003 driver 1

#define motorPin5  A1    											 // IN1 on the ULN2003 driver 2
#define motorPin6  A0   											 // IN2 on the ULN2003 driver 2
#define motorPin7  13    											// IN3 on the ULN2003 driver 2
#define motorPin8  12   											// IN4 on the ULN2003 driver 2


																	// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48

AccelStepper stepper1(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);
AccelStepper stepper2(HALFSTEP, motorPin5, motorPin7, motorPin6, motorPin8);



void Stopbot(){
  
}











int checkdis(int last_ping){

	
	for (uint8_t i = 0; i < SONAR_NUM; i++) {      					 // Loop through all the sensors.
		
		Serial.println("sensing");
		while((millis()-last_ping)<=50);
		currentSensor = i;                        					  // Sensor being accessed.
		pingdis[currentSensor]=sonar[currentSensor].ping_cm();
		
		if(pingdis[currentSensor]==0){
	    	pingdis[currentSensor]=200;
   		}
																
	}                                                

 

	Serial.println("Distances measured by sensors: ");
 
	for(uint8_t i = 0; i<SONAR_NUM; i++) {
		Serial.print(i);
		Serial.print("-----");
		Serial.println(pingdis[i]);
 	}

 
	return millis();  
}



double calcDir(){

	double x_dis=0;													//Initialising variables
	double y_dis=0;
	double angle=0;

	for (uint8_t i = 0; i < SONAR_NUM; i++) { 
	
		x_dis=((1.00/pingdis[i])*cos(i*1.0472)) + x_dis;			//Superposition of all distance vectors
    	y_dis=((1.00/pingdis[i])*sin(i*1.0472)) + y_dis;			//1.0472= 60 * rad(1)
  
	}

	angle=(atan(y_dis/x_dis))* 57296 / 1000;						//Resultant vector dir range = +90 to 90

	if(x_dis<0 && y_dis>0){											//handle second quadrant angle-- range 0 to +180

		angle=angle+180;
	}
	if(x_dis<0 && y_dis<0){											//handle fourth quadrant angle-- range 0 to -180

		angle=angle-180;
	}

	Serial.print("Angle: ");
	Serial.println(angle);
  
	return angle;
}


void move(float dir){											  			//Move in the desired direction

	stepper1.setCurrentPosition(0);									//Set current position to zero
	stepper2.setCurrentPosition(0);

	stepper1.setMaxSpeed(1000.0);									//Set maxspeed achievable
    stepper2.setMaxSpeed(1000.0);

    stepper1.setAcceleration(1000.0);								//Set the Acceleration
    stepper2.setAcceleration(1000.0);

	stepper1.setSpeed(400);											//Set initial speed
	stepper2.setSpeed(400);


	stepper1.moveTo(dir*2048);											//Assign the no. of turns to move
	stepper2.moveTo(dir*-2048);		
	
	Serial.println("Moving forward...");
	
	while(stepper1.distanceToGo() != 0&&stepper2.distanceToGo() != 0){					//check if the no. of turns achieved

		stepper1.run();
		stepper2.run(); 
	}
	
	Serial.println("Motion Successful!!");
		
}


void turn(double angle){

	if(!(angle>=(-30)&&angle<=30)){                       			 // check angle range -30 to +30 for forward movement unless turn

	    Serial.println("Initiating turn..");
	    
	    stepper1.setCurrentPosition(0);									//Set current position to zero
	    stepper2.setCurrentPosition(0);

	    stepper1.setMaxSpeed(1000.0);									//Set maxspeed achievable
	    stepper2.setMaxSpeed(1000.0);

	    stepper1.setAcceleration(1000.0);								//Set acceleration
	    stepper2.setAcceleration(1000.0);
	    
	    stepper1.setSpeed(400);											//Set initial speed
	    stepper2.setSpeed(400);
	      
	    stepper1.moveTo(int(6.82*angle));                			   //calculate the no. of turns from an arbitary angle
	    stepper2.moveTo(int(6.82*angle));
	    
	    Serial.println("Turning...");

	    while(stepper1.distanceToGo() != 0&&stepper2.distanceToGo() != 0){		//check if the no. of turns achieved
	    	stepper1.run();
	    	stepper2.run();
	      
	    }
	    
	    Serial.println("Turned!!");
	}
	
	else{
    
    Serial.println("Did not turn!!");									//Report did not turn
	
	}
}



void setup() {

	Serial.begin(115200);
																					//Wire.begin();
	delay(50);
	lastping=checkdis(millis());										//Initialse search

}





void loop() {
  
	lastping=checkdis(lastping);
	
	
	if(pingdis[0]<=MIN_DISTANCE &&  pingdis[1]<=MIN_DISTANCE && pingdis[5]<=MIN_DISTANCE )
 {
  stepper1.stop();                  //Set current position to zero
  stepper2.stop();
   
 }
	else{
  if(pingdis[0]<=MIN_DISTANCE){

    move(-2);
    turn(90);
    }
  
	
	else{
	angle=calcDir();													//Get the right direction to move
	turn(angle);														//See if a turn is required
	move(0.5);		
	}														//Move in the found direction

	}
																			/*	Wire.beginTransmission(1); // transmit to device #8
																				Wire.write(angle);        // sends five bytes
																				Wire.endTransmission();    // stop transmitting
																			*/
																			
	delay(10);

}



