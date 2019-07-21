   
/*
  Spelit

  Spelit is reaction speed game from tv-show called Speden Spelit.

  Created:
  Akseli  26.9.2018
  Modified:
  Akseli  21.7.2019

  https://github.com/akseliaa/spelit

*/
  
  #include <LiquidCrystal.h>
  #include <EEPROM.h>
  
  //Button pins.
  int green = 13;
  int blue = 12;
  int yellow = 11;
  int red = 10;

  //Button led pins.
  int greenLED = 5;
  int blueLED = 4;
  int yellowLED = 3;
  int redLED = 2;

  int latest = 0;   //Last colour number.
  int bufferList[50];
  int bufferSize = 0;
  int score = 0;
  int changeDelay = 1000;
  int highScore = 0;
  String hsName = "";

  bool firstTime = true;
  const char letters[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
  const int rs = 30, en = 32, d4 = 34, d5 = 36, d6 = 38, d7 = 40;   //Pins for lcd.
  LiquidCrystal lcd(rs, en, d4, d5, d6, d7);    //Lcd library initialization.

  //Object for saving high score variables.
  struct Object {
    int score;
    int nameLength;
    char field1;
    char field2;
    char field3;
    char field4;
    char field5;
    char field6;
    char field7;
    char field8;
    char field9;
    char field10;
    char field11;
    char field12;
    char field13;
    char field14;
    char field15;
  };

void setup() {
  Serial.begin(9600);
  pinMode(green, INPUT);
  pinMode(blue, INPUT);
  pinMode(yellow, INPUT);
  pinMode(red, INPUT);

  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  
  lcd.begin(16, 2);   //Defining lcd library to use 16x2 display.
  delay(500);
  readHighScore();
  endGame();
}

void loop() {
  latest = getRandom(latest);   //Returns random number that defines next colour.
  switchOn(latest);    //Switch on the drawed colour.
  check(changeDelay);   //Check if any button is being pressed.
  decreaseChangeDelay(5);
}


void check(int givenDelay) {
  while(givenDelay > 0) {    //Run 'button press check' -loop for given time.
    int isPressed = pressed();    //Check if any button is being pressed.
    if (isPressed != -1) {   
      if (isPressed == bufferList[0]) {   //Check if pressed button is the first one on the buffer list. 
        deleteLast();   //Remove pressed button from the buffer list.
        score++;    //Add one point to the score.
        showCurrentScore();    //Show current score.
        if(bufferSize == 0) {   //Turn off all colours if the pressed button was the only one on the buffer list.
          allOFF();
        }
      } else {    //End game because wrong button was pressed
        endGame();
      }
    }
    delay(1);
    givenDelay--;    //Decrease the given delay.
  }
}


void switchOn(int colour) {   //Switch on the colour responding to the number
  bufferList[bufferSize] = colour;    //Add the colour to the buffer list
  bufferSize++;   //Increase buffer size
  switch (colour) {   //Switch on the colour
    case 2: 
      redON();
      yellowOFF();
      blueOFF();
      greenOFF();
      break;
    case 3:
      redOFF();
      yellowON();
      blueOFF();
      greenOFF();
      break;
    case 4: 
      redOFF();
      yellowOFF();
      blueON();
      greenOFF();
      break;
    case 5:
      redOFF();
      yellowOFF();
      blueOFF();
      greenON();
      break;
    default:
      break;
  }
}

void save(String highScoreName, int highScore) {   //Saves high score and its owner to the EEPROM
  char nameLetters[15];
  for(int i = 0; i < 15; i++) {   //Splits string to char array
    if(i < highScoreName.length()) {
      nameLetters[i] = highScoreName[i];
    } else {
      nameLetters[i] = '-';
    }
  }
  Object hsObj = {   //Creates struckt-object for easier saving saving and reading
    highScore,
    highScoreName.length(),
    nameLetters[0],
    nameLetters[1],
    nameLetters[2],
    nameLetters[3],
    nameLetters[4],
    nameLetters[5],
    nameLetters[6],
    nameLetters[7],
    nameLetters[8],
    nameLetters[9],
    nameLetters[10],
    nameLetters[11],
    nameLetters[12],
    nameLetters[13],
    nameLetters[14]
  };
  EEPROM.put(0, hsObj);
}

void readHighScore() {    //Reads high score and its owner from EEPROM
  Object hsObj;
  EEPROM.get(0, hsObj);
  String readHsName = "";
  highScore = hsObj.score;
  int nameLength = hsObj.nameLength;
  while(true) {
    if(nameLength < 1) {break;}
    readHsName += hsObj.field1;
    if(nameLength < 2) {break;}
    readHsName += hsObj.field2;
    if(nameLength < 3) {break;}
    readHsName += hsObj.field3;
    if(nameLength < 4) {break;}
    readHsName += hsObj.field4;
    if(nameLength < 5) {break;}
    readHsName += hsObj.field5;
    if(nameLength < 6) {break;}
    readHsName += hsObj.field6;
    if(nameLength < 7) {break;}
    readHsName += hsObj.field7;
    if(nameLength < 8) {break;}
    readHsName += hsObj.field8;
    if(nameLength < 9) {break;}
    readHsName += hsObj.field9;
    if(nameLength < 10) {break;}
    readHsName += hsObj.field10;
    if(nameLength < 11) {break;}
    readHsName += hsObj.field11;
    if(nameLength < 12) {break;}
    readHsName += hsObj.field12;
    if(nameLength < 13) {break;}
    readHsName += hsObj.field13;
    if(nameLength < 14) {break;}
    readHsName += hsObj.field14;
    if(nameLength < 15) {break;}
    readHsName += hsObj.field15;
    break;
  }
  hsName = readHsName;
}

void showScore() {    //Shows player's score
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Game Over!");
  lcd.setCursor(0,1);
  lcd.print("Your score:");
  lcd.setCursor(12,1);
  lcd.print(score);
  delay(4000);
  lcd.clear();
}

void endGame() {    //Ends game and shows high score.
  allOFF();
  if(!firstTime) {   //Doesn't show personal score on startup.
    showScore();
    firstTime = false;
  }
  if(score > highScore) {    //If player's score is greater than high score then asks for name and saves variables.
    highScore = score;
    getName();
    save(hsName, highScore);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Top Score:");
  lcd.setCursor(13, 0);
  lcd.print(highScore);
  lcd.setCursor(0,1);
  lcd.print(hsName);
  while(pressed() == -1) {    //Waiting for button push.
    if(Serial.available() > 0) {    //Waiting for debug commands if serial is available.
      String command = Serial.readString();
      if(command.equals("reset")) {
        highScore = 0;
        hsName = "";
        save(hsName, highScore);
      } else if(command.equals("green on")) {
        greenON();
      } else if(command.equals("green off")) {
        greenOFF();
      } else if(command.equals("red on")) {
        redON();
      } else if(command.equals("red off")) {
        redOFF();
      } else if(command.equals("blue on")){
        blueON();
      } else if(command.equals("blue off")) {
        blueOFF();
      } else if(command.equals("yellow on")) {
        yellowON();
      } else if(command.equals("yellow off")) {
        yellowOFF();
      }
    }
   }

  lcd.clear();
  for(int i = 3; i > 0; i--) {    //Shows countdown.
    lcd.clear();
    lcd.setCursor(7,0);
    lcd.print(i);
    delay(1000);
  }
  latest = 0;
  bufferSize = 0;
  score = 0;
  changeDelay = 1000;
  int bufferList[50];
  showScore();
}


int getRandom(int latest) {    //Returns random colour that not being the same as last colour.
  int colour;
  while(true) {
    colour = random(4) + 2;
    if (colour != latest) {
      break;
    }
  }
  return colour;
}

void getName() {   //Function for name input.
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Name:");
  lcd.setCursor(0,1);

  hsName = "";
  int place = 0;
  int letter = -1;
  bool breaking = false;
  lcd.blink();
  while(true) {
    switch(pressed()) {
      case 2:   //Move to the next letter.
        letter++;
        if (letter == 26) {
          letter = 0;
        }
        printLetter(place, letter);
        break;
      case 3:   //Move to the previus letter.
        letter--;
        if(letter <= -1) {
          letter = 25;
        }
        printLetter(place, letter);
        break;
      case 4:   //Move to the next letter place
        place++;
        hsName += letters[letter];
        break;
      case 5:   //Enter
        hsName += letters[letter];
        breaking = true;
        lcd.noBlink();
      default:
        break;
    }
    if(breaking) {
      break;
    }
  }
}

void printLetter(int place, int letter) {    //Prints the chosen letter to the display.
  lcd.setCursor(place,1);
  lcd.print(letters[letter]);
}

void deleteLast() {   //Removes buffers last colour.
  for(int i=0; i < 50; i++) {
    bufferList[i] = bufferList[i+1];
  }
  bufferSize--;
}

void allOFF() {   //Switches off all colours.
  redOFF();
  yellowOFF();
  blueOFF();
  greenOFF();
}

void redON() {
  digitalWrite(redLED, HIGH);
}
void redOFF() {
  digitalWrite(redLED,LOW);
}
void yellowON() {
  digitalWrite(yellowLED, HIGH);
}
void yellowOFF() {
  digitalWrite(yellowLED, LOW);
}
void blueON() {
   digitalWrite(blueLED, HIGH);
}
void blueOFF() {
  digitalWrite(blueLED, LOW);
}
void greenON() {
  digitalWrite(greenLED, HIGH);
}
void greenOFF() {
  digitalWrite(greenLED, LOW);
}


int pressed() {   //Returns number responding to the button being pressed.
  if (digitalRead(red) == 1) {
    delay(20);
    while(digitalRead(red) != 0){}
    return redLED;
  }else if (digitalRead(yellow) == 1) {
    delay(20);
    while(digitalRead(yellow) != 0){}
    return yellowLED;
  }else if (digitalRead(blue) == 1) {
    delay(20);
    while(digitalRead(blue) != 0){}
    return blueLED;
  }else if (digitalRead(green) == 1) {
    delay(20);
    while(digitalRead(green) != 0){}
    return greenLED;
  }
  return -1;
}

void decreaseChangeDelay(int ms) {    //Decreases changeDelay.
  if(changeDelay - ms > 0) {
    changeDelay -= ms;
  }
}

void showCurrentScore(){   //Show current score  
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("STREAK");
  lcd.setCursor(6,1);
  lcd.print(score);
}
