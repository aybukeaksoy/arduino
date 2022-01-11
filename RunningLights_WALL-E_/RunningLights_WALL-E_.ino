// C++ code
//
    
int input;
byte led=1;  
int button_count=0;
void runningLights(int delays,int isButtonUsed); // function declaration of runningLights
void ledWrite(byte led); // function declaration of ledWrite
void pause(); // function declaration of pause
void setup()
{
  pinMode(3,INPUT_PULLUP); //sets the 3rd pin's(button pin) function as input
  Serial.begin(9600); // begins serial communication
  for(int i=4;i<12;i++){
    pinMode(i,OUTPUT); //sets up the function of pins of leds(4-11) as output. 
  }
  attachInterrupt(digitalPinToInterrupt(3),pause,CHANGE); //to be able to use button as an interrupt for other functions
  Serial.print("Enter:\n");
  Serial.print("1 for running lights\n");
  Serial.print("2 for running lights with pause and resume\n");
  Serial.print("3 for binary count up - on timer\n");
  Serial.print("4 for binary count up - on button\n");
  Serial.print("5 for binary display of numbers [1-255]\n");  
}

void loop()
{
  if(Serial.available()>0){ //only reads the serial line if there is input.
    
  input=Serial.read(); //reads the serial line
  Serial.read(); //for the newline character
  if(input<49 || input>53){ //if user enters an invalid input
    Serial.println("Please enter a valid input between 1 and 5");
    while(1){ 
      ledWrite(255); //turns on all the leds
      delay(200);
      ledWrite(0); //turns off all the leds
      delay(200);
      }
  }}
 
 
   switch (input) { 
    
    case '1': { // to turn on and off the leds back to back from right to left and left to right.
    ledWrite(led); //turns on the rightmost led. 
    while (1){  
      runningLights(300,0); //calls the runningLights function with delay=300 and isButtonUsed=0
      }
      break;
      
    }
  
    case '2': { //to pause leds from running when the button is pushed and resume when its released
      ledWrite(led); 
      while(1){ //turns on the rightmost led. 
        runningLights(200,1); //calls the runningLights function with delay=300 and isButtonUsed=1
      }
      break;
    }

    case '3': { //to represent the numbers between 0 and 255 with leds
      led=0;
      while(1){
        ledWrite(led); //writing the count to the leds           
        Serial.write("Current count is "); //writes the current count
        Serial.println(led); 
        led++; //incrementing the count and also led                           
        delay(200);                          
    }
      break;}
  
  case '4': { //start to represent the numbers between 0 and 255 when button is pushed and released. 
    //Rolls back to 0 when it achieves 255 and waits for another button push and release.
      int track=0; //to keep the track of how many times button is pushed and released
      led=0;
      ledWrite(led); //turning off all the leds
      while(1){
          if(button_count==2+track){ //if button is pushed and released not only pushed
              track+=2; //incrementing the track by 2
              led=1; //turning on the rightmost led
              while(led!=0){ //continues until the led becomes 0 
                  ledWrite(led); //calling the ledWrite function
                  Serial.write("Current count is "); //writes the current count
                  Serial.println(led);
                  led++; //incrementing the count and also led    
                  delay(200);
             }
             ledWrite(led);
          }
      }
      break;}
  
   case '5': { //to get input from user and represent each input number in binary by using leds
     int number,digit1,digit2,digit3; //declaring the variables that will be used the calculate the original number
     Serial.print("Please enter a number between 0 and 255");
     while(1){
     if(Serial.available() > 0){ //Only enters the statement when there is something to read on the serial buffer
       delay(200); 
       
       if(Serial.available()==1){ // if the input has 1 digit
         number=Serial.read(); //reads the digit
         ledWrite(number-48); //converting the value of the number in ascii table to its original one
       }
       else if(Serial.available()==2){
         
         digit1=Serial.read();
         digit2=Serial.read();
         number=(digit1-48)*10+(digit2-48); //converting the value of the number in ascii table to its original one
         ledWrite(number);
       }
       else{
         digit1=Serial.read(); //reads the first digit
         digit2=Serial.read(); //reads the second digit
         digit3=Serial.read(); //reads the last digit
         number=(digit1-48)*100+(digit2-48)*10+(digit3-48); //converting the value of the number in ascii table to its original one
         ledWrite(number);
       }
     delay(200);
     }  
     }
     break;} 
  }
  
  delay(100);

}
void ledWrite(byte led){ //Writes the bits of the byte variable onto the leds one by one
  for(int i=0;i<8;i++){
    digitalWrite(i+4,bitRead(led,i));
  }
}
void runningLights(int delays,int isButtonUsed){
   
   for(int i=1;i<8;i++){
    if(isButtonUsed){ //if button is used in that switch case
      while (button_count%2==1){ //if button is pushed running stops
        delay(100);
      }
    }
    led=led << 1; //shifts the running led from right to left
    delay(delays);
    ledWrite(led); //calling the ledWrite function
    }
      
   for(int i=7;i>=1;i--){
      if(isButtonUsed){ //if button is used in that switch case
         while (button_count%2==1){ //if button is pushed running stops
           delay(100);
         }
      }
      led=led >> 1; //shifts the running led from left to right
      delay(delays);
      ledWrite(led); //calling the ledWrite function
      }
}
void pause(){ //function to count how many times button is pushed and released
  //will be used in attachInterrupt function
  button_count++; //incrementing the button count
}

  
