#include "MQ135.h"

#include <SoftwareSerial.h>

#define DEBUG true

SoftwareSerial esp8266(9,10);

const int sensorPin= 0;

int air_quality;

#include <LiquidCrystal.h>

LiquidCrystal lcd(12,11, 5, 4, 3, 2);


void setup() {

pinMode(8, OUTPUT);

lcd.begin(16,2);

lcd.setCursor (0,0);

lcd.print ("REES52 ");

lcd.setCursor (0,1);

lcd.print ("Sensor Warming ");

delay(1000);

Serial.begin(9600);

esp8266.begin(9600);

  sendData("AT+RST\r\n",2000,DEBUG); 

  sendData("AT+CWMODE=2\r\n",1000,DEBUG); 

  sendData("AT+CIFSR\r\n",1000,DEBUG); 

  sendData("AT+CIPMUair_quality=1\r\n",1000,DEBUG); 

  sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG);

pinMode(sensorPin, INPUT);        

lcd.clear();

}


void loop() {


MQ135 gasSensor = MQ135(A0);

float air_quality = gasSensor.getPPM();


if(esp8266.available()) 

  {

    if(esp8266.find("+IPD,"))

    {

     delay(1000);

     int connectionId = esp8266.read()-48; 

     String webpage = "<h1>IOT Air Pollution Monitoring System</h1>";

       webpage += "<p><h2>";  

       webpage+= " Air Quality is ";

       webpage+= air_quality;

       webpage+=" PPM";

       webpage += "<p>";

     if (air_quality<=1000)

{

  webpage+= "Fresh Air";

}

else if(air_quality<=2000 && air_quality>=1000)

{

  webpage+= "Poor Air";

}


else if (air_quality>=2000 )

{

webpage+= "Danger! Move to Fresh Air";

}


webpage += "</h2></p></body>";

     String cipSend = "AT+CIPSEND=";

     cipSend += connectionId;

     cipSend += ",";

     cipSend +=webpage.length();

     cipSend +="\r\n";

     

     sendData(cipSend,1000,DEBUG);

     sendData(webpage,1000,DEBUG);

     

     cipSend = "AT+CIPSEND=";

     cipSend += connectionId;

     cipSend += ",";

     cipSend +=webpage.length();

     cipSend +="\r\n";

     

     String closeCommand = "AT+CIPCLOSE=";

     closeCommand+=connectionId; 

     closeCommand+="\r\n";

     

     sendData(closeCommand,3000,DEBUG);

    }

  }


lcd.setCursor (0, 0);

lcd.print ("Air Quality is ");

lcd.print (air_quality);

lcd.print (" PPM ");

lcd.setCursor (0,1);

if (air_quality<=1000)

{

lcd.print("Fresh Air");

digitalWrite(8, LOW);

}

else if( air_quality>=1000 && air_quality<=2000 )

{

lcd.print("Poor Air, Open Windows");

digitalWrite(8, HIGH );

}

else if (air_quality>=2000 )

{

lcd.print("Danger! Move to Fresh Air");

digitalWrite(8, HIGH);   

}

lcd.scrollDisplayLeft();

delay(1000);

}

String sendData(String command, const int timeout, boolean debug)

{

    String response = "";

    esp8266.print(command); 

    long int time = millis();

    while( (time+timeout) > millis())

    {

      while(esp8266.available())

      {

        

        char c = esp8266.read();

        response+=c;

      }  

    }

    if(debug)

    {

      Serial.print(response);

    }

    return response;

}



mq135 configration 


Arduino CodeArduino
#include <LiquidCrystal.h>
int sensorValue;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
void setup(){  lcd.begin(16, 2);
Serial.begin(9600);                            
 }
void loop(){sensorValue = analogRead(0);      
Serial.print("AirQua=");
Serial.print(sensorValue, DEC);               
Serial.println(" PPM");
lcd.setCursor(0,0);
lcd.print("ArQ=");
lcd.print(sensorValue,DEC);
lcd.print(" PPM");
lcd.println("       "); 
lcd.print("  ");
delay(1000);                                   
}
