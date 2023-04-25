//****************************************//
//* Example Code for Sending             *//
//* Signed Integers over I2C             *//
//* ESP32 (Master) to Arduino (Slave)    *//
//*                                      *//
//* Master Code                          *//
//*                                      *//
//* UoN 2022 - Nat Dacombe               *//
//****************************************//

// read through all of the code and the comments before asking for help
// research 'two's complement' if this is not familiar to you as it is used to represented signed (i.e. positive and negative) values



#include <Wire.h>
#define I2C_SLAVE_ADDR 0x04  // 4 in hexadecimal
#include <Keypad.h>

#define ROW_NUM 4     // four rows
#define COLUMN_NUM 3  // three columns
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(19, 23, 18, 17, 16, 15);


// the minimum and maximum values here are determined by the amount of bits used by the chosen variable type
// for int, this is either 16-bits or 32-bits
// due to two's complement, the minimum value is -2^(N-1), and the maximum is (2^(N-1))-1; where N is the number of bits
int LeftMotor = 0;
int RightMotor = 0;
int ServoAngle = 80;
char keySelect[1] = { '0' };

char keys[ROW_NUM][COLUMN_NUM] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};

byte pin_rows[ROW_NUM] = { 13, 12, 14, 26 };   // GIOP13, GIOP12, GIOP14, GIOP26 connect to the row pins
byte pin_column[COLUMN_NUM] = { 32, 33, 25 };  // GIOP32, GIOP33, GIOP25 connect to the column pins

//Create an object of keypad
Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

void setup() {
  Serial.begin(57500);
  Wire.begin();  // join i2c bus (address optional for the master) - on the Arduino NANO the default I2C pins are A4 (SDA), A5 (SCL)
  lcd.begin(16, 2);
  lcd.clear();
  // Print a message to the LCD.
  lcd.print("~ Starting ~");
  delay(2000);
}



void I2CValuesToSend(int LeftMotor, int RightMotor, int ServoAngle) {
  Wire.beginTransmission(I2C_SLAVE_ADDR);  // transmit to device #4
  Wire.write((byte)((LeftMotor & 0x0000FF00) >> 8));   // first byte of x, containing bits 16 to 9
  Wire.write((byte)(LeftMotor & 0x000000FF));          // second byte of x, containing the 8 LSB - bits 8 to 1
  Wire.write((byte)((RightMotor & 0x0000FF00) >> 8));  // first byte of y, containing bits 16 to 9
  Wire.write((byte)(RightMotor & 0x000000FF));         // second byte of y, containing the 8 LSB - bits 8 to 1
  Wire.write((byte)((ServoAngle & 0x0000FF00) >> 8));  // first byte of y, containing bits 16 to 9
  Wire.write((byte)(ServoAngle & 0x000000FF));
  Wire.endTransmission();
  delay(1000);
}

void MovementSelection(char* Keys1, int x) {
  if (Keys1[x] == '*') {
    ServoAngle = 0;
    LeftMotor = 10;
    RightMotor = -10;
    lcd.clear();
    lcd.print("Turning Left");
    delay(1500);
    I2CValuesToSend(LeftMotor, RightMotor, ServoAngle);
  } else if (Keys1[x] == '0') {
    ServoAngle = 170;
    LeftMotor = -10;
    RightMotor = 10;
    lcd.clear();
    lcd.print("Turning Right");
    delay(1500);
    I2CValuesToSend(LeftMotor, RightMotor, ServoAngle);
  } else if (Keys1[x] == '#') {
    ServoAngle = 80;
    LeftMotor = 0;
    RightMotor = 0;
    lcd.clear();
    lcd.print("Stopping");
    delay(1500);
    I2CValuesToSend(LeftMotor, RightMotor, ServoAngle);
  } else {
    for (int y = 0; y < Keys1[x]; y++) {
      ServoAngle = 80;
      LeftMotor = 250;
      RightMotor = 250;
      I2CValuesToSend(LeftMotor, RightMotor, ServoAngle);
      delay(100);
      I2CValuesToSend(0, 0, 80);
      delay(2000);
      lcd.clear();
      lcd.print("Maze Navigation");
      lcd.setCursor(0, 1);
      lcd.print("Selected");
      delay(1500);
    }
  }
}



void loop() {

  char Keys1[99];
  int Counter = 0;
  lcd.clear();
  lcd.print("Select : ");
  delay(1500);
  lcd.clear();
  lcd.print("1 : To Start");
  lcd.setCursor(0, 1);
  lcd.print("Maze Navigation");
  delay(1500);
  lcd.clear();
  lcd.print("2 :To Exit");
  delay(1500);
  do {
    keySelect[0] = keypad.getKey();
    Serial.println(keySelect);
    //switch (keySelect) {
    //  case '1':
    if (keySelect[0] == '1') {
      lcd.clear();
      lcd.print("Maze Navigation");
      lcd.setCursor(0, 1);
      lcd.print("Selected");
      delay(1500);
      lcd.clear();
      lcd.print("Press a number");
      lcd.setCursor(0, 1);
      lcd.print("Between 1 - 9");
      delay(2500);
      lcd.clear();
      lcd.print("To Move Foward");
      lcd.setCursor(0, 1);
      lcd.print("The Inputted ");
      delay(2500);
      lcd.clear();
      lcd.print("Number of Times");
      delay(2500);
      lcd.clear();
      lcd.print("Press : ");
      lcd.setCursor(0, 1);
      lcd.print("0 For Right");
      delay(2500);
      lcd.clear();
      lcd.print("* For Left");
      lcd.setCursor(0, 1);
      lcd.print("# to Start");
      delay(2500);
      lcd.clear();
      do {
        Keys1[Counter] = keypad.getKey();
        delay(1500);
        if (Keys1[Counter])
        {
          lcd.clear();
          lcd.print("Input Taken");
          delay(1500);
          lcd.clear();
          Counter = Counter + 1;
          Serial.println(Keys1[Counter]);
        }
      } while (Keys1[Counter] != '#');
      for (int x = 0; x < Counter; x++) {
        MovementSelection(Keys1, x);
        delay(150);
        // }
      }
    }
    //break;
    //case '2':
    else if (keySelect[0] == '2') {
      lcd.clear();
      lcd.print(" ~ Exiting ~ ");
      delay(1500);
      exit(0);
      //break;
    }
    //default:
    else {
      lcd.clear();
      lcd.print(" Please Enter a Valid Input ");
      delay(1500);
      //break;
    }
  } while ((keySelect[0] != '2') || (keySelect[0] != '1') || (keySelect[0] != '#'));
}


/*
  char key = keypad.getKey();// Read the key
  
  // Print if key pressed
  if (key){
    Serial.print("Key Pressed : ");
    Serial.println(key);
  }
  */
//}


//have a function to display the menu of onto the numpad screen and then ask for user input into which options to take
//In main loop use function and create a flag to reopen the menu after current task is complete

//have a function to take in user input for A: the menu B: the actual maze navigation system
//Finally Display the current task being completed when the maze is being navigatted on the numpad
