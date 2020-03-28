#include <MPU6050_tockn.h>
#include <Wire.h>
#define I2C_SCL 21//määritellään mitä portteja lämpömittari ja gyroskooppi käyttää
#define I2C_SDA 22

int previousX = -1;//edelliset mittausarvot
int previousY  = -1;
int previousZ  = -1;

int currentX  = -1;//uudet mittausarvot
int currentY  = -1;
int currentZ  = -1;

void motionSensor(){
 previousX = currentX;//päivitetään edelliset mittausarvot uusiin
 previousY = currentY;
 previousZ = currentZ;
 
 mpu6050.update();//haetaan uudet arvot  mpu6050
 currentX = mpu6050.getAngleX();//asetetaan X, Y ja Z kulmille uudet arvot
 currentY = mpu6050.getAngleY();
 currentZ = mpu6050.getAngleZ();
 
 if(previousX != -1 || previousY != -1 || previousZ != -1){// tässä verrataan, että aikaisempi X:n arvo ei ole vanha previousX
 // Serial.println("Ei -1");
  int difX = abs(previousX - currentX);
  //Serial.print("X:n erotus "); Serial.println(difX);
  int difY = abs(previousY - currentY);
  //Serial.print("Y:n erotus ");Serial.println(difY);
  int difZ = abs(previousZ - currentZ);
  //Serial.print("Z:n erotus ");Serial.println(difZ);

  int difTotal = difX + difY + difZ;//lasketaan yhteen X, Y ja Z kulmien summa
  //Serial.println(difTotal);
  //unsigned long timer = millis();
 
  if(difTotal > 10){
    Serial.println("Liikettä");
    counter++;
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Laskuri: "); Serial.println(counter);
    delay(500);
    if(counter >= 5){
      msg="Itkuhälytin ilmoitus! Havaittu jatkuvaa liikettä";
      sendMessage();
      delay(100000);
    }
 }
 if((unsigned long)(millis() - previousMillis) >= interval && counter < 5){
      counter = 0;
      Serial.println("Laskuri on nollattu");
      previousMillis = currentMillis;
      delay(500);
      resetFunc();//kutsutaan uudelleenkäynnistysfunktiota
    }
}
}
