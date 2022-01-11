unsigned long currentTime=0;
unsigned int myTime=0;
unsigned long currentSenseTime=0;
unsigned long currentFSMTime=0;
unsigned long currentActuationTime=0;
byte button1=0;
byte button2=0;
const bool RELEASED=false;
const bool PRESSED=true;
bool buttonReleased=false;
const unsigned int DEBOUNCE_TIME=3;
byte ledState1=0;
byte ledState2=0;
bool moveLED=false;
int player1=0;
int player2=0;
bool leftMove=true;
bool rightMove=true;
int FSMState=0;
int penalty1=0;
int penalty2=0;

void ledDriver (byte x){  //an important function which displays any byte type variable onto the 8 LEDs
  for(int i=0 ; i<8 ; i++){
    digitalWrite(i+4 ,bitRead(x,i)); } //Writes the bits of the byte variable onto the leds one by one
  }
void Timekeeper(){
  unsigned long currentMillis=millis();
  if(currentMillis-currentTime>=10){
    currentTime=currentMillis;
    myTime++;
  }
}
void buttoncheck1(){
 
  button1++;
  
}
void buttoncheck2(){ 
  
  button2++;
  
}
bool CheckPressRelease(int button){
  if(digitalRead(button)){
    return RELEASED;
  }
  return PRESSED;
}
void CheckButtonPolling(byte buttonstate, int button){
  static unsigned int buttonPressedTime=0;
  switch(buttonstate){
    case 0:
        if(CheckPressRelease(button)==PRESSED && (myTime-buttonPressedTime>DEBOUNCE_TIME)){
          buttonPressedTime=myTime;
          buttonstate=1;
        }
        
    case 1:
        if(CheckPressRelease(button)==RELEASED && (myTime-buttonPressedTime>DEBOUNCE_TIME)){
          buttonPressedTime=myTime;
          buttonReleased=true;
          buttonstate=0;
    }
    break;
  }
  
}
void Sense(){
  CheckButtonPolling(button1,2);
  CheckButtonPolling(button2,3);
}
void FSM(){
  
  switch(FSMState) {
  case 0:
     if(button1==1 && penalty1==0){
        moveLED=1;
        leftMove=0;
        FSMState=3;
        rightMove=1;
     }
     if(button2==1 && penalty2==0){
        moveLED=1;
        leftMove=1;
        rightMove=0;
        FSMState=1;
     }
     break;
  case 1:
      if(button2>1){          
        player1+=2;
        penalty2=5;
        button2=0;
        leftMove=0;
        rightMove=0;
        moveLED=0;
        ledState2=0;
        FSMState=0;
        Serial.println("Penalty for Player2");
        Serial.println("Player1 Score:");
        Serial.println(player1);
        Serial.println("Player2 Score:");
        Serial.println(player2);     
      }
      if(button1==1 && penalty1==0){
        leftMove=1;
        rightMove=1;
        FSMState=2;
        moveLED=1;
      }
      if(leftMove && ledState2==128){
        player2++;
        Serial.println("Player1 Score:");
        Serial.println(player1);
        Serial.println("Player2 Score:");
        Serial.println(player2);     
        FSMState=0;
        moveLED=0;
        ledState1=0;
        ledState2=0;
        button2=0;
      }
    // code block
    break;
  case 2:
    // code block
    if(ledState1==ledState2 || ledState1==ledState2<<1){
      Serial.println("SHOTS CANCELLING EACH OTHER");
      Serial.println("Player1 Score:");
      Serial.println(player1);
      Serial.println("Player2 Score:");
      Serial.println(player2); 
      FSMState=0;
      moveLED=0;
      ledState1=0;
      ledState2=0;
      button1=0;
      button2=0;
    }
        
    break;
  case 3:
    // code block
    if(button1>1){
        player2+=2;
        Serial.println("Penalty for Player1");
        Serial.println("Player1 Score:");
        Serial.println(player1);
        Serial.println("Player2 Score:");
        Serial.println(player2);     
        penalty1=5;
        button1=0;
        leftMove=0;
        rightMove=0;
        moveLED=0;
        ledState1=0;
        FSMState=0;
      }
    if(button2==1 && penalty2==0){
        leftMove=1;
        FSMState=2;
        moveLED=1;
        rightMove=1;
     }
     if(!leftMove && ledState1==1){
        player1++;
        Serial.println("Player1 Score:");
        Serial.println(player1);
        Serial.println("Player2 Score:");
        Serial.println(player2);     
        FSMState=0;
        moveLED=0;
        ledState1=0;
        ledState2=0;
        button1=0;
      }
    break;
  }
}
void Actuate(){
  if(moveLED){
    if(leftMove){
      if(ledState2==0){
        ledState2=1;
      }
      else{
        ledState2=ledState2 << 1;
        if(ledState2==0){
          leftMove=false;
        }
        if(penalty1){
          penalty1--;
        }
      }
    }
    if(rightMove){
      if(ledState1==0){
        ledState1=128;
      }
      else{
        ledState1=ledState1 >> 1;
        if(ledState1==0){
          leftMove=true;
        }
        if(penalty2){
          penalty2--;
        }
      }
      
    }
  }
  ledDriver(ledState1);
  ledDriver(ledState2);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(3,INPUT_PULLUP);
  pinMode(2,INPUT_PULLUP);
  Serial.begin(9600); // begins serial communication
  for(int i=4;i<12;i++){
    pinMode(i,OUTPUT); //sets up the function of pins of leds(4-11) as output. 
  }
  attachInterrupt(digitalPinToInterrupt(2),buttoncheck1,RISING); //to be able to use button as an interrupt for other functions
  attachInterrupt(digitalPinToInterrupt(3),buttoncheck2,RISING); 

}

void loop() {
  // put your main code here, to run repeatedly:
  Timekeeper();
  
  if(myTime%5==0 && myTime !=currentSenseTime){
    currentSenseTime=myTime;
    Sense();
  }

  if(myTime!=currentFSMTime){
    currentFSMTime=myTime;
    FSM();
  }

  if(myTime%50==0 && myTime != currentActuationTime){
    currentActuationTime=myTime;
    Actuate();
  }
  if (player1==5){
    Serial.println("Winner: player1");
    FSMState=0;
    player1=0;
    player2=0;
  }
  if (player2==5){
    Serial.println("Winner: player2");
    FSMState=0;
    player1=0;
    player2=0;
  }
}
