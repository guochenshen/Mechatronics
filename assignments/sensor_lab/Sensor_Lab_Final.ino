//Sensor Lab for Mechatronics
//Team I: Guochen, Deepak, Sruti, Richard, Luke, Anchit
//2/3/16

int lightPin = 0; //Ambient Light Sensor to Analog Pin 0
int slotPin = 1; //Optical Slot Sensor to Analog Pin 1
int irPin = 2; //IR Sensor to Analog Pin 2
int prPin = 3; //Photoresistor to Analog Pin 3
int ultraTrigPin = 8; //Ultrasonic Sensor Trigger to Digital Pin 8
int ultraEchoPin = 9; //Ultrasonic Sensor Echo to Digital Pin 9
int maxPRValue = 0;
int minPRValue = 190;
int prOutput = 0;
double window[5] = {0, 0, 0, 0, 0};
int i;
int j;
int k;
double val;
double avg;
double median;

void setup() {
  Serial.begin(9600);
  pinMode(lightPin, INPUT);
  pinMode(slotPin, INPUT);
  pinMode(irPin, OUTPUT);
  pinMode(prPin, INPUT);
  pinMode(ultraTrigPin, OUTPUT);
  pinMode(ultraEchoPin, INPUT);
  j = 0;
  k = 0;
}

void loop() {
  float lightVoltage = analogRead(lightPin);
  int slotValue = analogRead(slotPin);
  int irValue = analogRead(irPin);
  int prValue = analogRead(prPin);
  
  //ultrasonic calculations
  digitalWrite(ultraTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(ultraTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultraTrigPin, LOW);
  float ultraDuration = pulseIn(ultraEchoPin, HIGH);
  float ultraDistance = (ultraDuration/2) / 29.1;
  
  //light sensor calculations
  float lightAmps = lightVoltage/ 10000.0;
  float lightLux = lightAmps * 1000000.0 * 2.0;
  
  //slot sensor calculations
  slotValue = slotValue + analogRead(slotPin);
  slotValue = slotValue + analogRead(slotPin);
  int ssAvg = slotValue/3;
  String slotDetect = "Clear";
  
  if(ssAvg > 200){
    slotDetect = "Clear";
  }
    if(ssAvg < 200){
    slotDetect = "Object Detected";
  }
  
  //photoresistor calculations
//  float prAmps = prVoltage/ 10000.0;
//  float prLux = prAmps * 1000000.0 * 2.0;
  if (prValue > maxPRValue) {
    maxPRValue = prValue;
  } 
  if (prValue < minPRValue) {
   minPRValue = prValue; 
  }
   prOutput = (((255 - 0)*(prValue - minPRValue))/(maxPRValue - minPRValue))+0; 
  
   i=analogRead(irPin);
    //voltage_out = -75.79*(1/dist)^2+31.15*(1/dist) - 0.02
    val=(6762/(i-9))-5; //original
    // val=(25*(47708004556764623646264944272343040 - 15011340144537033171921364670480384*i)^(1/2) + 5479578924742607200)/(351843720888320000*i + 7810930603720704);
    String str = "distance in cm: " + String(val);
    //Serial.println(str);
    //occupy j
    window[j] = val;

    if (k==0 || k==1) {
      avg = window[j];
    }
    else if(k >= 4){
      avg = (window[0]+window[1]+window[2]+window[3]+window[4])/5;
    }

    str = "unaveraged IR distance in cm: " + String(double(window[2]));
    Serial.print(str);
    str = "\taverage filter distance in cm: " + String(avg);
    Serial.println(str);
    
    k++;
    
    if (j < 4) {
      j++;
    }
    else {
      j = 0;  
    }
    
  //Printing
  Serial.print("Light Sensor Lux: ");
  Serial.print(lightLux);
  Serial.println(";");
  Serial.print("Slot Sensor Value: ");
  Serial.print(slotDetect);
  Serial.println(";");
//  Serial.print("IR Distance: ");
//  Serial.print(irValue);
//  Serial.print(" cm");
//  Serial.println(";");
  Serial.print("Photoresistor Value: ");
  Serial.print(prOutput);
  Serial.println(";");
  Serial.print("Ultrasonic Distance: ");
  Serial.print(ultraDistance);
  Serial.print(" cm");
  Serial.println(";");
Serial.println();
  Serial.println("-");
  
  delay(500);
}
