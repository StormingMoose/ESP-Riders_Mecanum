/*
  many, many examples were combined to do this

  https://circuits4you.com/2018/11/21/esp32-access-point-station-ap-html-web-server-arduino/
   was particularily informative
  

*/


//#include <Servo.h>
//Servo myservo;                                 
//static const int servoPin = 15;
const int Flight = 18;

int PWM_Speed = 50; // initial speed PWM duty cycle                                 // MAYBE YOU NEED TO CHANGE THIS intitial servo postion to be at 90 degrees to make it straight different from 90 for mechanical adjustment

// wired connections
#define MOTORLR_PWM 33 //  Speed
#define MOTORLR_DIR 32 //  Direction
#define MOTORRR_PWM 25 //  Speed
#define MOTORRR_DIR 26 //  Direction
#define MOTORLF_PWM 27 //  Speed
#define MOTORLF_DIR 14 //  Direction
#define MOTORRF_PWM 12 //  Speed
#define MOTORRF_DIR 13 //  Direction


// the actual values for "fast" and "slow" depend on the motor
#define dir_Forward 0
#define dir_Back 255

#define PWM_STOP 0
//int PWM_Speed 50 // initial speed PWM duty cycle

const int freq = 5000;
const int ledChannelLR_PWM = 10;
const int ledChannelLR_DIR = 11;
const int ledChannelRR_PWM = 12;
const int ledChannelRR_DIR = 13;
const int ledChannelLF_PWM = 14;
const int ledChannelLF_DIR = 15;
const int ledChannelRF_PWM = 8;
const int ledChannelRF_DIR = 9;
const int resolution = 8;

#include <WiFi.h>
#include <WebServer.h>

#include "index.h"  //Web page header file

//Enter your WiFi SSID and PASSWORD

const char* ssid = "ESPRider_Mecanum";               // different name and password?
const char* password = "********";                    // password length minimum 8 char

WebServer server(80);

void setup(void) {

//  pinMode(servoPin, OUTPUT);
//  myservo.attach(servoPin);
//  myservo.write(pos);

  pinMode(Flight, OUTPUT);

  ledcSetup(ledChannelLR_PWM, freq, resolution);
  ledcSetup(ledChannelLR_DIR, freq, resolution);
  ledcSetup(ledChannelRR_PWM, freq, resolution);
  ledcSetup(ledChannelRR_DIR, freq, resolution);
  ledcSetup(ledChannelLF_PWM, freq, resolution);
  ledcSetup(ledChannelLF_DIR, freq, resolution);
  ledcSetup(ledChannelRF_PWM, freq, resolution);
  ledcSetup(ledChannelRF_DIR, freq, resolution);

  pinMode( MOTORLR_DIR, OUTPUT );
  pinMode( MOTORLR_PWM, OUTPUT );
  pinMode( MOTORRR_DIR, OUTPUT );
  pinMode( MOTORRR_PWM, OUTPUT );
  pinMode( MOTORLF_DIR, OUTPUT );
  pinMode( MOTORLF_PWM, OUTPUT );
  pinMode( MOTORRF_DIR, OUTPUT );
  pinMode( MOTORRF_PWM, OUTPUT );

  ledcAttachPin(MOTORLR_PWM, ledChannelLR_PWM);
  ledcAttachPin(MOTORLR_DIR, ledChannelLR_DIR);
  ledcAttachPin(MOTORRR_PWM, ledChannelRR_PWM);
  ledcAttachPin(MOTORRR_DIR, ledChannelRR_DIR);
  ledcAttachPin(MOTORLF_PWM, ledChannelLF_PWM);
  ledcAttachPin(MOTORLF_DIR, ledChannelLF_DIR);
  ledcAttachPin(MOTORRF_PWM, ledChannelRF_PWM);
  ledcAttachPin(MOTORRF_DIR, ledChannelRF_DIR);
  
  ledcWrite( ledChannelLR_DIR, dir_Forward );
  ledcWrite( ledChannelLR_PWM, PWM_STOP );
  ledcWrite( ledChannelRR_DIR, dir_Forward );
  ledcWrite( ledChannelRR_PWM, PWM_STOP );
  ledcWrite( ledChannelLF_DIR, dir_Forward );
  ledcWrite( ledChannelLF_PWM, PWM_STOP );
  ledcWrite( ledChannelRF_DIR, dir_Forward );
  ledcWrite( ledChannelRF_PWM, PWM_STOP );

  digitalWrite(Flight, LOW); //Flashlight will turn off

  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting Sketch...");

  //ESP32 As access point IP: 192.168.4.1
  WiFi.mode(WIFI_AP); //Access Point mode
  WiFi.softAP(ssid, password);    

 
  server.on("/", handle_OnConnect);                      //  commands are sent from html page index.h
  server.on("/A", handle_NW);                  //  what they do is described below
  server.on("/B", handle_Forward);
  server.on("/C", handle_NE);
  server.on("/D", handle_Left);
  server.on("/E", handle_Right);
  server.on("/F", handle_SW);
  server.on("/G", handle_Back);
  server.on("/H", handle_SE);
  server.on("/I", handle_Spin_Left);
  server.on("/J", handle_Spin_Right);
  server.on("/off", handle_Stop);
  
  
  server.on("/FO", handle_Flashlight_Off);
  server.on("/Pos", handle_Speed_Slider_Input);


  server.onNotFound(handle_NotFound);

  server.begin();                  //Start server
  Serial.println("HTTP server started");
}


