#include "ESP32_MailClient.h" //tuodaan sähköpostin lähettämiseen tarvittava kirjasto
#include <MPU6050_tockn.h>
#include <Wire.h>
MPU6050 mpu6050(Wire);
const char* ssid = "IidaIid"; //asetetaan wifi salasana ja tunnus
const char* password = "Iidaonparas";
int LED_BUILTIN = 2;
String msg;
int counter;
int interval = 50000;
unsigned long previousMillis = 0;
unsigned long currentMillis = millis();

#define emailSenderAccount    "rndmgnm@gmail.com"    
#define emailSenderPassword   "paskojavihanneksia"
#define emailRecipient        "otto@xd.fi"
#define smtpServer            "smtp.gmail.com"
#define smtpServerPort        465
#define emailSubject          "Itkuhälytin ilmoitus"
#define I2C_SCL 21//määritellään mitä portteja gyroskooppi käyttää
#define I2C_SDA 22

//sähköpostin lähetysolion data
SMTPData smtpData;

void loop() {
  soundDetection();
  motionSensor();
  
 delay(500);
}

//callback funktio, jolla haetaan sähköpostin status
void sendCallback(SendStatus info);

void setup(){
  
  Serial.begin(9600);
  Wire.begin(I2C_SDA, I2C_SCL);
  mpu6050.begin();
  mpu6050.calcGyroOffsets(false);
  pinMode (LED_BUILTIN, OUTPUT);  
}

void(* resetFunc) (void) = 0;//uudelleenkäynnistys funktio

void sendMessage(){
  Serial.print("Yhdistetään");
 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
  }

  Serial.println();
  Serial.println("Langaton yhteys muodostettu");
  Serial.println();
  Serial.println("Valmistellaan lähetettävää sähköpostia");
  Serial.println();
  
  //asetetaan SMTP palvelimen host, portti, tili ja salasana
  smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);

  // asetetaan lähettäjän sähköposti osoite ja nimi
  smtpData.setSender("Itkuhälytin ilmoitus", emailSenderAccount);

  //asetetaan sähköposti prioriteetti
  smtpData.setPriority("High");

  // asetetaan aihe
  smtpData.setSubject(emailSubject);

  // asetetaan viesti HTML-muotoon
  //smtpData.setMessage("<div style=\"color:#2f4468;\"><h1>Itkuhälytin ilmoitus</h1><p>Liikettä havaittu</p></div>", true);
  smtpData.setMessage(msg, true);
  
  // lisätään vastaanottajan tiedot
  smtpData.addRecipient(emailRecipient);

  smtpData.setSendCallback(sendCallback);

  //aloitetaan sähköpostin lähetys
  if (!MailClient.sendMail(smtpData))
    Serial.println("Virhe lähetyksessä, " + MailClient.smtpErrorReason());

  //tyhjennetään email-oliosta tilan vapauttamista varten
  smtpData.empty();
}

//callback funktio, jolla saadaan sähköpostin lähetys status
void sendCallback(SendStatus msg) {
  // Print the current status
  Serial.println(msg.info());

  // lähetyksen onnistumisviesti
  if (msg.success()) {
    Serial.println("Sähköpostin lähetys onnistui! Woop woop");
  }
}
