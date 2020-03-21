#include <MPU6050_tockn.h>
#include <Wire.h>
#define I2C_SCL 21//määritellään mitä portteja lämpömittari ja gyroskooppi käyttää
#define I2C_SDA 22
MPU6050 mpu6050(Wire);

void setup() {
  Serial.begin(9600);
  Wire.begin(I2C_SDA, I2C_SCL);
  mpu6050.begin();
  mpu6050.calcGyroOffsets(false);
}
void loop(){
 
}

int settingComparisonValues() {//asetetaanko tähän parametriarvot, jotka lähetetään toiseen muuttujaan

 int i = 0;
 int cX;//vertailumuuttujat esim cX "comparisonX"
 int cY;
 int cZ;
 
  Serial.println("======================");
  Serial.println("Itkuhälytin käynnistyy");
  Serial.println("======================");
  delay(2000);
  while(i < 1000){
    Serial.println("Asetetaan vertailuarvot");
    cX = mpu6050.getAngleX();
    Serial.println(cX);
    cY = mpu6050.getAngleY();
    Serial.println(cY);
    cZ = mpu6050.getAngleZ();
    Serial.println(cZ);
    if(cX ==0 || cY == 0 || cZ == 0){
      Serial.println("Oli nollia, lasketaan uusiksi");
      mpu6050.update();
      delay(1000);
      continue;
    }else{
      Serial.println("Ei nollia, suoritus jatkuu");
    break;
    }
  }
  delay(2000);
  Serial.println("");
  Serial.println("Arvot asetettu");
  Serial.print("Annettu arvo X: " ); Serial.println(cX);
  Serial.print("Annettu arvo Y: " ); Serial.println(cY);
  Serial.print("Annettu arvo Z: " ); Serial.println(cZ);

  compareValues(cX, cY, cZ);//kutsutaan compareValues funktiota, joka saa tästä arvot käyttöönsä?
  return cX, cY, cZ;
}

int compareValues(int cX, int cY, int cZ){
  int i =0;
  unsigned long previousTime = 0;
  const int timeLimit = 5000;
  int timer = millis();
  int counter = 1;
  int interval = mpu6050.getAngleX() -cX;
  int X;// = mpu6050.getAngleX();
  int Y;// = mpu6050.getAngleY();
  int Z;// = mpu6050.getAngleZ();
  //mpu6050.calcGyroOffsets(X);
  //mpu6050.calcGyroOffsets(Y);
  //mpu6050.calcGyroOffsets(Z);

  Serial.println("Vertaillaan arvoja loopissa");
  
  mpu6050.update();
  
  while(counter < 6){
    
   if(mpu6050.getAngleX() > (cX*5) || mpu6050.getAngleY() > (cY*5) || mpu6050.getAngleZ() > (cZ*5)){
     Serial.print("Tässä on X: "); Serial.println(mpu6050.getAngleX());
     Serial.print("cX: "); Serial.println(cX);
     Serial.print("Tässä on Y: "); Serial.println(mpu6050.getAngleY());
     Serial.print("cY: "); Serial.println(cY);
     Serial.print("Tässä on Z: "); Serial.println(mpu6050.getAngleZ());
     Serial.print("cZ: "); Serial.println(cZ);
     Serial.print("Counter: "); Serial.println(counter);
     delay(1000);
     mpu6050.update();
     Serial.print("Ajastin: "); Serial.println(timer);
     delay(1000);
     timer = 0;
   }
  }
}
