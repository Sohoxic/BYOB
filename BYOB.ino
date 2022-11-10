// Arduino Line Follower Robot Code
const int in1 =D5;  //Motor1  L293 Pin in1
const int in2 =D6;  //Motor1  L293 Pin in2
const int in3 =D7;  //Motor2  L293 Pin in3
const int in4 =D8;  //Motor2  L293 Pin in4
const int R_S =D0;  //ir sensor Right
const int L_S =D2;  //ir sensor Left
const int M_S =D1;  //ir sensor Middle
void setup() {

  pinMode(R_S, INPUT);
  pinMode(L_S, INPUT);
  pinMode(M_S, INPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  delay(1000);
  Serial.begin(9600);
}

void loop() {

  unsigned int maxHumid = 0;
  for(unsigned int x = 0; x < 5; x++)
  {
    wait_for_dht11();
    start_signal(2);
    int humid = read_dht11(2);
    if(humid > maxHumid) {
      maxHumid = humid;
    }
  }
  if ((digitalRead(R_S) == 0) && (digitalRead(L_S) == 0) && (digitalRead(M_S) == 1)) { forward(); }  //if Right Sensor and Left Sensor are at White color then it will call forword function

  if ((digitalRead(R_S) == 1) && (digitalRead(L_S) == 0) && (digitalRead(M_S) == 1)) { turnRight(); }  //if Right Sensor is Black and Left Sensor is White then it will call turn Right function

  if ((digitalRead(R_S) == 0) && (digitalRead(L_S) == 1) && (digitalRead(M_S) == 1)) { turnLeft(); }  //if Right Sensor is White and Left Sensor is Black then it will call turn Left function

  if (((digitalRead(R_S) == 1) && (digitalRead(L_S) == 1) && (digitalRead(M_S) == 0))||((digitalRead(R_S) == 0) && (digitalRead(L_S) == 0) && (digitalRead(M_S) == 0)) && maxHumid) { Stop(); }  //if Right Sensor and Left Sensor are at Black color then it will call Stop function

  

  
  
Serial.end();
}

void forward() {  //forword

  digitalWrite(in1, 0);  //Right Motor forword Pin
  digitalWrite(in2, 1);  //Right Motor backword Pin
  digitalWrite(in3, 1);  //Left Motor backword Pin
  digitalWrite(in4, 0);  //Left Motor forword Pin
  delay(100);
}

void turnRight() {  //turnRight

  digitalWrite(in1, LOW);  //Right Motor forword Pin
  digitalWrite(in2, HIGH);  //Right Motor backword Pin
  digitalWrite(in3, LOW);  //Left Motor backword Pin
  digitalWrite(in4, HIGH);  //Left Motor forword Pin
}

void turnLeft() {  //turnLeft

  digitalWrite(in1, HIGH);  //Right Motor forword Pin
  digitalWrite(in2, LOW);  //Right Motor backword Pin
  digitalWrite(in3, HIGH);  //Left Motor backword Pin
  digitalWrite(in4, LOW);  //Left Motor forword Pin
}

void Stop() {  //stop

  digitalWrite(in1, LOW);  //Right Motor forword Pin

  digitalWrite(in2, LOW);  //Right Motor backword Pin

  digitalWrite(in3, LOW);  //Left Motor backword Pin

  digitalWrite(in4, LOW);  //Left Motor forword Pin
}


void dec2bin(int n)
{
    int c, k;

   for (c = 15; c >= 0; c--)
    {
        k = n >> c;

        if (k & 1)
            Serial.print("1");
        else
            Serial.print("0");
        }
    } 

void dec2bin8(int n)
{
    int c, k;

   for (c = 7; c >= 0; c--)
    {
        k = n >> c;

        if (k & 1)
            Serial.print("1");
        else
            Serial.print("0");
        }
    }


void wait_for_dht11()
{
delay(2000);
}

void start_signal(uint8_t dht11_pin){
pinMode(dht11_pin, OUTPUT);
digitalWrite(dht11_pin, LOW); 
delay(18);
digitalWrite(dht11_pin, HIGH);
pinMode(dht11_pin, INPUT);
digitalWrite(dht11_pin, HIGH); 
}

int read_dht11(uint8_t dht11_pin)
{
  uint16_t rawHumidity = 0;
  uint16_t rawTemperature = 0;
  uint8_t checkSum = 0;
  uint16_t data = 0;

  uint8_t humi;
  uint8_t humd;
  uint8_t tempi;
  uint8_t tempd; 

  unsigned long startTime;
  
  for ( int8_t i = -3 ; i < 80; i++ ) {
    byte live;
    startTime = micros();

    do {
      live = (unsigned long)(micros() - startTime);
      if ( live > 90 ) {
        Serial.println("ERROR_TIMEOUT");
        return;
      }
    }
    while ( digitalRead(dht11_pin) == (i & 1) ? HIGH : LOW );

    if ( i >= 0 && (i & 1) ) {
      data <<= 1;

      // TON of bit 0 is maximum 30 usecs and of bit 1 is at least 68 usecs.
      if ( live > 30 ) {
        data |= 1; // we got a one
      }
    }

    switch ( i ) {
      case 31:
        rawHumidity = data;
        break;
      case 63:
        rawTemperature = data;
      case 79: 
        checkSum = data;
        data = 0;
        break;
    }
  }

Serial.println("Humidity: ");
dec2bin(rawHumidity);
Serial.print("\t");
humi = rawHumidity >> 8;
dec2bin8(humi);
Serial.print("\t");
rawHumidity = rawHumidity << 8;
humd = rawHumidity >> 8;
dec2bin8(humd);
Serial.print("\t");
Serial.print(humi);
Serial.print(".");
Serial.print(humd);
Serial.print("%");
Serial.println("");

Serial.println("Temperature Degree Celcius: ");
dec2bin(rawTemperature);
Serial.print("\t");
tempi = rawTemperature >> 8;
dec2bin8(tempi);
Serial.print("\t");
rawTemperature = rawTemperature << 8;
tempd = rawTemperature >> 8;
//tempd = (byte)rawTemperature;
dec2bin8(tempd);
Serial.print("\t");
Serial.print(tempi);
Serial.print(".");
Serial.print(tempd);
Serial.print("C");
Serial.println("");

Serial.println("Checksum Byte: ");
dec2bin8(checkSum);
Serial.println("");

Serial.println("");
if((byte)checkSum == (byte)(tempi + tempd + humi + humd)){dec2bin8(tempi + tempd + humi + humd);}
else {Serial.print("CHECKSUM_ERROR");}
Serial.println("");
Serial.println("");
Serial.println("");
}
