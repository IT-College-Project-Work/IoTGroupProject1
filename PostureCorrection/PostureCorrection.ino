/*
   Created by Steven Kelly, Niall Feehan and Sean Hoffman March 2021. 
   Name: Posture Correcting Chair. 
   Description: Software produced for IoT Group Project 1. 
*/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <Bridge.h>
#include <HttpClient.h>
#include <BlynkSimpleYun.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YCuTWes0epB1FcnpqoJiwtRzc2L1K_eE";

// Add your PushingBox Scenario DeviceID here:
char devid[] = "v17175AEC3026700"; 
char serverName[] = "api.pushingbox.com";
boolean DEBUG = true;

int beamThreshold = 100;    //This will now be adjustable via a slider in the Blynk App 
const int LED = 13;
const int SENSOR = A3;
const int buzzer = 3;             //Define pin 10, can use other PWM pins  (5,6 or 9)
                                  //Note pins 3 and 11 can't be used when using the tone function in Arduino Uno
const int songspeed = 1.5;        //Change to 2 for a slower version of the song, the bigger the number the slower the song
const int NUMBEROFNOTES = 5;
int lightSensor;
unsigned long interval;                     //represents the interval between recorded events in milliseconds. 
int melody = 1;

int eventID = 0;
String status = "";

//Defining note frequency
//*****************************************
#define NOTE_C0  16.35
#define NOTE_Db0 17.32
#define NOTE_D0  18.35
#define NOTE_Eb0 19.45
#define NOTE_E0  20.60
#define NOTE_F0  21.83
#define NOTE_Gb0 23.12
#define NOTE_G0  24.50
#define NOTE_Ab0 25.96
#define NOTE_LA0 27.50
#define NOTE_Bb0 29.14
#define NOTE_B0  30.87
#define NOTE_C1  32.70
#define NOTE_Db1 34.65
#define NOTE_D1  36.71
#define NOTE_Eb1 38.89
#define NOTE_E1  41.20
#define NOTE_F1  43.65
#define NOTE_Gb1 46.25
#define NOTE_G1  49.00
#define NOTE_Ab1 51.91
#define NOTE_LA1 55.00
#define NOTE_Bb1 58.27
#define NOTE_B1  61.74
#define NOTE_C2  65.41
#define NOTE_Db2 69.30
#define NOTE_D2  73.42
#define NOTE_Eb2 77.78
#define NOTE_E2  82.41
#define NOTE_F2  87.31
#define NOTE_Gb2 92.50
#define NOTE_G2  98.00
#define NOTE_Ab2 103.83
#define NOTE_LA2 110.00
#define NOTE_Bb2 116.54
#define NOTE_B2  123.47
#define NOTE_C3  130.81
#define NOTE_Db3 138.59
#define NOTE_D3  146.83
#define NOTE_Eb3 155.56
#define NOTE_E3  164.81
#define NOTE_F3  174.61
#define NOTE_Gb3 185.00
#define NOTE_G3  196.00
#define NOTE_Ab3 207.65
#define NOTE_LA3 220.00
#define NOTE_Bb3 233.08
#define NOTE_B3  246.94
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

//************Melody1 Note sequence**************
int melody1[5] = 
{  //Notes of the alert jingle
   NOTE_A5, NOTE_C5, NOTE_D5, NOTE_F5, 0 
};
//************Melody1 Note sequence**************
int melody2[5] = 
{  //Notes of the alert jingle
   NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5, 0 
};
//************Melody3 Note sequence**************
int melody3[5] = 
{  //Notes of the alert jingle
   NOTE_A5, NOTE_G5, NOTE_D5, NOTE_E5, 0 
};
//************Melody4 Note sequence**************
int melody4[5] = 
{  //Notes of the alert jingle
   NOTE_D4, NOTE_E4, NOTE_G4, NOTE_B4, 0 
};
//************Melody4 Note sequence**************
int melody5[5] = 
{  //Notes of the alert jingle
   NOTE_G4, NOTE_G4, NOTE_G4, NOTE_Eb3, 0 
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
   //StartBeam();
   //Start bridge and a new serial session
   Initialize(); 
}

void loop() 
{
  interval = millis();
  
  Blynk.run();
  
  // Initialize the client library
  HttpClient client;
  
  bool scan = CheckForBeam();

  Serial.println();
  if(scan)
  {
     status = "LightDetected";
     //update the interval field
     interval = millis();
     //Make a HTTP request:  
     String APIRequest;
     APIRequest = String(serverName) + "/pushingbox?devid=" + String(devid)+ "&IDtag=" + eventID++ + "&TimeStamp="+ interval + "&LightLevel="+ lightSensor + "&Status=" + status;
     client.get (APIRequest);
     //Print the status to serial monitor
     Serial.println(status);
     Serial.println(APIRequest);
     //Print detected value to serial monitor
     Serial.print("Uploaded light value: ");
     Serial.print(lightSensor);
     Serial.println();

     //Check the selected melody and trigger playback
     if(melody == 1)
     {
       //Play melody when light is detected
        PlayMelody(NUMBEROFNOTES, melody1);
     }
     if(melody == 2)
     {
        //Play melody when light is detected
        PlayMelody(NUMBEROFNOTES, melody2);
     }
     if(melody == 3)
     {
        //Play melody when light is detected
        PlayMelody(NUMBEROFNOTES, melody3);
     }
     if(melody == 4)
     {
        //Play melody when light is detected
        PlayMelody(NUMBEROFNOTES, melody4);
     }
     if(melody == 5)
     {
        //Play melody when light is detected
        PlayMelody(NUMBEROFNOTES, melody5);
     }
  }
  else
  {
    status = "Light_Not_Detected";
    Serial.println(status);
    Serial.println("Light Sensor value:" + lightSensor);
  }
  
  // if there are incoming bytes available
  // from the server, read them and print them:
  while (client.available()) 
  {
    char c = client.read();
  }

  SendStatusToBlynk(status);

  Serial.flush();
  delay(1000);
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
  //Initialize Blynk instance
  Blynk.begin(auth);
  // You can also specify server:
  //Blynk.begin(auth, "blynk-cloud.com", 80);
  //Blynk.begin(auth, IPAddress(192,168,1,100), 8080);
}

//Lights the LED
void StartBeam()
{
   //Play simple light sequence
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
  if(lightSensor > beamThreshold)
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
void PlayMelody(int numberOfNotes, int melody[])
{
   for (int i=0;i<numberOfNotes;i++)
   {              
      int wait = duration[i] * songspeed;
      tone(buzzer,melody[i],wait);          //tone(pin,frequency,duration)
      delay(wait);                        //delay is used so it doesn't go to the next loop before tone is finished playing
   }                        
}

//Get beamThreshold value via a virtual pin sent via Blynk
BLYNK_WRITE(V30) 
{
  //Set the Beam Threshold to whatever the slider value is
  beamThreshold = param[0].asInt();
  //Display New value of slider via the console
  Serial.print("BeamThreshold is now = ");
  Serial.print(beamThreshold);
}

//Get melody value via a virtual pin sent via Blynk
BLYNK_WRITE(V31) 
{
   //Set the Melody to whatever the new value send via Blynk is:
   melody = param[0].asInt();                                                                                                                               
  //Display New value of melody via the console
  Serial.print("Melody is now set to = ");
  Serial.print(melody);
}

//Send Current status to the Blynk control panel
void SendStatusToBlynk(String status)
{
  Blynk.virtualWrite(V15, status);
}
