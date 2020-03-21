#include "ESP32_MailClient.h" //tuodaan sähköpostin lähettämiseen tarvittava kirjasto
#include <MPU6050_tockn.h>
#include <Wire.h>
MPU6050 mpu6050(Wire);
const char* ssid = "IidaIida"; //asetetaan wifi salasana ja tunnus
const char* password = "Iidaonparas";

#define emailSenderAccount    "rndmgnm@gmail.com"    
#define emailSenderPassword   "paskojavihanneksia"
#define emailRecipient        "otto@xd.fi"
#define smtpServer            "smtp.gmail.com"
#define smtpServerPort        465
#define emailSubject          "Itkuhälytin ilmoitus"
#define I2C_SCL 21//määritellään mitä portteja lämpömittari ja gyroskooppi käyttää
#define I2C_SDA 22

//sähköpostin lähetysolion data
SMTPData smtpData;

void loop() {
 int cX;//vertailumuuttujat esim cX "comparisonX"
 int cY;
 int cZ;
 settingComparisonValues();
 //compareValues(cX, cY, cZ);
 delay(500000);
}

//callback funktio, jolla haetaan sähköpostin status
void sendCallback(SendStatus info);

void setup(){
  Serial.begin(9600);
  Wire.begin(I2C_SDA, I2C_SCL);
  mpu6050.begin();
  mpu6050.calcGyroOffsets(false);
  //Serial.begin(115200);
  Serial.println();
}
void sendMessage(){
  Serial.print("Yhdistetään");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
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
  smtpData.setMessage("<div style=\"color:#2f4468;\"><h1>Itkuhälytin ilmoitus</h1><p>- Sent from ESP32 board</p></div>", true);

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
    Serial.println("Woop woop");
  }
}
