int Pump=17;
int PumpEnable=18;
void setup() {
  // put your setup code here, to run once:
  pinMode(Pump, OUTPUT);
  pinMode(PumpEnable,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly: 
  digitalWrite(Pump, HIGH); //pump turns on, drawing 17mA from Arduino, driving pump with 525mA  
  digitalWrite(PumpEnable, HIGH);
  delay(5000); //for 5 seconds, adjust for PWM
  digitalWrite(Pump, LOW); //pump turns off, no current from Arduino nor current through pump
  digitalWrite(PumpEnable, LOW);
  delay(5000);//for 5 seconds, adjust for PWM
}
