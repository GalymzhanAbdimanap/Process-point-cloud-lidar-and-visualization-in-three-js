#include <Servo.h>
#include <Wire.h>
#include <TFMini.h>
#include <SoftwareSerial.h>

//Delay between each sample to avoid mechanical wobble
#define DELAY_BETWEEN_SAMPLES 100
//Size of the steps of YAW/PITCH in degrees (1 = full res)
#define YAW_STEP 1
#define PITCH_STEP 1
#define MATH_PI 3.1415f

//Variables
SoftwareSerial mySerial(2, 3);
TFMini tfmini;
Servo servoYaw,servoPitch;
char s[15];
int yawAngle,pitchAngle;
int x,y,z,r;
float theta,phi;
int key=1;

void setup() {
  // Initialize serial connection to display distance readings
  Serial.begin(115200); 

  //Servo init
  servoYaw.attach(10);
  servoPitch.attach(11);

  //Lidar Lite v3 init
  mySerial.begin(TFMINI_BAUDRATE);
  tfmini.begin(&mySerial);
}

void loop() {
  delay(5000);
  //Sweep Yaw servomotor
  for (pitchAngle = 0; pitchAngle <= 90; pitchAngle += PITCH_STEP) {
    servoPitch.write(pitchAngle);

    
    //Sweep Pitch servomotor. The direction depends on the current directory
    if(key==1){
      for (yawAngle = 0; yawAngle <= 180;yawAngle+= YAW_STEP){
          servoYaw.write(yawAngle);
          key=0;
          //delay(100);
          sendMeasurement();
           
      } 
    }
    else{
      for (yawAngle = 180; yawAngle >= 0;yawAngle-= YAW_STEP){
          servoYaw.write(yawAngle);
          key=1;
          //delay(100);
          sendMeasurement();    
      }
    }
  }
}

// Function to acquire, convert and send the measurement.
void sendMeasurement(){
  delay(DELAY_BETWEEN_SAMPLES);
  
  // Get spherical coordinates
  uint16_t r = tfmini.getDistance();
  theta = (float)yawAngle * PI / 180.0f;
  phi = (float)pitchAngle * PI / 180.0f;

  // Convert and send them
  sprintf(s,"%d %d %d\n\0",(int)(r*cos(phi)*cos(theta)),(int)(r*cos(phi)*sin(theta)),(int)(r*sin(phi)));
  Serial.print(s);

}
