#include <HardwareSerial.h>
#define TX_PIN 17
#define RX_PIN 16
byte ECM_WAKEUP[] = {0xFE, 0x04, 0x72, 0x8C}; 
byte ECM_PART[] = {0x72, 0x05, 0x71, 0x00, 0x18};
char hexString2[11]; 
byte buff[14];
int buffCount=0;
int hexIndex = 0;

HardwareSerial bike(2);

void setup() {
  Serial.begin(9600);
  wakeup();
  ecmPart();
}
void loop() {}

void wakeup(){
  pinMode(TX_PIN, OUTPUT);
  //WAKE UP KE-1
  digitalWrite(TX_PIN, HIGH); 
  delay(100);
  digitalWrite(TX_PIN, LOW);
  delay(70);
  digitalWrite(TX_PIN, HIGH);
  delay(120);
  //WAKE UP KE-2 (TUJUANNYA MEMASTIKAN BENAR-BENAR WAKEUP)
  digitalWrite(TX_PIN, HIGH); 
  delay(100);
  digitalWrite(TX_PIN, LOW);
  delay(70);
  digitalWrite(TX_PIN, HIGH);
  delay(120);
  //SETTING SERIAL2 (KLINE)
  bike.begin(10400, SERIAL_8N1, 16, 17);
  bike.write(ECM_WAKEUP, sizeof(ECM_WAKEUP));
  delay(100);  
}

void ecmPart(){
  bike.flush();
  delay(100);
  bike.write(ECM_PART, sizeof(ECM_PART));
  delay(100);
  buffCount = 0;
  while ( (bike.available() > 0 ) && ( buffCount < 21)) {
    buff[buffCount++] = bike.read();
  }
  buffCount = 0;
  memset(hexString2, 0, sizeof(hexString2));
  int buffStart = 9;
  int buffEnd = 13;
  int hexIndex = 0;
  for (int i = buffStart; i <= buffEnd; i++) {
    sprintf(&hexString2[hexIndex], "%02X", buff[i]);
    hexIndex += 2; 
  }
  if (strcmp(hexString2, "0102E30F01") == 0) {
    Serial.println();
    Serial.println();
    Serial.print("ECM CODE = "); Serial.println(hexString2);
    Serial.println("ECM PART = 30400-K81H-N52");
  }
  //Lanjutkan perintah if dengan ECM Part lainnya
}
