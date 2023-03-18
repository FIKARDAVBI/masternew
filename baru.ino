#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiManager.h>
#include "WiFi.h"
#include "secrets.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define AWS_IOT_PUBLISH_TOPIC "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "lockers/open"

WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);

int HID = 1, MID = 1;

const byte ROWS = 4;
const byte COLS = 4;

bool admin = false, praadmin = false;
unsigned long int previousMillis = 0;
int interval = 200, counter, timercheck;
String passcodeinput, passcode = "45678";
LiquidCrystal_I2C lcd(0x27, 20, 4);

char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 26, 25, 33, 32 };
byte colPins[COLS] = { 2, 12, 14, 27 };

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void connectAWS() {
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  client.setServer(AWS_IOT_ENDPOINT, 8883);

  client.setCallback(messageHandler);

  while (!client.connect(THINGNAME)) {
    lcd.clear();
    lcdtampil(0, 0, "Menghubungkan ke Server");
    delay(100);
  }

  if (!client.connected()) {
    Serial.println("AWS IoT Timeout!");
    lcd.clear();
    lcdtampil(0, 0, "Gagal Terkonesi ke Server");
    return;
  }
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
  Serial.println("AWS IoT Connected!");
  lcd.clear();
  lcdtampil(0, 0, "Terkoneksi Server");
}

void setwifidengantimeout() {
  WiFiManager wm;
  wm.setConfigPortalTimeout(120);

  bool stat_ = wm.startConfigPortal("Medstation1", "password");

  if (!stat_) {
    lcd.clear();
    lcdtampil(0, 0, "WAKTU HABIS");
    delay(4000);
    lcd.clear();
    LCDdefault();
  } else {
    lcd.clear();
    lcdtampil(0, 0, "BERHASIL TERKONEKSI");
    delay(2000);
    lcd.clear();
    LCDdefault();
  }
}

void setwifitanpatimeout() {
  WiFiManager wm;
  bool res;
  res = wm.autoConnect("Medstation1", "password");
  if (!res) {
    lcd.clear();
    lcdtampil(0, 0, "GAGAL TERKONEKSI");
  } else {
    lcd.clear();
    lcdtampil(0, 0, "BERHASIL TERKONEKSI");
  }
}

void LCDdefault() {
  lcd.clear();
  lcdtampil(0, 1, "NexMed Medstation");
  lcdtampil(1, 2, "Silahkan Gunakan");
  lcdtampil(2, 3, "Aplikasi Untuk");
  lcdtampil(3, 3, "Membuka Locker");
}

unsigned long int currentMillis() {
  return millis();
}

void lcdtampil(uint8_t row, uint8_t col, String kata) {
  lcd.setCursor(col, row);
  lcd.print(kata);
}

void buttoncheck(char key_) {
  if (key_ == 'C' && praadmin == false && admin == false) {
    lcd.clear();
    lcdtampil(0, 0, "Passcode : ");
    passcodeinput = "";
    praadmin = true;
  } else if (praadmin && admin == false) {
    if (key_ == 'D') {
      if (passcodeinput == passcode) {
        lcd.clear();
        lcdtampil(0, 0, "Membuka Loker : B");
        lcdtampil(1, 0, "Set Pass Wifi : C");
        counter = 0;
        praadmin = false;
        admin = true;
      } else {
        lcd.clear();
        lcdtampil(0, 2, "PASSWORD SALAH");
        delay(2000);
        LCDdefault();
        praadmin = false;
        admin = false;
        counter = 0;
      }
    } else {
      passcodeinput += key_;
      lcdtampil(1, 0, passcodeinput);
    }
  }
  if (admin) {
    if (key_ == 'C') {
      counter = 0;
      praadmin = false;
      admin = false;
      lcd.clear();
      lcdtampil(0, 0, "Silahkan Set Wifi");
      setwifidengantimeout();
      connectAWS();
      delay(2000);
    } else if (key_ == 'B') {
      Serial.print("membuka semua loker");
      counter = 0;
      praadmin = false;
      admin = false;
    }
  }
}

void publishMessage() {
  StaticJsonDocument<200> doc;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);  // print to client

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

int messagecheck(String dat_, int Hos, int Med) {
  String IDbuffer[10];
  int j = 0;
  for (int l = 0; l < dat_.length(); l++) {
    if (dat_[l] == '-') {
      j++;
      IDbuffer[j] = "";
    } else if (dat_[l] != '"') {
      IDbuffer[j] += dat_[l];
    }
  }
  if ((IDbuffer[0].toInt() == Hos) && (IDbuffer[1].toInt() == Med)) {
    return 1;
  } else {
    return 0;
  }
}

void messageHandler(char* topic, byte* payload, unsigned int length) {
  StaticJsonDocument<512> doc;
  deserializeJson(doc, payload);
  JsonArray lockerIds = doc["locker_ids"];  // get the "locker_ids" array
  for (int i = 0; i < lockerIds.size(); i++) {
    const char* lockerId = lockerIds[i];  // get the current locker ID
    String buffer = "";
    serializeJson(lockerIds[i], buffer);
    if (messagecheck(buffer, HID, MID)) {
      if ((strlen(lockerId) > 0)) {  // check if the locker ID has data
        Serial2.print(lockerId);     // print the locker ID to the serial monitor
        if (i < (lockerIds.size() - 1)) {
          Serial2.print(",");
        }
      }
    }
  }
  Serial2.print("\n");
}

void setup() {
  Serial2.begin(9600);
  Serial.begin(19200);
  lcd.backlight();
  lcd.init();
  lcd.clear();
  WiFi.mode(WIFI_STA);
  int timeout_ = 0;
  while (WiFi.status() != WL_CONNECTED) {
    lcdtampil(0, 0, "Menghubungkan Wifi....");
    delay(500);
    timeout_++;
    if (timeout_ > 50) {
      lcd.clear();
      lcdtampil(0, 0, "Gagal Menghubungkan");
      lcdtampil(1, 0, "Silahkan Set Wifi");
      setwifitanpatimeout();
    }
  }
  lcd.clear();
  lcdtampil(0, 0, "Berhasil Terkoneksi");
  delay(1000);
  connectAWS();
  delay(1000);
  LCDdefault();
}

void loop() {
  char customKey = customKeypad.getKey();
  if (customKey) {
    buttoncheck(customKey);
  }

  if (currentMillis() - previousMillis >= interval) {
    previousMillis = currentMillis();
    timercheck++;
    if (timercheck > 49) {
      timercheck = 0;
      if (WiFi.status() != WL_CONNECTED && !(praadmin || admin)) {
        lcd.clear();
        lcdtampil(0, 0, "Koneksi Wifi Putus");
      }
    }
    if ((praadmin || admin) && counter < 50) {
      counter++;
    } else if ((praadmin || admin) && counter > 49) {
      lcd.clear();
      lcdtampil(0, 0, "WAKTU SUDAH HABIS");
      delay(2000);
      LCDdefault();
      praadmin = false;
      admin = false;
      counter = 0;
    }
  }
  client.loop();
}
