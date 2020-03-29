#define snd39 39 //mikrofonin portti
int dB;

int soundDetection() {
  double volts;
  const int sampleWindow = 100;
  unsigned int sample;

   unsigned long startMillis= millis();  // aikaa lasketaan millisekunteina
   unsigned int peakToPeak = 0;   // peak-to-peak level
 
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;
 
   // collect data for 50 mS
   while(millis() - startMillis < sampleWindow)
   {
    
      sample = analogRead(snd39);
      sample = sample - 2900;
      //Serial.println(sample);
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
   //Serial.println(signalMin);
   //Serial.println(signalMax);
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   volts = (peakToPeak * 5.0) / 1024;  //luvut muutetaan volteiksi
   Serial.println(volts); //tulostetaan volttien määrä Vr = V2/V1

   if(volts > 0.8){
    Serial.println("Kova ääni");
    counter++;
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Laskuri: "); Serial.println(counter);
    if(counter >= 5){
      msg="Itkuhälytin ilmoitus! Havaittu ääntä!";
      sendMessage();
      delay(1000000);
    }
   }
}
