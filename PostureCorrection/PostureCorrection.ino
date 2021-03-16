/*
   Created by Steven Kelly, Niall Feehan and Sean Hoffman March 2021. 
   Name: Posture Correcting Chair. 
   Description: Software produced for IoT Group Project 1. 
*/

#include <Bridge.h>
#include <HttpClient.h>

// Add your PushingBox Scenario DeviceID here:
char devid[] = "v17175AEC3026700"; 
char serverName[] = "api.pushingbox.com";
boolean DEBUG = true;

const int BEAMTHRESHOLD = 350;
const int LED = 13;
const int SENSOR = A3;
const int LOUDSPEAKER;
const int buzzer = 3; //Define pin 10, can use other PWM pins  (5,6 or 9)
                       //Note pins 3 and 11 can't be used when using the tone function in Arduino Uno
const int songspeed = 1.5; //Change to 2 for a slower version of the song, the bigger the number the slower the song
const int NUMBEROFNOTES = 5;
int lightSensor;

int eventID = 0;
String status = "";

//Defining note frequency
//*****************************************
#define NOTE_C4  262   
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_A5  880
#define NOTE_B5  988

//************Melody Note sequence**************
int notes[5] = 
{  //Notes of the alert jingle
   NOTE_A5, NOTE_C5, NOTE_D5, NOTE_F5, 0 
};

//************Note Duration*********************
int duration[] = 
{ 
  //duration of each note (in ms) Quarter Note is set to 250 ms
  125, 125, 125, 125, 125 
};

void setup() 
{
   //Start the light beam
   StartBeam();
   //Start bridge and a new serial session
   Initialize(); 
}

void loop() 
{
  // Initialize the client library
  HttpClient client;
  
  bool scan = CheckForBeam();

  Serial.println();
  if(scan)
  {
     status = "LightDetected";
     //Make a HTTP request:  
     String APIRequest;
     APIRequest = String(serverName) + "/pushingbox?devid=" + String(devid)+ "&IDtag=" + eventID++ + "&TimeStamp=50&LightLevel="+ lightSensor + "&Status=" + status;
     client.get (APIRequest);
     //Print the status to serial monitor
     Serial.println(status);
     Serial.println(APIRequest);
     //Print detected value to serial monitor
     Serial.print("Uploaded light value: ");
     Serial.print(lightSensor);
     Serial.println();
     //Play melody when light is detected
     PlayMelody(NUMBEROFNOTES);
  }
  else
  {
    status = "Light_Not_Detected";
    Serial.println(status);
  }
  
  // if there are incoming bytes available
  // from the server, read them and print them:
  while (client.available()) 
  {
    char c = client.read();
  }

  Serial.flush();
  delay(5000);
}

void Initialize()
{
  // Bridge takes about two seconds to start up
  // it can be helpful to use the on-board LED
  // as an indicator for when it has initialized
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Bridge.begin();
  digitalWrite(LED, HIGH);
  Serial.begin(9600);
  // wait for a serial connection
  while (!Serial); //Remove when not connected to PC
}

//Lights the LED
void StartBeam()
{
   digitalWrite(LED, HIGH);
   delay(1000);
   digitalWrite(LED, LOW);
   delay(1000);
   digitalWrite(LED, HIGH);
   delay(400);
   digitalWrite(LED, LOW);
   delay(400);
   digitalWrite(LED, HIGH);
   delay(400);
}

//Uses the light sensor to scan for a lightsource
bool CheckForBeam()
{
  bool isDetected = false;
  //Setup sensorValue to read a value from Analog Port A3
  lightSensor = analogRead(SENSOR);
  if(lightSensor > BEAMTHRESHOLD)
  {
    isDetected = true;
  }
  else
  {
    isDetected = false;
  }
  return isDetected;
}

//Plays a melody over the loudspeaker as an alert.
void PlayMelody(int numberOfNotes)
{
   for (int i=0;i<numberOfNotes;i++)
   {              
      int wait = duration[i] * songspeed;
      tone(buzzer,notes[i],wait);          //tone(pin,frequency,duration)
      delay(wait);                        //delay is used so it doesn't go to the next loop before tone is finished playing
   }                        
}
