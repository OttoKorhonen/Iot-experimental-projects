#include <Adafruit_MCP9808.h> //tuodaan kirjastosta adafruitin MCP9808 lämpömittarin ajurit
#include <Wire.h> //tuodaan kirjastosta Wire-ohjainpaketti
#include <WiFi.h>
void setup() {
  Serial.begin(9600);
}

void loop() {
  temperature();
  //soundDetection();
  Serial.println("");
  delay(200);
}

 
void soundDetection() {

  const int sampleWindow = 100; // Sample window width in mS (50 mS = 20Hz)
  unsigned int sample;

   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level
 
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;
 
   // collect data for 50 mS
   while(millis() - startMillis < sampleWindow)
   {
      sample = analogRead(0);
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   //peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   //double volts = (peakToPeak * 5.0) / 1024;  // convert to volts
    Serial.println();
   //Serial.println(volts); //tulostetaan volttien määrä Vr = V2/V1
}

void temperature(){

  Adafruit_MCP9808 temperatureSensor =  Adafruit_MCP9808();
  
  if(!temperatureSensor.begin(0x18)){//0x18 on vakioportti
    Serial.print("Lämpömittaria ei löytynyt, tarkista liitos tai portti!");
  }
  temperatureSensor.setResolution(3);//valitaan sensorille päivitysaika
  
  temperatureSensor.wake();
  float c = temperatureSensor.readTempC();
  if(c >= 28){//ehtolause, joka tarkistaa lämoötilan ja ilmoittaa, jos lämpötila on liian korkea tai matala
    Serial.print("Lämpötila on korkea: "); Serial.println( c, 4);
  }else if(c < 27 && c >20){
    Serial.print("Lämpötila on sopiva: "); Serial.println( c, 4);
  }else if(c < 19 && c > 10){
    Serial.print("Lämpötila on viileä: "); Serial.println( c, 4);
  }else if(c < 9){
    Serial.print("Lämpötila on kylmä"); Serial.println( c, 4);
  }

}

void wifiConnection(){

  const char* ssid = "IidaIida";
  const char* password = "Iidaonparas";

  pinMode(2, OUTPUT);
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(200);
    Serial.println("Yhdistetään WiFiin");
    digitalWrite(2, HIGH);
    delay(200);
    digitalWrite(2, LOW);
  }
  Serial.println("Yhdistetty verkkoon");
  Serial.println(WiFi.localIP());
  digitalWrite(2, HIGH);
}

void vibrationSensor(){
  
  const int switchPin = 13;
  unsigned long  previousTime = 0;
  int switchState = 0;
  int previousSwitchState = 0;
  long timeLimit = 5000;
  unsigned long timeMeasurement = millis();
  int counter = 0;
  
  pinMode(switchPin, INPUT);
  switchState = digitalRead(switchPin);

  if(switchState != previousSwitchState){
    Serial.println("Liikettä");
    previousSwitchState = switchState;
      while(timeMeasurement < timeLimit){
        Serial.println("Aika alkoi");
        if(switchState != previousSwitchState){
          counter+1;
          previousSwitchState = switchState;
          Serial.println(counter);
        }
      }
    }
}
