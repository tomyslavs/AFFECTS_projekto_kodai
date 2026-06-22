// Read 6 state LEDs and 5 level LEDs in Mindalive device

//int incomingByte = 0; // for incoming serial data
const byte interruptPin2 = 2; // PIN2 PD2 interrupt pin, Start session - Tone
const byte interruptPin3 = 3; // PIN3 PD3 interrupt pin, Stop session - Turn ON/OFF
volatile byte enable = LOW, enable_disp = HIGH;

int LV_1 = 0, LV_2 = 0, LV_3 = 0, LV_4 = 0, LV_5 = 0;
int ST_1 = 0, ST_2 = 0, ST_3 = 0, ST_4 = 0, ST_5 = 0, ST_6 = 0;
int Level = 0, State = 0;

void setup()
{
  attachInterrupt(digitalPinToInterrupt(interruptPin2), btn_note, FALLING); // CHANGE RISING FALLING
  attachInterrupt(digitalPinToInterrupt(interruptPin3), btn_on_off, RISING); // CHANGE RISING FALLING
  pinMode(4, INPUT);     // State = 1 Energize
  digitalWrite(4, LOW);  // turn on pullup resistors
  pinMode(5, INPUT);     // State = 2 Meditate
  digitalWrite(5, LOW);  // turn on pullup resistors
//  pinMode(21, INPUT);     // State = 3 Brain Booster    A7 - analog
//  digitalWrite(21, LOW);  // turn on pullup resistors
//  pinMode(20, INPUT);     // State = 4 Sleep            A6 - analog :(
//  digitalWrite(20, LOW);  // turn on pullup resistors
  pinMode(19, INPUT);     // State = 5 Feeling Better
  digitalWrite(19, LOW);  // turn on pullup resistors
  pinMode(18, INPUT);     // State = 6 Gamma
  digitalWrite(18, LOW);  // turn on pullup resistors
  
  pinMode(13, INPUT);    // LVL = 5
  digitalWrite(13, LOW); // 0
  pinMode(14, INPUT);    // LVL = 4
  digitalWrite(14, LOW); // 0
  pinMode(15, INPUT);    // LVL = 3
  digitalWrite(15, LOW); // 0
  pinMode(16, INPUT);    // LVL = 2
  digitalWrite(16, LOW); // 0
  pinMode(17, INPUT);    // LVL = 1
  digitalWrite(17, LOW); // 0
  
  //pinMode(LED_BUILTIN, OUTPUT); // 13 pin
  pinMode(12, OUTPUT);          // ON 3s | ON 1s = '0' | '1'
  pinMode(11, OUTPUT);          // NOTE       0.1s '2'
  pinMode(10, OUTPUT);          // VOL UP     0.1s '3'
  pinMode(9, OUTPUT);           // VOL DOWN   0.1s '4'
  pinMode(8, OUTPUT);           // INT UP     0.1s '5'
  pinMode(7, OUTPUT);           // INT DOWN   0.1s '6'
  pinMode(6, OUTPUT);           // CES UP     0.1s '7'
//  pinMode(5, OUTPUT);           // CES DOWN   0.1s '8'
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
  digitalWrite(10, LOW);
  digitalWrite(9, LOW);
  digitalWrite(8, LOW);
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
//  digitalWrite(5, LOW);
  Serial.begin(9600); // set baud rate
}