void loop(void) {
  server.handleClient();

}
void handle_NotFound () {
}


void handle_OnConnect() {

  String s = MAIN_page; //Read HTML contents
  Serial.println("connected:");
  server.send(200, "text/html", s); //Send web page

}

void handle_Speed_Slider_Input() {
  if(server.arg("value") ){
  PWM_Speed = server.arg("value").toInt();   //
  Serial.print("PWM_Speed = ");
  Serial.println(PWM_Speed);
 /* 
 // myservo.write(slidervalue);
  Serial.println(slidervalue);
  pos = slidervalue;
  myservo.write(slidervalue);
*/  }

server.send(200, "text/plain", "ok");
}

void handle_Forward() {
  digitalWrite(Flight, HIGH); // Flash light will turn on
  ledcWrite( ledChannelLR_DIR, dir_Forward ); // direction = forward
  ledcWrite( ledChannelLR_PWM, PWM_Speed ); // PWM speed = fast
  ledcWrite( ledChannelRR_DIR, dir_Forward ); // direction = forward
  ledcWrite( ledChannelRR_PWM, PWM_Speed ); // PWM speed = fast
  ledcWrite( ledChannelLF_DIR, dir_Forward ); // direction = forward
  ledcWrite( ledChannelLF_PWM, PWM_Speed ); // PWM speed = fast
  ledcWrite( ledChannelRF_DIR, dir_Forward ); // direction = forward
  ledcWrite( ledChannelRF_PWM, PWM_Speed ); // PWM speed = fast
  Serial.println("forward");
  server.send(200, "text/plain", "ok");
}

void handle_NW() {

  digitalWrite(Flight, LOW);
  ledcWrite( ledChannelLR_DIR, dir_Forward ); // direction = forward
  ledcWrite( ledChannelLR_PWM, PWM_Speed ); // PWM speed = fast
  ledcWrite( ledChannelRF_DIR, dir_Forward ); // direction = forward
  ledcWrite( ledChannelRF_PWM, PWM_Speed ); // PWM speed = fast

  server.send(200, "text/plain", "ok");
}
void handle_NE() {

  digitalWrite(Flight, LOW);
  ledcWrite( ledChannelLF_DIR, dir_Forward ); // direction = forward
  ledcWrite( ledChannelLF_PWM, PWM_Speed ); // PWM speed = fast
  ledcWrite( ledChannelRR_DIR, dir_Forward ); // direction = forward
  ledcWrite( ledChannelRR_PWM, PWM_Speed ); // PWM speed = fast

  server.send(200, "text/plain", "ok");
}

void handle_Back() {
  digitalWrite(Flight, LOW);
  ledcWrite( ledChannelLR_DIR, dir_Back ); // direction = Back
  ledcWrite( ledChannelLR_PWM, 255 - PWM_Speed ); // PWM speed = Slow  ledChannel_PWM, 180 - PWM_Speed
  ledcWrite( ledChannelRR_DIR, dir_Back  ); 
  ledcWrite( ledChannelRR_PWM, 255 - PWM_Speed ); 
  ledcWrite( ledChannelLF_DIR, dir_Back ); 
  ledcWrite( ledChannelLF_PWM, 255 - PWM_Speed ); 
  ledcWrite( ledChannelRF_DIR, dir_Back ); 
  ledcWrite( ledChannelRF_PWM, 255 - PWM_Speed ); 
  server.send(200, "text/plain", "ok");
}

