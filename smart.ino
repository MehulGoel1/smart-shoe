#include "TinyGPS++.h"
#include "SoftwareSerial.h"

SoftwareSerial serial_connection(10, 11); //RX=pin 10, TX=pin 11
TinyGPSPlus gps;

float threshhold = 80.0;
const int trigPin = 2;
const int echoPin = 4;
const int buzzer = 6;

long duration;
int distance;
int safetyDistance;



const int xpin=A2;
const int ypin=A3;
const int zpin=A4;

float xval[100] = { 0 };
float yval[100] = { 0 };
float zval[100] = { 0 };
float xavg;
float yavg;
float zavg;
float calories;
int steps,mem=0, flag = 0,cnt=0,google=0;

void setup() 
{
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
pinMode(buzzer, OUTPUT);
Serial.begin(9600); // Starts the serial communication
serial_connection.begin(9600);

calibrate();
 
}

void loop()
{  
    int acc = 0;
    float avg=0;
    float totvect[100] = { 0 };
    float totave[100] = { 0 };
    float xaccl[100] = { 0 };
    float yaccl[100] = { 0 };
    float zaccl[100] = { 0 };

    cnt++;
  
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      
      // Sets the trigPin on HIGH state for 10 micro seconds
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      
      // Reads the echoPin, returns the sound wave travel time in microseconds
      duration = pulseIn(echoPin, HIGH);
      
      // Calculating the distance
      distance= duration*0.034/2;
      
      safetyDistance = distance;
      if (safetyDistance <= 29){
        digitalWrite(buzzer, HIGH);
      
      }
      else{
        digitalWrite(buzzer, LOW);
      }
     
        
    for (int i = 0; i < 5; i++) 
    {
        xaccl[i] = float(analogRead(xpin));
        delay(1);
        yaccl[i] = float(analogRead(ypin));
        delay(1);
        zaccl[i] = float(analogRead(zpin));
        delay(1);
        totvect[i] = sqrt(((xaccl[i] - xavg) * (xaccl[i] - xavg)) + ((yaccl[i] - yavg) * (yaccl[i] - yavg)) + ((zval[i] - zavg) * (zval[i] - zavg)));
        totave[i] = (totvect[i] + totvect[i - 1]) / 2;
     //  Serial.print("Tot Ave : ");
         avg=totave[i];
        
        
        
        if (totave[i] > threshhold && flag == 0) 
        {
            steps = steps + 1;
            flag = 1;
            //Serial.println(totave[i]);
        }
        else if (totave[i] > threshhold && flag == 1) {
        }
        if (totave[i] < threshhold && flag == 1) 
        {
            flag = 0;
        }
    }
    
     

      if(google==0)
      {
        while(serial_connection.available())//While there are characters to come from the GPS
      
          {
            
            gps.encode(serial_connection.read());//This feeds the serial NMEA data into the library one char at a time
          }
        if(gps.location.isUpdated())    //This will pretty much be fired all the time anyway but will at least reduce it to only after a package of NMEA data comes in
          {
            //Get the latest info from the gps object which it derived from the data sent by the GPS unit
            Serial.println("Satellite Count:");
            Serial.println(gps.satellites.value());
            Serial.println("Latitude:");
            Serial.println(gps.location.lat(), 6);
            Serial.println("Longitude:");
            Serial.println(gps.location.lng(), 6);
            google=1;
            Serial.println("Speed MPH:");
            Serial.println(gps.speed.mph());
            Serial.println("Altitude Feet:");
            Serial.println(gps.altitude.feet());

            
       
      }
            
  }
          
       
       if(mem != steps)
       {
            Serial.println(avg); 
             mem=steps;
             Serial.println(steps/3);
          
        }
          
  
       
       
    
     //     }
  
     
}




void calibrate()
{
    digitalWrite(13, HIGH);
    float sum = 0;
    float sum1 = 0;
    float sum2 = 0;
    for (int i = 0; i < 100; i++) {
        xval[i] = float(analogRead(xpin));

        sum = xval[i] + sum;
    }
    delay(100);
    xavg = sum / 100.0;


    for (int j = 0; j < 100; j++) {
        xval[j] = float(analogRead(xpin));

        sum1 = xval[j] + sum1;
    }
    yavg = sum1 / 100.0;


    delay(100);
    for (int i = 0; i < 100; i++) {
        zval[i] = float(analogRead(zpin));

        sum2 = zval[i] + sum2;
    }
    zavg = sum2 / 100.0;
    delay(100);
    
}
