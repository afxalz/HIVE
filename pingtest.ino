#include <NewPing.h>

int trigpin=2;
int echopin=3;

NewPing sonar(trigpin, echopin,200);

void setup() {
Serial.begin(115200);  // put your setup code here, to run once:

}

void loop() {
 delay(50);                      // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int uS = sonar.ping_median(); // Send ping, get ping time in microseconds (uS).
  Serial.print("Ping: ");
  Serial.print(uS / US_ROUNDTRIP_CM); // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial.println("cm"); // put your main code here, to run repeatedly:

}