void handle_SE() {
  digitalWrite(Flight, LOW);
  ledcWrite( ledChannelRF_DIR, dir_Back ); // direction = Back
  ledcWrite( ledChannelRF_PWM, 255 - PWM_Speed ); // PWM speed = Retro
  ledcWrite( ledChannelLR_DIR, dir_Back ); // direction = Back
  ledcWrite( ledChannelLR_PWM, 255 - PWM_Speed ); // PWM speed = Retro
  server.send(200, "text/plain", "ok");
}
void handle_SW() {
  digitalWrite(Flight, LOW);
  ledcWrite( ledChannelLF_DIR, dir_Back ); // direction = Back
  ledcWrite( ledChannelLF_PWM, 255 - PWM_Speed ); // PWM speed = Retro
  ledcWrite( ledChannelRR_DIR, dir_Back ); // direction = Back
  ledcWrite( ledChannelRR_PWM, 255 - PWM_Speed ); // PWM speed = Retro
  server.send(200, "text/plain", "ok");
}

void handle_Stop() {
  digitalWrite(Flight, LOW);
  ledcWrite( ledChannelLR_DIR, dir_Forward);
  ledcWrite( ledChannelLR_PWM, PWM_STOP );
  ledcWrite( ledChannelRR_DIR, dir_Forward ); 
  ledcWrite( ledChannelRR_PWM, PWM_STOP );  
  ledcWrite( ledChannelLF_DIR, dir_Forward ); 
  ledcWrite( ledChannelLF_PWM, PWM_STOP );
  ledcWrite( ledChannelRF_DIR, dir_Forward ); 
  ledcWrite( ledChannelRF_PWM, PWM_STOP ); 
  server.send(200, "text/plain", "ok");
}

void handle_Right() { 
  ledcWrite( ledChannelLR_DIR, dir_Back ); // direction = forward or back
  ledcWrite( ledChannelLR_PWM, 255 - PWM_Speed ); // PWM speed = fast or slow
  ledcWrite( ledChannelRR_DIR, dir_Forward ); 
  ledcWrite( ledChannelRR_PWM, PWM_Speed ); 
  ledcWrite( ledChannelLF_DIR, dir_Forward ); 
  ledcWrite( ledChannelLF_PWM, PWM_Speed ); 
  ledcWrite( ledChannelRF_DIR, dir_Back ); 
  ledcWrite( ledChannelRF_PWM, 255 - PWM_Speed ); 
  server.send(200, "text/plain", "ok");
}

void handle_Left() { 
  ledcWrite( ledChannelLR_DIR, dir_Forward ); // direction = forward or back
  ledcWrite( ledChannelLR_PWM, PWM_Speed ); // PWM speed = fast or slow
  ledcWrite( ledChannelRR_DIR, dir_Back ); 
  ledcWrite( ledChannelRR_PWM, 255 - PWM_Speed ); 
  ledcWrite( ledChannelLF_DIR, dir_Back ); 
  ledcWrite( ledChannelLF_PWM, 255 - PWM_Speed ); 
  ledcWrite( ledChannelRF_DIR, dir_Forward ); 
  ledcWrite( ledChannelRF_PWM, PWM_Speed ); 

  server.send(200, "text/plain", "ok");
}
void handle_Spin_Left() { 
  ledcWrite( ledChannelRR_DIR, dir_Forward ); // direction = forward or back
  ledcWrite( ledChannelRR_PWM, PWM_Speed ); // PWM speed = fast or slow
  ledcWrite( ledChannelLR_DIR, dir_Back ); 
  ledcWrite( ledChannelLR_PWM, 255 - PWM_Speed ); 
  ledcWrite( ledChannelLF_DIR, dir_Back ); 
  ledcWrite( ledChannelLF_PWM, 255 - PWM_Speed ); 
  ledcWrite( ledChannelRF_DIR, dir_Forward ); 
  ledcWrite( ledChannelRF_PWM, PWM_Speed ); 

  server.send(200, "text/plain", "ok");
}
void handle_Spin_Right() { 
  ledcWrite( ledChannelLR_DIR, dir_Forward ); // direction = forward or back
  ledcWrite( ledChannelLR_PWM, PWM_Speed ); // PWM speed = fast or slow
  ledcWrite( ledChannelRR_DIR, dir_Back ); 
  ledcWrite( ledChannelRR_PWM, 255 - PWM_Speed ); 
  ledcWrite( ledChannelRF_DIR, dir_Back ); 
  ledcWrite( ledChannelRF_PWM, 255 - PWM_Speed ); 
  ledcWrite( ledChannelLF_DIR, dir_Forward ); 
  ledcWrite( ledChannelLF_PWM, PWM_Speed ); 

  server.send(200, "text/plain", "ok");
}

void handle_Flashlight_Off() {
  digitalWrite(Flight, LOW);
  server.send(200, "text/plain", "ok");
}
