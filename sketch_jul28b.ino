#include<Keypad.h> //Keypad
#include <Wire.h> // Wire
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);
const int bklight = 53;
const int rs = 41, en = 40, d4 = 26, d5 = 27, d6 = 28, d7 = 29;
const byte rows = 4; 
const byte cols = 4; 
char keys[rows][cols] = {
  {'1','2','3','a'},
  {'4','5','6','b'},
  {'7','8','9','c'},
  {'#','0','*','d'}
};
 
byte rowPins[rows] = {37, 36, 35, 34}; 
byte colPins[cols] = {33, 32, 31,30}; 

String password = "1234";  // Password
int position = 0; // Character's password positiion

int wrong = 0; // Failed attempt counter

int redPin = 42; 
int greenPin = 43; 
int buzzer = 49; 


Keypad keypad=Keypad(makeKeymap(keys),rowPins,colPins,4,4);

int total = 0; // Failed attempt overall
// Initialization
void setup(){
 pinMode(bklight, OUTPUT);
 pinMode(redPin,OUTPUT);
 pinMode(greenPin,OUTPUT);
 pinMode(buzzer, OUTPUT);
 lcd.init(); //LCD initialization
 lcd.backlight();
 lcd.clear(); // Clear LCD screen
 setLocked(true); // Locking
 delay(20);
}

void loop(){
  digitalWrite(bklight, HIGH);
  lcd.clear(); // Clear LCD screen
  lcd.print(" Enter the code : ");
  delay(100);
  char pressed=keypad.getKey();//Get the inpur
  String key[3];
 
  if(pressed){
    if(pressed == '#'){ // The door is lock if we press '#'
          position = 0; // Clear character's password position
          setLocked(true); // Locked
          lcd.clear(); // Clear LCD screen
      }

     else if(pressed == password[position]){ //The character is in the right order
          key[position]=pressed;
          position++; // Incrementation of the position
      }
 
     else if (pressed != password[position] ) // The character is in the wrong order
      {
          wrong++; // Incrementation of failed attempt counter
          position ++; // Incrementation of the position
      }

    if(position == 4){
          if( wrong >0) // The password is wrong
            {
               
                total++; // Incrementation of the overall failed attempt
                wrong = 0; // clear counter
                position = 0; // Clear character's password position
                lcd.clear(); //  Clear LCD screen
                lcd.setCursor(0,1);
                lcd.print("  WRONG CODE!");
                lcd.setCursor(0,2);
                lcd.print("  LEFT ATTEMPTS: ");
                lcd.setCursor(19,2);
                lcd.print(3-total);
                setLocked(true); // Vérouillage
            }

          else if(position == 4 && wrong == 0) // The code is right
            {
                buzzer_beep_unlock(); // Unlock the door
            }

             if(total ==3){ // 3 failed attempt in a row
                total=0;  // clear counter
                buzzer_beep_lock(); // Start the alarm
            }

        }

   }

   
}

void setLocked(int locked){ // Lock/unlock function
    if (locked){ // Lock
          digitalWrite(redPin, HIGH); // Set Lock LED to  1
          digitalWrite(greenPin, LOW); // Set Unlock LED to  0
          delay(2000);
      }
    else{ //Unlock
          digitalWrite(redPin, LOW); // Set Lock LED to  0
          digitalWrite(greenPin, HIGH); //Set Unlock LED to  1
          delay(5000);
          digitalWrite(redPin, HIGH); // Set Lock LED to  1
          digitalWrite(greenPin, LOW); // Set Unlock LED to  0
      }
  }
void buzzer_beep_lock(){ // Alarm
   lcd.clear(); // Clear LCD Screen
   lcd.setCursor(0,1);
   lcd.print("   ACCESS DENIED !!!");
   // Start alarm during 10 seconds
   digitalWrite(buzzer,HIGH);
   delay(10000);
   digitalWrite(buzzer,LOW);
   
}
void buzzer_beep_unlock(){ // Alarm
   position = 0; // Clear character's password position
   wrong = 0; // Clear counter
   lcd.clear(); // Clear LCD Screen
   lcd.setCursor(0,1);
   lcd.print("      WELCOME!     ");
   digitalWrite(buzzer,HIGH);
   delay(100);
   digitalWrite(buzzer,LOW);
   setLocked(false); // unlock
   
}
