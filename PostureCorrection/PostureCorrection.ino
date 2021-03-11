/*
   Created by Steven Kelly, Niall Feehan and Sean Hoffman March 2021. 
   Name: Posture Correcting Chair. 
   Description: Software produced for IoT Group Project1. 
*/

#include <Bridge.h>
#include <HttpClient.h>

// Add your PushingBox Scenario DeviceID here:
char devid[] = "v135BADAB42391E0";
char serverName[] = "api.pushingbox.com";
boolean DEBUG = true;

const int LED = 13;
const int LOUDSPEAKER;
int lightSensor;

void setup() {
  // Bridge takes about two seconds to start up
  // it can be helpful to use the on-board LED
  // as an indicator for when it has initialized
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Bridge.begin();
  digitalWrite(LED, HIGH);

  Serial.begin(9600);
  while (!Serial); // wait for a serial connection
}

void loop() 
{
  // Initialize the client library
  HttpClient client;

  //Setup sensorValue to read a value from Analog Port A3
  lightSensor = analogRead(A3);
  Serial.print(lightSensor);

  // Make a HTTP request:  
  String APIRequest;
  APIRequest = String(serverName) + "/pushingbox?devid=" + String(devid)+ "&IDtag=100&TimeStamp=50&TempC="+ lightSensor;
  client.get (APIRequest);
  
  // if there are incoming bytes available
  // from the server, read them and print them:
  while (client.available()) 
  {
    char c = client.read();
  }

  Serial.flush();
  String UploadMessage;
  Serial.print("\n Uploaded temp value: ");
  Serial.print(lightSensor);
  delay(5000);

}

void Initialize()
{
  
}

//Plays a sound over the loudspeaker as an alert.
void PlaySound()
{
   for (int i=0;i<203;i++){              //203 is the total number of music notes in the song
  int wait = duration[i] * songspeed;
  tone(buzzer,notes[i],wait);          //tone(pin,frequency,duration)
  delay(wait);}                        //delay is used so it doesn't go to the next loop before tone is finished playing
  //You can click reset on Arduino to replay the song
}
