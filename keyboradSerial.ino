uint8_t const cols[] = { 2, 3, 4, 5, 6 };
uint8_t const rows[] = { 7, 8, 9, 10, 11, 12 };
uint8_t const ledPin = 22;
void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, 0);
  for (const uint8_t &pin : cols) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, 1);
  }
  for (const uint8_t &pin : rows) {
    pinMode(pin, INPUT_PULLUP);
  }
  Serial.begin(115200);
}
boolean isTimeOut(unsigned long &time, const unsigned int &timeOut) {
  unsigned long currentTime = millis();
  boolean rs = false;
  if (currentTime > time) {
    rs = currentTime - time >= timeOut;
  } else {
    rs = time - currentTime >= timeOut;
  }
  if (rs) {
    time = currentTime;
  }
  return rs;
}

boolean valueOf(uint8_t const &pin, boolean status = true) {
  if (!digitalRead(pin)) {
    delay(30);
    if (!digitalRead(pin)) {
      return status;
    }
  }
  return !status;
}

String valueString = "";
String tempValue = "";
unsigned long checkTime = millis();
char a, b;
void readSerial() {
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    line.trim();
    if (line.equalsIgnoreCase("isConnect")) {
      Serial.println("isConnect");
    }
  }
}
void loop() {
  readSerial();
  if (!valueString.equals(tempValue) && isTimeOut(checkTime, 50)){
    if (!tempValue.isEmpty()) {
      Serial.println(tempValue);
      digitalWrite(ledPin, 1);
      delay(100);
      digitalWrite(ledPin, 0);
    }
    valueString = tempValue;
  }
     
  ////////////////////////////
  a = 'A';
  tempValue = "";
  for (const uint8_t &colPin : cols) {
    digitalWrite(colPin, 0);
    b = '1';
    for (const uint8_t &rowPin : rows) {
      if (valueOf(rowPin)) {
        if (!tempValue.isEmpty()) {
          tempValue += '-';
        }
        tempValue += a;
        tempValue += b;
      }
      b += 1;
    }
    digitalWrite(colPin, 1);
    a += 1;
  }
}
