#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include"moto.h";

const char *ssid = "edmi 4";      //change ssid
const char *password = "12345678"; //change Password
const byte pinBuiltinLed PROGMEM = D5;

unsigned int localPort = 8888;  //Port from which we receive UDP packets
const int NTP_PACKET_SIZE = 48; // Buffer size for received packets
char packetBuffer[ NTP_PACKET_SIZE]; //Packet storage buffer

//connecting the engine driver L298N
#define L9110_A_IA D1 // Pin D12 --> output int1 to motor "A"
#define L9110_A_IB D2 // Pin D11 --> output int2 to motor "А"
#define L9110_B_IA D3 // Pin D10 --> output  int3 to motor "В"
#define L9110_B_IB D4 // Pin D9 --> output  int4 to motor "В"
 
// which contacts are responsible for what
#define MOTOR_A_PWM L9110_A_IA // control the speed of motors "A"
#define MOTOR_A_DIR L9110_A_IB // motor power
#define MOTOR_B_PWM L9110_B_IA // control the speed of motors "В"
#define MOTOR_B_DIR L9110_B_IB // motor power


WiFiUDP udp;

int t = 0;
int rnd;
int sc = 1; //operating mode 1-joystick 0-accelerometer
int xx = 0; //speed coefficient

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  pinMode(pinBuiltinLed, OUTPUT);
  // We start operations with WIFI
  Serial.print("Configuring access point...");
  Serial.println(ssid);
WiFi.mode(WIFI_OFF); //turn off WIFI
WiFi.mode(WIFI_AP);  //Put WIFI In Access Point Mode
WiFi.softAP(ssid, password,11,0); //Run the access point with the given ssid and password on channel 11 (do not interfere with others)

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  Serial.println("HTTP server started");
  Serial.println("Starting UDP");
  udp.begin(localPort); // initialize UDP
  Serial.print("Local port: ");
  Serial.println(udp.localPort());

pinMode( D0, OUTPUT );
pinMode( D5, OUTPUT );
pinMode( D6, OUTPUT );
pinMode( D7, OUTPUT );

 pinMode( MOTOR_A_DIR, OUTPUT ); //-----------------------------------------//
 pinMode( MOTOR_A_PWM, OUTPUT ); //                                         //
 pinMode( MOTOR_B_DIR, OUTPUT ); //    pins to EXIT mode and stop motors    //
 pinMode( MOTOR_B_PWM, OUTPUT ); //                                         //
 fstop(10);                      //-----------------------------------------//
}

void loop()
{
    int cb = udp.parsePacket(); 
  if (!cb) {}
  else {
    udp.read(packetBuffer, NTP_PACKET_SIZE);
    Serial.println(packetBuffer);
    String  req=(packetBuffer);

//-------------------------------------------------------------------------------------------//
//                                                                                           //
//                   This is the main cycle of our program !!!                               //
//                                                                                           //
//-------------------------------------------------------------------------------------------//    

// If the robot is in joystick control then ...
if (req.indexOf("fforward") != -1){aforwad();}
if (req.indexOf("fback") != -1){aback();}
if (req.indexOf("fleft") != -1){aright();}
if (req.indexOf("fright") != -1){aleft();}
if (req.indexOf("stop") != -1){astop();}
if (req.indexOf("gy") != -1){sc=0;}

if (sc==0) {  //If the robot is in accelerometer control mode, then ..
  if (req.indexOf("bm") != -1) xx = 3; // high speed
  if (req.indexOf("sm") != -1) xx = 2; //average speed
  if (req.indexOf("tm") != -1) xx = 1; //slow speed
  if (req.indexOf("m") == -1)  {astop(); xx = 0;} //Stop



  if (req.indexOf("zm") != -1) { aback(); } // reverse

  if (req.indexOf("aforwad") != -1)aforwad();
  if (req.indexOf("aback") != -1)aback();
  if (req.indexOf("aleft") != -1)aleft();
  if (req.indexOf("aright") != -1)aright();
  if (req.indexOf("astop") != -1)astop();

  xx = 0; 
}

memset(packetBuffer, 0, NTP_PACKET_SIZE);  //clear the buffer to receive the next command
  xx = 0; 
}
//-------------------------------------------------------------------------------------------//
//                                                                                           //
//                        End our main loop !!!                                              //
//                                                                                           //
//-------------------------------------------------------------------------------------------//
}


//-----Below we create simplified commands for controlling the robot----//


//***************************************************
void move(int motor, int speed, int rotate){
//create a motion function with
//three parameters (motors, speed, direction of rotation)
// motors - 1 or 0
// speed - from 0 to 255
// direction of rotation - 1 or 0

  boolean inPin1 = LOW;
  
  if(rotate == 1){
    inPin1 = HIGH;
    
  if(motor == 1){
    digitalWrite(MOTOR_A_DIR, inPin1);
    analogWrite(MOTOR_A_PWM, 1023-speed);
          }else{
            digitalWrite(MOTOR_B_DIR, inPin1);
            analogWrite(MOTOR_B_PWM, 1023-speed);
          }    
    }
    
  else{
    inPin1 = LOW;
  
  if(motor == 1){
    digitalWrite(MOTOR_A_DIR, inPin1);
    analogWrite(MOTOR_A_PWM, 1023-(1023-speed));
          }else{
            digitalWrite(MOTOR_B_DIR, inPin1);
            analogWrite(MOTOR_B_PWM, 1023-(1023-speed));
          }  
 }
 
}
//***************************************************
void fstop(int t){  //create the stop function

  move(1,0,0);
  move(2,0,0);
 delay(t);
}

//***************************************************
void forward(int s, int time){//create the "forward" function
  move(1,s,0);
  move(2,s,0);
  delay(time);
}
//***************************************************
void fforward(int s){  //create the function "move forward without a time limit"
  move(1,s,0);
  move(2,s,0);
}
//***************************************************
void back(int s, int time){ //create the "backward" function
  move(1,s,1);
  move(2,s,1);
  delay(time);
}
//***************************************************
void fback(int s){ //create the function "move backward without a time limit"
  move(1,s,1);
  move(2,s,1);
}
//***************************************************
void left(int s, int time){ //create the function "turn right"
  move(1,s,1);
  move(2,s,0);
  delay(time);
}
//***************************************************
void fleft(int s){ //create the function "turn right without time limit"
  move(1,s,1);
  move(2,s,0);
}
//***************************************************
void right(int s, int time){ //create the function "turn left"
  move(1,s,0);
  move(2,s,1);
  delay(time);
}
//***************************************************
void fright(int s){ //create the function "turn left without time limit"
  move(1,s,0);
  move(2,s,1);
}
//***************************************************
void iforward(int sl, int sr){//create the function "move forward without a time limit"
  move(1,sl,0);
  move(2,sr,0);
}
