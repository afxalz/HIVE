#include <NewPing.h>
#include <math.h>
#include <Wire.h>

#define SONAR_NUM     6 // Number or sensors.
#define MAX_DISTANCE 50 // Maximum distance (in cm) to ping.

unsigned int pingdis[6]={50,50,50,50,50,50},lastping=0;         // Where the ping distances are stored.
uint8_t currentSensor = 0;
double angle=0;   

      

NewPing sonar[SONAR_NUM] = {     // Sensor object array.
  NewPing(10, 11, MAX_DISTANCE),//0    // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(12, 13, MAX_DISTANCE),//1           //
  NewPing(A0, A1, MAX_DISTANCE),//2           
  NewPing(52, 53, MAX_DISTANCE),//3
  NewPing(50, 51, MAX_DISTANCE),//4
  NewPing(48, 49, MAX_DISTANCE),//5           
  };


int checkdis(int lastping){

	
	for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop through all the sensors.
		
		while((millis()-lastping)<=35);
		
		sonar[currentSensor].timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
		currentSensor = i;                          // Sensor being accessed.
		sonar[currentSensor].ping_timer(echoCheck);
	}
	
	return millis();  
}



void echoCheck() { // If ping received, set the sensor distance to array.

	pingdis[currentSensor]=50;		
	if (sonar[currentSensor].check_timer())
		dis= sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
	
}	

double calcDir(){

	double x_dis=0;
	double y_dis=0;
	double angle=0;
	for (uint8_t i = 0; i < SONAR_NUM; i++) { 
		x_dis=(1/pingdis[i])*cos(((i*60)*1000) / 57296) + x_dis;
		y_dis=(1/pingdis[i])*sin(((i*60)*1000) / 57296) + y_dis;
	}

	angle=(atan(y_dis/x_dis)* 57296) / 1000;

	if(x_dis<0 && y_dis>0){

		angle=angle+180;
	}
	if(x_dis<0 && y_dis<0){

		angle=angle-180;
	}

	return angle;
}


void setup() {

	Wire.begin();
	deay(50);
	lastping=checkdis(millis());

}


void loop() {
  
	lastping=checkdir(lastping);
	angle=calcDir();
	Wire.beginTransmission(1); // transmit to device #8
	Wire.write(angle);        // sends five bytes
	Wire.endTransmission();    // stop transmitting

	delay(500);

}

