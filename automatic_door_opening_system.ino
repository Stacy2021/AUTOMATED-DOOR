

#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#include <SPI.h>
#include <MFRC522.h>




const int trigPin = 7;
const int echoPin = 8;
const int relayPin = 3;
int servoPin = 5;
int led1 = 4;
int rstPin = 9 ;
int ssPin = 10 ;
int mosiPin = 11;// rfid miso pin
int misoPin = 12;//rfid mosi pin
int sckPin = 13;
int angle = 0;// micro servo angle


LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 mfrc522(ssPin, rstPin);
Servo servo;

long duration;
int distance_cm;

void setup() {


  Serial.begin(9600);// initialises the serial printer
  SPI.begin();
  mfrc522.PCD_Init();//initiate rfid



  lcd.init();//initiate lcd
  lcd.backlight();
  lcd.clear();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(relayPin, OUTPUT);
  pinMode(led1, OUTPUT);


  servo.attach (servoPin);
  servo.write(angle);

}

void loop() {

  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance_cm = 0.034 / 2 * duration;

  if (distance_cm <= 20)
  {

    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("TAP CARD");
    
  }






  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }

  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.println("Message : ");
  content.toUpperCase();

  if (content.substring(1) == "4C 8B EF 38")

  {
    
    lcd.setCursor(0,0);
    lcd.clear();
    lcd.print("authorized access");
    
    digitalWrite(led1, HIGH);
    digitalWrite(relayPin, LOW);
    if (angle == 0)
    {
      angle = 90;
    }

    else       // (angle==90);
    {
      angle = 0;
    }
    servo.write(angle);

    delay(1000);
  }



  else {
    lcd.setCursor(0,0);
    lcd.clear();
    lcd.print("access denied");

    digitalWrite(relayPin, HIGH);
    digitalWrite(led1, LOW);
    delay(100);
  }
  Serial.println(distance_cm);
}
