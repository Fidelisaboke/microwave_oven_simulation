/* 
Microwave Modelling - by Fidel Isaboke
Simulating a microwave using Arduino
Circuit built on Tinkercad simulator
*/

#include <LiquidCrystal.h>
LiquidCrystal lcd(6, 7, 8, 9, 10, 11);

// What can the microwave do?
// Check at the end of 'void loop()' for full definitions
void checkPower();
void togglePower(bool microwave_state);
void powerOn();
void powerOff();
void displayMenu();
void processOption(int option_input);
void setTimer(); // Unused - requires further implementation
void displayOptions();
void run(unsigned int seconds);
void activateMotor(unsigned int seconds);
void cancel(); // Undefined - No implementation
bool checkOption(int option_input);
void displayOptionError();
void beep(int no_of_beeps, int tone_delay); // For the buzzer

// Setting pin constants
const int motor_left = 2;
const int motor_right = 3;
const int buzzer = 4;
const int power_button = 5;

/* Important variables (definitions) */
int option;
int previous_btn_state;
int current_btn_state;
bool microwave_on = false;

// Array of accepted options:
int options[4] = {1, 2, 3, 0};



/* The microwave functionality */
// Check the power button
void checkPower(){
  previous_btn_state = current_btn_state;
  current_btn_state = digitalRead(power_button);
  //Serial.print("Button state: ");
  //Serial.println(current_btn_state);
  
  if(previous_btn_state == LOW && current_btn_state == HIGH){
    microwave_on = !microwave_on;
    delay(20);
    togglePower(microwave_on);
  }
}

// Toggle the power on or off
void togglePower(bool microwave_state){
  if(microwave_state == true){
    powerOn();
  }
  else{
    powerOff();
  }
}

// Power on the microwave
void powerOn(){  
  // Startup display
  lcd.setCursor(0, 0);
  lcd.print("Microwave Model");
  lcd.setCursor(0, 1);
  lcd.print("By Fidel Isaboke");
  delay(1500);
  
  lcd.clear();
  delay(500);
 
  displayMenu();
}

// The microwave menu
void displayMenu(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("1 - Run");
  lcd.setCursor(0, 1);
  lcd.print("2 - Help");
  
  if(Serial.available()>0){
    lcd.clear();
    option = Serial.parseInt();
    Serial.println(option);
    bool is_an_option = checkOption(option);
    if(is_an_option){
      // Proceed to process option
      processOption(option);
    }else{
      // Display an error
      displayOptionError();
    }
  }
}

// Check if an option is valid
bool checkOption(int option_input){
  bool valid_option = false;
  for(int i = 0; i < sizeof(options); i++){
    if(option_input == options[i]){
      valid_option = true;
      break;
  	}
  }
  return valid_option;
}

// Process the option and redirect to submenu
void processOption(int option){
  if(option == 1){
    run(30);
  }else if(option == 2){
    displayOptions();
  }else{
    displayOptionError();
  }
}

// Set microwave timer
// Unused, may implement in future
void setTimer(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Timer (sec)");
  lcd.setCursor(0, 1);
  lcd.print("0 - 3600 sec");
}

void displayOptions(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Use 1,2,3,4");
  lcd.setCursor(0, 1);
  lcd.print("2 - Back");
  if(Serial.available()>0){
    option = Serial.parseInt();
    Serial.println(option);
    if(option == 2){
      displayMenu();
    }
  }
}

// Display an error message and sound the buzzer
void displayOptionError(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Invalid option");
  lcd.setCursor(0, 1);
  lcd.print("Options: ");
  lcd.setCursor(10, 1);
  lcd.print("A,B,C");
  beep(3, 200);
  
  // Redirect to menu
  displayMenu();
}

// Run the microwave
void run(unsigned int seconds = 20){
  activateMotor(seconds);
  for(int i = 0; i < 3; i++){
    beep(3, 500);
  	lcd.clear();
  	lcd.setCursor(0, 0);
  	lcd.print("Done cooking!");
  	delay(500);
  }
  displayMenu();
}

// Power off the microwave
void powerOff(){
  lcd.clear();
}

// Sound the buzzer - notification
void beep(int no_of_beeps, int tone_delay = 200){
  for(int i = 0; i < no_of_beeps; i++){
    tone(buzzer, 500);
  	delay(tone_delay);
    noTone(buzzer);
    delay(tone_delay);
  }
}

void activateMotor(unsigned int seconds){
  analogWrite(motor_left, 0);
  analogWrite(motor_right, 128);
  
  // Display time left
  for(int i = seconds; i > 0; i--){
  	lcd.clear();
  	lcd.setCursor(0, 0);
  	lcd.print("Time (sec):");
    lcd.setCursor(0, 1);
    lcd.print(i);
    Serial.println(i);
    delay(1000);
  }
}


/* Setup */
void setup(){
  /* pinMode(motor_left, OUTPUT);
  pinMode(motor_right, OUTPUT); */
  pinMode(buzzer, OUTPUT);
  pinMode(power_button, INPUT);
  lcd.begin(16, 2);
  Serial.begin(9600);
  
  // Initial button state - LOW
  current_btn_state = LOW;
}


/* Loop - runs the microwave */
void loop(){
  checkPower();
}