void btn_note(){ // interrupt function
  if (enable==HIGH){ // gaudome vertes viena kart per paleidima
    LV_5 = digitalRead(13);         // read LVL5
    LV_4 = digitalRead(14);         // read LVL4
    LV_3 = digitalRead(15);         // read LVL3
    LV_2 = digitalRead(16);         // read LVL2
    LV_1 = digitalRead(17);         // read LVL1
    ST_1 = not digitalRead(4);      // State = 1 Energize       - INVERTED LOGIC!
    ST_2 = not digitalRead(5);      // State = 2 Meditate       - INVERTED LOGIC!
    ST_3 = analogRead(A7);          // State = 3 Brain Booster  - A7 - analog!
    if (ST_3 > 512)
      ST_3 = 0;
    else
      ST_3 = 1;
    ST_4 = analogRead(A6);          // State = 4 Sleep          - A6 - analog! 
    if (ST_4 > 512)
      ST_4 = 0;
    else
      ST_4 = 1;
    ST_5 = not digitalRead(19);     // State = 5 Feeling Better - INVERTED LOGIC!
    ST_6 = not digitalRead(18);     // State = 6 Gamma          - INVERTED LOGIC!
    
    State = (ST_6<<5) | (ST_5<<4) | (ST_4<<3) | (ST_3<<2) | (ST_2 << 1)| ST_1;
    Level = (LV_5<<4) | (LV_4<<3) | (LV_3<<2) | (LV_2 << 1)| LV_1;
    
    if (State==32)     State = 6;
    else if (State==16)State = 5;
    else if (State==8) State = 4;
    else if (State==4) State = 3;
    else if (State==2) State = 2;
    else if (State==1) State = 1;
    else               State = 0; // not valid state or device is turned off;
    
    if (Level==16)     Level = 5;
    else if (Level==8) Level = 4;
    else if (Level==4) Level = 3;
    else if (Level==2) Level = 2;
    else if (Level==1) Level = 1;
    else               Level = 0; // not valid level or device is turned off;

    if ((State > 0) && (Level > 0) && (enable_disp == HIGH)){
      Serial.print(State);
      Serial.println(Level);
      enable_disp = LOW;
    }
  }
  enable = LOW;
  if ((State == 0) && (Level == 0)) // State ir Level nusinulina per restarta
    enable_disp = HIGH;             // rodyk viena karta po restarto ir tik note mygtuko paspaudimo metu
  //  Serial.println(LV_1, DEC);
}

void btn_on_off() { // interrupt function
  // Serial.print("ON-OFF "); Serial.print(State); Serial.println(Level);
  enable = HIGH; 
  // dar galima su timeriu apriboti, max 1x per 7min, nes toks minimalus stimuliavimo laikas 15 rezime
}

void loop()
{

  /* Mygtuku valdymas */
//  delay(1000);
//  LV_5 = digitalRead(13);     // read LVL5
//  Serial.println(LV_5, DEC);
  
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // what you got:
    Serial.print("I received char: ");
    Serial.println(incomingByte, DEC);
    if (incomingByte=='0'){      // ON 3s
      digitalWrite(12, HIGH);
      delay(4000);            
      digitalWrite(12, LOW); 
    }
    else if (incomingByte=='1'){ // ON 1s
      digitalWrite(12, HIGH);
      delay(1000);            
      digitalWrite(12, LOW); 
    }
    else if (incomingByte=='2'){ // NOTE 0.1s
      digitalWrite(11, HIGH);
      delay(100);            
      digitalWrite(11, LOW); 
    }
    else if (incomingByte=='3'){ // VOL UP 0.1s
      digitalWrite(10, HIGH);
      delay(100);            
      digitalWrite(10, LOW); 
    }
    else if (incomingByte=='4'){ // VOL DOWN 0.1s
      digitalWrite(9, HIGH);
      delay(100);            
      digitalWrite(9, LOW); 
    }
    else if (incomingByte=='5'){ // INT UP 0.1s
      digitalWrite(8, HIGH);
      delay(100);            
      digitalWrite(8, LOW); 
    }
    else if (incomingByte=='6'){ // INT DOWN 0.1s
      digitalWrite(7, HIGH);
      delay(100);            
      digitalWrite(7, LOW); 
    }
    else if (incomingByte=='7'){ // CES UP 0.1s
      digitalWrite(6, HIGH);
      delay(100);            
      digitalWrite(6, LOW); 
    }
    else if (incomingByte=='8'){ // CES DOWN 0.1s
      digitalWrite(5, HIGH);
      delay(100);            
      digitalWrite(5, LOW); 
    }
  }
  
}
    
