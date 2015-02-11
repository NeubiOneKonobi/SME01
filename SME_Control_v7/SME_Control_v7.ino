
//******************************************************************
// Title: Mega "SME"
// Name:  William Evans
// Date:  1/1/2015
// Version: 7
// Board: MEGA 2560
// Hardware: 
//     MEGA 2560, 
//     Arduino Nano
//     LCD (20X4)
//     
//     Cabinets, cables, I/O ports, relays, etc.
// Notes:
//*******************************************************************

//**********INCLUDES*******************************
#include <Keypad_MC17.h>
#include <Keypad.h>
#include <Wire.h>
#include <Tone1.h>
#include <LiquidCrystal.h>
#include <DS3231.h>

//**********DEFINITIONS/INITIALIZATIONS*************

//**** Misc Switching Relay Definitions ****
byte Etheron = B00000001; // Bit 1
byte RotorLight = B00000010; // Bit 2
byte DLFan = B00000100; // Bit 3
byte KAT500 = B00001000; // Bit 4
byte KPA500 = B00010000; // Bit 5
byte TwoMeterOut = B00100000; // Bit 6
byte P3Monitor = B01000000; // Bit 7
byte Extra = B10000000; // Bit 8
//**** Switching Relay Definitions ****
byte RY1 = B00000001; // Bit 1 (LSD)
byte RY2 = B00000010; // Bit 2
byte RY3 = B00000100; // Bit 3
byte RY4 = B00001000; // Bit 4
byte RY5 = B00010000; // Bit 5
byte RY6 = B00100000; // Bit 6
byte RY7 = B01000000; // Bit 7
byte RY8 = B10000000; // Bit 8

//**** Aux I/O Definitions ****
// byte VSense = B00010000; // Bit 5
int GPIOA = 0x12;
int GPIOB = 0x13;

int BCDRA=0, BCDRB=0, BCDRC=0, BCDRD=0; // Radio Antenna INPUT VARIABLES 
byte BCDE1, BCDE2; // Ethernet INPUT VARIABLES
byte BCDA1, BCDA2; // Aux INPUT Bytes

//**** Antenna Names Initialization****
String AntN1 = " FW Loop-", AntN2 = "  Dipole-"; // LCD Display Constants
String AntN3 = "        -", AntN4 = "  Dipole-"; // LCD Display Variables
String AntN5 = " Inv Vee-", AntN6 = "Hex Beam-"; // LCD Display Variables
String AntN7 = "        -", AntN8 = "        -"; // LCD Display Variables

//**** Misc Variables Initialization****
int AltAflag = 0;
int AltBflag = 0;
int DLAflag = 0;
int DLBflag = 0;
int ManAflag = 0; 
int ManBflag = 0;
int PWSwitch = 0;
int Etherflag = 0;
int SetClockUpFlag = 0;
int SetClockDownFlag = 0;

//**** PIN ASSIGNMENTS *****
int speakerpin = 3;
// ***** LCD PIN ASSIGNMENTS ****
int RS = 14;
int E = 15;
int D4 = 16;
int D5 = 17;
int D6 = 18;
int D7 = 19;
// **** LED PIN ASSIGNMENTS ****
//int ManALED = 5;
//int AltALED = 6;
//int DLALED = 7;
//int Aux1LED = 8;
//int ManBLED = 9;
//int AltBLED = 10;
//int DLBLED = 11;
//int Aux2LED = 12;


//****Antenna masks initialization****
byte AntMask0 = B00000000, AntMask1 = B00000001, AntMask2 = B00000010; // Masks for setting antennas
byte AntMask3 = B00000100, AntMask4 = B00001000, AntMask5 = B00010000; // Masks for setting antennas
byte AntMask6 = B00100000, AntMask7 = B01000000, AntMask8 = B10000000; // Masks for setting antennas

//****Antenna Selection Initialization****
struct ANT {// For Manual Switching
  String AntName; 
  byte PortMask;   
};
// Construct instances (members) of structure:
ANT AntA = {
  AntN1, AntMask1}; // Antenna connected to Antenna Switch  port 1
ANT AntB = {
  AntN2, AntMask2}; // Antenna connected to Antenna Switch  port 2
ANT AntC = {
  AntN3, AntMask3}; // Antenna connected to Antenna Switch  port 3
ANT AntD = {
  AntN4, AntMask4}; // Antenna connected to Antenna Switch  port 4
ANT AntE = {
  AntN5, AntMask5}; // Antenna connected to Antenna Switch  port 5
ANT AntF = {
  AntN6, AntMask6}; // Antenna connected to Antenna Switch  port 6
ANT AntG = {
  AntN7, AntMask7}; // Antenna connected to Antenna Switch  port 7
ANT AntH = {
  AntN8, AntMask8}; // Antenna connected to Antenna Switch  port 8

//**** Assign NORMAL Antenna to Bands ****
ANT BandN160 = AntA; // 160 Meters == B0000
ANT BandN80 = AntB; //80 Meters == B0001
ANT BandN60 = AntA; // 60 Meters == B0010
ANT BandN40 = AntD; // 40 Meters == B0011
ANT BandN30 = AntA; //30 Meters == B0100
ANT BandN20 = AntF; // 20 Meters == B0101
ANT BandN17 = AntF; // 17 Meters == B0110
ANT BandN15 = AntF; //15 Meters == B0111
ANT BandN12 = AntF; // 12 Meters == B1000
ANT BandN10 = AntF; //10 Meters == B1001
ANT BandN6 = AntF; // 6 Meters == B1010

//**** Assign Alt Antenna to Bands ****
ANT BandAlt160 = AntA; // 160 Meters == B0000
ANT BandAlt80 = AntB; //80 Meters == B0001
ANT BandAlt60 = AntA; // 60 Meters == B0010
ANT BandAlt40 = AntE; // 40 Meters == B0011
ANT BandAlt30 = AntE; //30 Meters == B0100
ANT BandAlt20 = AntE; // 20 Meters == B0101
ANT BandAlt17 = AntE; // 17 Meters == B011
ANT BandAlt15 = AntE; //15 Meters == B0111
ANT BandAlt12 = AntE; // 12 Meters == B1000
ANT BandAlt10 = AntE; //10 Meters == B1001
ANT BandAlt6 = AntE; // 6 Meters == B1010

//****Switching Relay Name initialization
//String RLY1 = "RLY1", RLY2 = "RLY2", RLY3 = "RLY3", RLY4 = "RLY4"; // variable for printing
//String RLY5 = "RLY5", RLY6 = "RLY6", RLY7 = "RLY7", RLY8 = "RLY8"; // variable for printing
//String  RLY9 = "RLY9", RLY10 = "RLY10", RLY11 = "RLY11", RLY12 = "RLY12"; // variable for printing
//String  RLY13 = "RLY13", RLY14 = "RLY14", RLY15 = "RLY15", RLY16 = "RLY16"; // variable for printing

//****I2C ADDRESS INITIALIZATION
int AntAdr = 0x20; // i2C address for Antenna OUTPUTS
int RadAdr = 0x21; // i2C address for Radio Inputs
int LEDAddr = 0x22; // LED Readout for Antenna in Use
int KPAdr = 0x23; // i2C address for keypad
int AuxAdr = 0x24; // i2C address for Aux I/O
int SwReAdr= 0x25; // i2C address for Switching Relays
int EthAdr = 0x26; // i2C address for ethernet data in
int ClockAdr = 0x22//int LCDAdr = 7; //I2C address for Adafruit Library LCD == 0x27

//***** -- Keypad initialization
const byte ROWS = 4; //four rows of custom Keypad 1
const byte COLS = 8; //Eight columns of custom Keypad 1
//Next, define the symbols on the buttons of the keypads:
//(I just numbered them sequentially - the final translation is done in the case statements)
char keys[ROWS][COLS] = {
  {
    '1','2','3','4','5','6','7','8'                  }
  ,
  {
    'A','B','C','D','E','F','G','H'                  }
  ,
  {
    'I','J','K','L','M','N','O','P'                  }
  ,
  {
    'Q','R','S','T','U','V','W','X'                  }
};
byte rowPins[ROWS] = {
  0, 1, 2, 3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {
  4, 5, 6, 7, 8, 9, 10, 11}; //connect to the column pinouts of the keypad
//initialize an instance of class NewKeypad
Keypad_MC17  kpd = Keypad_MC17( makeKeymap(keys), rowPins, colPins, ROWS, COLS, KPAdr); 
//char key; // variable for checking if key is pressed
char lastAkeypressed;// variable for missing key presses
char lastBkeypressed;// variable for missing key presses
char lastCkeypressed;
int keyflag = 0;

LiquidCrystal lcd(RS, E, D4, D5, D6, D7);// straight parallel LCD (RS, E, D4, D5, D6, D7)
//LiquidCrystal lcd(0);
//**********INITIAL SETUP****************************
void setup()
{

  //Serial.begin(57600); // initialize Serial port
  Wire.begin();// initialize the I2C line
  lcd.begin(20,4);

  // **** Initialize Keypad ****
  kpd.begin();

  // ****Setup MCP23017s as input or output****

  Wire.beginTransmission(AntAdr); // I2C bus address for Outputs to Antennas
  Wire.write(0x00); // IODIRA register
  Wire.write(0x00); // set all of port A to outputs
  Wire.endTransmission();
  Wire.beginTransmission(AntAdr);
  Wire.write(0x01); // IODIRB register
  Wire.write(0x00); // set all of port B to outputs
  Wire.endTransmission();

  Wire.beginTransmission(SwReAdr); // I2C bus address for Outputs to Switching Relays
  Wire.write(0x00); // IODIRA register
  Wire.write(0x00); // set all of port A to outputs
  Wire.endTransmission();
  // RLY1 = 
  Wire.beginTransmission(SwReAdr);
  Wire.write(0x01); // IODIRB register
  Wire.write(0x00); // set all of port B to outputs
  Wire.endTransmission();

  Wire.beginTransmission(RadAdr); // I2C bus address for MAIN RADIO INPUTS
  Wire.write(0x00); // IODIRA register
  Wire.write(0xFF); // set all of port A to Inputs
  Wire.endTransmission();
  Wire.beginTransmission(RadAdr);
  Wire.write(0x01); // IODIRB register
  Wire.write(0xFF); // set all of port B to Inputs
  Wire.endTransmission();

    Wire.beginTransmission(KPAdr); // I2C bus address for Keypad INPUTS
    Wire.write(0x00); // IODIRA register
    Wire.write(0xFF); // set all of port A to Inputs
    Wire.endTransmission();
    Wire.beginTransmission(KPAdr);
    Wire.write(0x01); // IODIRB register
    Wire.write(0xFF); // set all of port B to Inputs
    Wire.endTransmission();

  //Uncomment for Extra Expansion Ports 
  Wire.beginTransmission(AuxAdr); // I2C bus address for Misc INPUTS/OUTPUTS
  Wire.write(0x00); // IODIRA register
  Wire.write(0x00); // set all of port A to OUTPUTS
  Wire.endTransmission();
  Wire.beginTransmission(AuxAdr);
  Wire.write(0x01); // IODIRB register
  Wire.write(0x00); // set all of port B to OUTPUTS
  Wire.endTransmission();

  Wire.beginTransmission(EthAdr); // I2C bus address for Ethernet INPUTS
  Wire.write(0x00); // IODIRA register
  Wire.write(0xFF); // set all of port A to Inputs
  Wire.endTransmission();
  Wire.beginTransmission(EthAdr);
  Wire.write(0x01); // IODIRB register
  Wire.write(0xFF); // set all of port B to Inputs
  Wire.endTransmission();

  // ****Initialize LCD ****
  lcd.begin(20,4);
  delay (30);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" MEGA SENIOR MOMENT ");
  //delay(2000);
  for (int i=0; i<8; i++){
    lcd.setCursor(0,1);
    lcd.print("  ** ELIMINATOR **  ");
    delay(200);
    lcd.setCursor(0,1);
    lcd.print("                    ");
    delay(200);       
  }//for
  lcd.setCursor(0,1);
  lcd.print("  ** ELIMINATOR **  ");
  //delay(2000);
  lcd.clear();


  // *****************Initial conditions **************
  PWSwitch = digitalRead(22); // check if Radio turned on!
  Etherflag = GetBit(B10000000,EthAdr,GPIOB);  // get etherflag as either 1 or 0
  //    if(PWSwitch == 1){
  //        SetBit(RotorLight, 0x26, GPIOB, 0); // Turn on Rotor
  //        SetBit( DLFan, SwReAdr, GPIOB, 1); // Turn off DLFan
  //        //SetBit( EtherOn,SwReAdr, GPIOB,1); // Turn on Ethernet 12V -- always on
  //        SetBit( KAT500, SwReAdr, GPIOB, 0); // Turn on KAT500
  //
  //            // ****Get all Data
  //        BCDRA = bcdToDec(GetInputs(RadAdr, GPIOA),1); // get Radios A inputs & Convert to decimal
  //        BCDRB = bcdToDec(GetInputs(RadAdr, GPIOA),2);// get Radio B inputs & Convert to decimal
  //        BCDRC = bcdToDec(GetInputs(RadAdr, GPIOB),1); // get Radios C inputs & Convert to decimal
  //        BCDRD = bcdToDec(GetInputs(RadAdr, GPIOB),2);// get Radio D inputs & Convert to decimal
  //        BCDA1 = GetInputs(AuxAdr, GPIOA); // get Aux A inputs
  //        BCDA2 = GetInputs(AuxAdr, GPIOB); // get Aux B inputs
  //        BCDE1 = GetInputs(EthAdr, GPIOA); // get Ethernet 1 inputs  
  //        BCDE2 = GetInputs(EthAdr, GPIOB); // Get Ethernet 2 inputs 
  //        lcd.setCursor(0,1);
  //        lcd.print(Etherflag);
  //        lcd.print("-");
  //        lcd.print(PWSwitch);
  //        lcd.print("-");
  //        lcd.print(BCDE1);
  //        lcd.print("-");
  //        lcd.print(BCDE2);
  //
  //    } // end if(PWSwitch ==1)



} // end void setup()


//**********SUB ROUTINES*******************************
void toneup() {
  int melody[] = {
    NOTE_DS7, NOTE_DS7, NOTE_DS7, NOTE_G7  };
  int noteDurations[] = {
    12,12,12,4   };
  for (int thisNote = 0; thisNote < 4; thisNote++) {
    int noteDuration = 1000/noteDurations[thisNote];
    tone(speakerpin, melody[thisNote],noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(speakerpin);
  }
}

void tonedown() {
  int melody[] = {
    NOTE_G7,NOTE_G7,NOTE_G7,NOTE_DS7  };
  int noteDurations[] = {
    12,12,12,4   };
  for (int thisNote = 0; thisNote < 4; thisNote++) {
    int noteDuration = 1000/noteDurations[thisNote];
    tone(speakerpin, melody[thisNote],noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(speakerpin);
  }
}

void buttonpushA() {
  tone(speakerpin, NOTE_C7,30);
  delay(40);
  noTone(speakerpin);
}

void buttonpushB() {
  tone(speakerpin, NOTE_C8,30);
  delay(40);
  noTone(speakerpin);
}

void buttonpushC() {
  tone(speakerpin, NOTE_F7,30);
  delay(40);
  noTone(speakerpin);
}

byte GetInputs(int address, int ptr) {
  Wire.beginTransmission(address);
  Wire.write(ptr);
  Wire.endTransmission();
  Wire.requestFrom(address, 1); // request one byte of data from MCP20317
  byte inputs1=Wire.read(); 
  return inputs1; 
}

byte bcdToDec(byte value, int rank)
{
  byte Temp=B00000000;
  if (rank==1) // wanted upper nibble
  {
    Temp = value & B11110000; // get upper nibble ( value 0-15)
    Temp >>=4;  // move upper nibble four places to right to get rid of lower nibble
}
  else if (rank == 2)// wanted lower nibble
  {
    Temp = value & B00001111; // get lower nibble and drop upper nibble (value 0-15)
  }
return Temp;
} // end bcdToDec(byte value, int rank)

void SetBit(byte MaskForBit, int AdrofChip, int PortNumber, int ONOrOff)
{
  // First, get present byte:
  byte presentValue = GetInputs(AdrofChip, PortNumber);
  // Next, set bit of interest to either 1 or 0:
  if (ONOrOff == 0) {// to set bit to 0
    byte x = MaskForBit ^ B11111111; // XOR to reverse the mask    
    Wire.beginTransmission(AdrofChip); // choose correct chip
    Wire.write(PortNumber); // port A or B
    Wire.write(x & presentValue ); //AND the byte to set ONLY the one bit in question to 0
    Wire.endTransmission();
  } // if (ONOrOff == 0)
  else if (ONOrOff == 1){   
    Wire.beginTransmission(AdrofChip); // choose correct chip
    Wire.write(PortNumber); // port A or B
    Wire.write(MaskForBit | presentValue); // OR the byte to set ONLY the one bit in question to 1
    Wire.endTransmission();
  } // if (ONOrOff == 1)
}// void SetBit()

int GetBit (byte name, int adr, int reg) 
{
  int mybit1;
  Wire.beginTransmission(adr); // choose correct chip 
  Wire.write(reg);// port A or B
  Wire.endTransmission();
  Wire.requestFrom(adr, 1);// request 1 byte 
  byte inputs = Wire.read(); // store byte in "inputs"
  inputs = inputs & name; // AND the byte with the mask to get the bit
  if (inputs > 0) { 
    mybit1 = 1; 
  }   // bit is 1
  else {
    mybit1 = 0; // bit is 0
  }
  return mybit1;
}// int GetBit()

void SetByte (byte newByte, int Address, int portnumber)
{
  Wire.beginTransmission(Address); // choose correct chip
  Wire.write(portnumber); // port A or B
  Wire.write(newByte); // 
  Wire.endTransmission();

}// void SetByte()

String GetBand(int BCDN) {
  String band;
  switch (BCDN) {
  case 0:
    band = " 60-";
    break;
  case 1:
    band = "160-";
    break;
  case 2:
    band = " 80-";
    break;
  case 3:
    band = " 40-";
    break;
  case 4:
    band = " 30-";
    break;
  case 5:
    band = " 20-";
    break;
  case 6:
    band = " 17-";
    break;
  case 7:
    band = " 15-";
    break;
  case 8:
    band = " 12-";
    break;
  case 9:
    band = " 10-";
    break;
  case 10:
    band = "  6-";
    break;
  default:
    band = "oops";

  }
  return band;
}


void dokey(char a)
{
  String band;
  int BCDN;
  a = int(a);
  if ((a > 48) && (a < 57)) {
    lastAkeypressed = a; 
  }
  if ((a > 64) && (a < 73)) {
    lastBkeypressed = a; 
  }
  if ((a > 72) && (a < 89)) {
    lastCkeypressed = a; 
  }

  switch (a) {
  case 49://1 - AntA1
    ManAflag = 1;
    digitalWrite(ManALED, HIGH);
    SetByte(AntMask1, AntAdr, GPIOA); // Set Antenna A to Antenna #1
    SetByte ( B0110000, LEDAddr, GPIOA); // Set AntLEDA to "1"
    SetAntA(1, 0); // set antA1, Normal
    BCDN = bcdToDec(GetInputs(RadAdr,GPIOA),1); // get Radios A inputs & Convert to decimal  
    band = GetBand(BCDN);
    lcd.setCursor(3,0);
    lcd.print(band);
    lcd.setCursor(15,0);
    lcd.print("MAN-");
    buttonpushA();
    break;
  case 50://2 - AntA2
    ManAflag = 1;
    digitalWrite(ManALED, HIGH);
    SetByte(AntMask2, AntAdr, GPIOA);
    SetByte ( B1101101, LEDAddr, GPIOA); // Set AntLEDA to "2"
    SetAntA(2,0); // set antA2, Normal
    BCDN = bcdToDec(GetInputs(RadAdr, GPIOA),1); // get Radios A inputs & Convert to decimal  
    band = GetBand(BCDN);
    lcd.setCursor(3,0);
    lcd.print(band);
    lcd.setCursor(15,0);
    lcd.print("MAN-");
    buttonpushA();
    break;
  case 51://3 - AntA3
    ManAflag = 1;
    digitalWrite(ManALED, HIGH);
    SetByte(AntMask3, AntAdr, GPIOA); // Set Antenna A to Antenna #3
    SetByte ( B1111001, LEDAddr, GPIOA); // Set AntLEDA to "3"
    SetAntA(3,0); // set antA2, Normal
    BCDN = bcdToDec(GetInputs(RadAdr, GPIOA),1); // get Radios A inputs & Convert to decimal  
    band = GetBand(BCDN);
    lcd.setCursor(3,0);
    lcd.print(band);
    lcd.setCursor(15,0);
    lcd.print("MAN-");
    buttonpushA();
    break;
  case 52://4 - AntA4
    ManAflag = 1;
    digitalWrite(ManALED, HIGH);
    SetByte(AntMask4, AntAdr, GPIOA);
    BCDN = bcdToDec(GetInputs(RadAdr, GPIOA),1); // get Radios A inputs & Convert to decimal  
    band = GetBand(BCDN);
    lcd.setCursor(3,0);
    lcd.print(band);
    lcd.setCursor(15,0);
    lcd.print("MAN-");
    buttonpushA();
    break;
  case 53://5 - AntA5
    ManAflag = 1;
    digitalWrite(ManALED, HIGH);
    SetByte(AntMask5, AntAdr, GPIOA);
    BCDN = bcdToDec(GetInputs(RadAdr, GPIOA),1); // get Radios A inputs & Convert to decimal  
    band = GetBand(BCDN);
    lcd.setCursor(3,0);
    lcd.print(band);
    lcd.setCursor(15,0);
    lcd.print("MAN-");
    buttonpushA();
    break;
  case 54://6 - AntA6
    ManAflag = 1;
    digitalWrite(ManALED, HIGH);
    SetByte(AntMask6, AntAdr, GPIOA);
    BCDN = bcdToDec(GetInputs(RadAdr, GPIOA),1); // get Radios A inputs & Convert to decimal  
    band = GetBand(BCDN);
    lcd.setCursor(3,0);
    lcd.print(band);
    lcd.setCursor(15,0);
    lcd.print("MAN-");
    buttonpushA();
    break;
  case 55://7 - AntA7
    ManAflag = 1;
    digitalWrite(ManALED, HIGH);
    SetByte(AntMask7, AntAdr, GPIOA);
    BCDN = bcdToDec(GetInputs(RadAdr, GPIOA),1); // get Radios A inputs & Convert to decimal  
    band = GetBand(BCDN);
    lcd.setCursor(3,0);
    lcd.print(band);
    lcd.setCursor(15,0);
    lcd.print("MAN-");
    buttonpushA();
    break;
  case 56://8 - AntA8
    ManAflag = 1;
    digitalWrite(ManALED, HIGH);
    SetByte(AntMask8, AntAdr, GPIOA);
    BCDN = bcdToDec(GetInputs(RadAdr, GPIOA),1); // get Radios A inputs & Convert to decimal  
    band = GetBand(BCDN);
    lcd.setCursor(3,0);
    lcd.print(band);
    lcd.setCursor(15,1);
    lcd.print("MAN-");
    buttonpushA();
    break;
  case 65: //A--AntB1
    ManBflag = 1;
    digitalWrite(ManBLED, HIGH);
    SetByte(AntMask1, AntAdr, GPIOB);
    BCDN = bcdToDec(GetInputs(RadAdr, GPIOA),2); // get Radios B inputs & Convert to decimal  
    band = GetBand(BCDN);
    lcd.setCursor(3,1);
    lcd.print(band);
    lcd.setCursor(15,1);
    lcd.print("MAN-");
    buttonpushB();
    break;
  case 66: //B-AntB2
    ManBflag = 1;
    digitalWrite(ManBLED, HIGH);
    SetByte(AntMask2, AntAdr, GPIOB);
    BCDN = bcdToDec(GetInputs(RadAdr, GPIOA),2); // get Radios B inputs & Convert to decimal  
    band = GetBand(BCDN);
    lcd.setCursor(3,1);
    lcd.print(band);
    lcd.setCursor(15,1);
    lcd.print("MAN-");
    buttonpushB();
    break;
  case 67: //C-AntB3
    ManBflag = 1;
    digitalWrite(ManBLED, HIGH);
    SetByte(AntMask3, AntAdr, GPIOB);
    BCDN = bcdToDec(GetInputs(RadAdr, GPIOA),2); // get Radios B inputs & Convert to decimal  
    band = GetBand(BCDN);
    lcd.setCursor(3,1);
    lcd.print(band);
    lcd.setCursor(15,1);
    lcd.print("MAN-");
    buttonpushB();
    break;
  case 68: //D-AntB4
    ManBflag = 1;
    digitalWrite(ManBLED, HIGH);
    SetByte(AntMask4, AntAdr, GPIOB);
    BCDN = bcdToDec(GetInputs(RadAdr, GPIOA),2); // get Radios B inputs & Convert to decimal  
    band = GetBand(BCDN);
    lcd.setCursor(3,1);
    lcd.print(band);
    lcd.setCursor(15,1);
    lcd.print("MAN-");
    buttonpushB();
    break;
  case 69: //E-AntB5
    ManBflag = 1;
    digitalWrite(ManBLED, HIGH);
    SetByte(AntMask5, AntAdr, GPIOB);
    BCDN = bcdToDec(GetInputs(RadAdr, GPIOA),2); // get Radios B inputs & Convert to decimal  
    band = GetBand(BCDN);
    lcd.setCursor(3,1);
    lcd.print(band);
    lcd.setCursor(15,1);
    lcd.print("MAN-");
    buttonpushB();
    break;
  case 70: //F-AntB6
    ManBflag = 1;
    digitalWrite(ManBLED, HIGH);
    SetByte(AntMask6, AntAdr, GPIOB);
    BCDN = bcdToDec(GetInputs(RadAdr, GPIOA),2); // get Radios B inputs & Convert to decimal  
    band = GetBand(BCDN);
    lcd.setCursor(3,1);
    lcd.print(band);
    lcd.setCursor(15,1);
    lcd.print("MAN-");
    buttonpushB();
    break;
  case 71: //G-AntB7
    ManBflag = 1;
    digitalWrite(ManBLED, HIGH);
    SetByte(AntMask7, AntAdr, GPIOB);
    BCDN = bcdToDec(GetInputs(RadAdr, GPIOA),2); // get Radios B inputs & Convert to decimal  
    band = GetBand(BCDN);
    lcd.setCursor(3,1);
    lcd.print(band);
    lcd.setCursor(15,1);
    lcd.print("MAN-");
    buttonpushB();
    break;
  case 72: //H-AntB8
    ManBflag = 1;
    digitalWrite(ManBLED, HIGH);
    SetByte(AntMask8, AntAdr, GPIOB);
    BCDN = bcdToDec(GetInputs(RadAdr, GPIOA),2); // get Radios B inputs & Convert to decimal  
    band = GetBand(BCDN);
    lcd.setCursor(3,1);
    lcd.print(band);
    lcd.setCursor(15,1);
    lcd.print("MAN-");
    buttonpushB();
    break;
  case 73: // I--Auto A
    if (ManAflag == 1) { // Turn off MAN flag
      ManAflag = 0;
      digitalWrite(ManALED, LOW);
      lcd.setCursor(15,0);
      lcd.print("AUT-");
    }
    //    else {
    //      ManAflag = 0;
    //      digitalWrite(ManALED,HIGH);
    //    }
    buttonpushC();
    break;
  case 74: // J-AltA
    // if AltAflag is not on, then turn it on, else turn off
    if (AltAflag == 0)
    {
      AltAflag = 1;
      digitalWrite(AltALED, HIGH);
    }
    else {
      AltAflag = 0;
      digitalWrite(AltALED,LOW);
    }
    buttonpushC();
    break;

  case 75: // K-DLA
    if(DLAflag == 0) {
      DLAflag = 1;
      digitalWrite(DLALED, HIGH);
    }
    else{
      DLAflag = 0;
      digitalWrite(DLALED, LOW);
    }
    buttonpushC();
    break;
  case 76: // L-AUXA
    if (PWSwitch == 0) { // temp test
      PWSwitch = 1;
    }
    else {
      PWSwitch = 0;
    }

    buttonpushC();
    break;
  case 77: // M--Toggle set clock up
    if(SetClockUpFlag == 0) {
      SetClockUpFlag=1;
      digitalWrite(SetClockUp, HIGH);
      toneup();
    }
    else if(SetClockUpFlag == 1) {
      SetClockUpFlag=0;
      digitalWrite(SetClockUp, LOW);
      tonedown();
    }
    break;
  case 78: // N - set seconds
    digitalWrite(Seconds, HIGH);
    delay(10); // adjust via trial and error
    digitalWrite(Seconds, LOW);
    buttonpushC();
    break;
  case 79: // O - set minutes
    digitalWrite(Minutes, HIGH);
    delay(10); // adjust via trial and error
    digitalWrite(Minutes, LOW);
    buttonpushC();
    break;
  case 80: // P - set hours
    digitalWrite(Hours, HIGH);
    delay(10); // adjust via trial and error
    digitalWrite(Hours, LOW);
    buttonpushC();
    break;    
  case 81://Q -- AutoB
    if (ManBflag == 1) { // Turn off MAN flag
      ManBflag = 0;
      digitalWrite(ManBLED, LOW);
      lcd.setCursor(15,1);
      lcd.print("AUT-");
    }
    buttonpushC();
    break;
  case 82: // R -- AltB
    // if AltBflag is not on, then turn it on, else turn off
    if (AltBflag == 0)
    {
      AltBflag = 1;
      digitalWrite(AltBLED, HIGH);
    }
    else {
      AltBflag = 0;
      digitalWrite(AltBLED,LOW);
    }
    buttonpushC();
    break;

  case 83: // S-DLB    // If Dummy Load B flag is off, turn it on, else turn it off
    if(DLBflag == 0) {
      DLBflag = 1;
      digitalWrite(DLBLED, HIGH);
    }
    else{
      DLBflag = 0;
      digitalWrite(DLBLED, LOW);
    }
    buttonpushC();
    break;
  case 84: // T-AUXB
    buttonpushC();
    break;
  case 85: // U --Toggle set clock down  ClockSetDownFlag
    if(SetClockDownFlag == 0) {
      SetClockDownFlag=1;
      digitalWrite(SetClockDown, HIGH);
      toneup();
    }
    else if(SetClockDownFlag == 1) {
      SetClockDownFlag=0;
      digitalWrite(SetClockDown, LOW);
      tonedown();
    }
    break;
  case 86: // V --set day
    digitalWrite(Days, HIGH);
    delay(10); // adjust via trial and error
    digitalWrite(Days, LOW);
    buttonpushC();
    break;
  case 87: // W - set month
    digitalWrite(Months, HIGH);
    delay(10); // adjust via trial and error
    digitalWrite(Months, LOW);
    buttonpushC();
    break;
  case 88: // X - set year
    digitalWrite(Years, HIGH); 
    delay(10); // adjust via trial and error
    digitalWrite(Years, LOW);
    buttonpushC();
    break;    

  } // end switch
}



//void SetEtherAntenna(){
//  if (GetBit(B00000001, EthAdr, GPIOA)==0){
//    SetAntA(1, AltAflag);
//  } // endif
//  else if (GetBit(B00000010, EthAdr, GPIOA)==0){
//    SetAntA(2, AltAflag);
//  } // endif
//  else if (GetBit(B00000100, EthAdr, GPIOA)==0){
//    SetAntA(3, AltAflag);
//  } // endif
//  else if (GetBit(B00001000, EthAdr, GPIOA)==0){
//    SetAntA(4, AltAflag);
//  } // endif
//  else if (GetBit(B00010000, EthAdr, GPIOA)==0){
//    SetAntA(5, AltAflag);
//  } // endif
//  else if (GetBit(B00100000, EthAdr, GPIOA)==0){
//    SetAntA(6, AltAflag);
//  } // endif
//  else if (GetBit(B01000000, EthAdr, GPIOA)==0){
//    SetAntA(7, AltAflag);
//  } // endif
//  else if (GetBit(B10000000, EthAdr, GPIOA)==0){
//    SetAntA(8, AltAflag);
//
//  } // endif
//} //end  SetEtherAntenna()


 
void SetEtherRelays(){
  if (GetBit(B00000001,EthAdr,GPIOB) == 1) {
    // Set Relay in question
  }
  if (GetBit(B00000010,EthAdr,GPIOB) == 1) {
    // Set Relay in question
  }
  if (GetBit(B00000100,EthAdr,GPIOB) == 1) {
    // Set Relay in question
  }
  if (GetBit(B00001000,EthAdr,GPIOB) == 1) {
    // Set Relay in question
  }
  if (GetBit(B00010000,EthAdr,GPIOB) == 1) {
    // Set Relay in question
  }
  if (GetBit(B00100000,EthAdr,GPIOB) == 1) {
    // Set Relay in question
  }
  if (GetBit(B01000000,EthAdr,GPIOB) == 1) {
    // Set Relay in question
  }
} // end SetEtherRelays()

void SetAntA(int inputA, int AltAflag) {
  switch (inputA) {
  case 0: // 60 Meters
    lcd.setCursor(0,0);
    lcd.print("A: 60-");
    if (AltAflag == 0){
      SetByte (AntAdr,  GPIOA, BandN60.PortMask);
      lcd.print(BandN60.AntName);
      lcd.setCursor(19,0);
      lcd.print("N");
    }
    else { 
      SetByte (AntAdr,  GPIOA, BandAlt60.PortMask);
      lcd.print(BandAlt60.AntName);
      lcd.setCursor(19,0);
      lcd.print("A");
    }
    break;
  case 1: // 160 Meters
    lcd.setCursor(0,0);
    lcd.print("A:160-");
    if (AltAflag == 0){
      SetByte (AntAdr,  GPIOA, BandN160.PortMask);
      lcd.print(BandN160.AntName);
      lcd.setCursor(19,0);
      lcd.print("N");
    }
    else { 
      SetByte (AntAdr,  GPIOA, BandAlt160.PortMask);
      lcd.print(BandAlt160.AntName);
      lcd.setCursor(19,0);
      lcd.print("A");
    }
    break;
  case 2: // 80 Meters
    lcd.setCursor(0,0);
    lcd.print("A: 80-");
    if (AltAflag == 0){
      SetByte (AntAdr,  GPIOA, BandN80.PortMask);
      lcd.print(BandN80.AntName);
      lcd.setCursor(19,0);
      lcd.print("N");
    }
    else { 
      SetByte (AntAdr,  GPIOA, BandAlt80.PortMask);
      lcd.print(BandAlt80.AntName);
      lcd.setCursor(19,0);
      lcd.print("A");
    }
    break;
  case 3: // 40 Meters
    lcd.setCursor(0,0);
    lcd.print("A: 40-");
    if (AltAflag == 0){
      SetByte (AntAdr,  GPIOA, BandN40.PortMask);
      lcd.print(BandN40.AntName);
      lcd.setCursor(19,0);
      lcd.print("N");
    }
    else { 
      SetByte (AntAdr,  GPIOA, BandAlt40.PortMask);
      lcd.print(BandAlt40.AntName);
      lcd.setCursor(19,0);
      lcd.print("A");
    }
    break;
  case 4: // 30 Meters
    lcd.setCursor(0,0);
    lcd.print("A: 30-");
    if (AltAflag == 0){
      SetByte (AntAdr,  GPIOA, BandN30.PortMask);
      lcd.print(BandN30.AntName);
      lcd.setCursor(19,0);
      lcd.print("N");
    }
    else { 
      SetByte (AntAdr,  GPIOA, BandAlt30.PortMask);
      lcd.print(BandAlt30.AntName);
      lcd.setCursor(19,0);
      lcd.print("A");
    }
    break;
  case 5: // 20 Meters
    lcd.setCursor(0,0);
    lcd.print("A: 20-");
    if (AltAflag == 0){
      SetByte (AntAdr,  GPIOA, BandN20.PortMask);
      lcd.print(BandN20.AntName);
      lcd.setCursor(19,0);
      lcd.print("N");
    }
    else { 
      SetByte (AntAdr,  GPIOA, BandAlt20.PortMask);
      lcd.print(BandAlt20.AntName);
      lcd.setCursor(19,0);
      lcd.print("A");
    }
    break;
  case 6: // 17 Meters
    lcd.setCursor(0,0);
    lcd.print("A: 17-");
    if (AltAflag == 0){
      SetByte (AntAdr,  GPIOA, BandN17.PortMask);
      lcd.print(BandN17.AntName);
      lcd.setCursor(19,0);
      lcd.print("N");
    }
    else { 
      SetByte (AntAdr,  GPIOA, BandAlt17.PortMask);
      lcd.print(BandAlt17.AntName);
      lcd.setCursor(19,0);
      lcd.print("A");
    }
    break;
  case 7: // 15 Meters
    lcd.setCursor(0,0);
    lcd.print("A: 15-");
    if (AltAflag == 0){
      SetByte (AntAdr,  GPIOA, BandN15.PortMask);
      lcd.print(BandN15.AntName);
      lcd.setCursor(19,0);
      lcd.print("N");
    }
    else { 
      SetByte (AntAdr,  GPIOA, BandAlt15.PortMask);
      lcd.print(BandAlt15.AntName);
      lcd.setCursor(19,0);
      lcd.print("A");
    }
    break;
  case 8: // 12 Meters
    lcd.setCursor(0,0);
    lcd.print("A: 12-");
    if (AltAflag == 0){
      SetByte (AntAdr,  GPIOA, BandN12.PortMask);
      lcd.print(BandN12.AntName);
      lcd.setCursor(19,0);
      lcd.print("N");
    }
    else { 
      SetByte (AntAdr,  GPIOA, BandAlt12.PortMask);
      lcd.print(BandAlt12.AntName);
      lcd.setCursor(19,0);
      lcd.print("A");
    }
    break;
  case 9: // 10 Meters
    lcd.setCursor(0,0);
    lcd.print("A: 10-");
    if (AltAflag == 0){
      SetByte (AntAdr,  GPIOA, BandN10.PortMask);
      lcd.print(BandN10.AntName);
      lcd.setCursor(19,0);
      lcd.print("N");
    }
    else { 
      SetByte (AntAdr,  GPIOA, BandAlt10.PortMask);
      lcd.print(BandAlt10.AntName);
      lcd.setCursor(19,0);
      lcd.print("A");
    }
    break;
  case 10: // 6 Meters
    lcd.setCursor(0,0);
    lcd.print("A:  6-");
    if (AltAflag == 0){
      SetByte (AntAdr,  GPIOA, BandN6.PortMask);
      lcd.print(BandN6.AntName);
      lcd.setCursor(19,0);
      lcd.print("N");
    }
    else { 
      SetByte (AntAdr,  GPIOA, BandAlt6.PortMask);
      lcd.print(BandAlt6.AntName);
      lcd.setCursor(19,0);
      lcd.print("A");
    }
    break;
  default:
    lcd.setCursor(0,0);
    lcd.print("ERROR BCDRA=");
    lcd.print(BCDRA);
    lcd.print(" ");

  }
  //delay (1000);
} // End SetAntA()


void SetAntB(int inputB, int AltBflag) {
  switch (inputB) {
  case 0: // 60 Meters
    lcd.setCursor(0,1);
    lcd.print("B: 60-");
    if (AltBflag == 0){
      SetByte (AntAdr,  GPIOB, BandN60.PortMask);
      lcd.print(BandN60.AntName);
      lcd.setCursor(19,1);
      lcd.print("N");
    }
    else { 
      SetByte (AntAdr,  GPIOB, BandAlt60.PortMask);
      lcd.print(BandAlt60.AntName);
      lcd.setCursor(19,1);
      lcd.print("A");
    }
    break;
  case 1: // 160 Meters
    lcd.setCursor(0,1);
    lcd.print("B:160-");
    if (AltBflag == 0){
      SetByte (AntAdr,  GPIOB, BandN160.PortMask);
      lcd.print(BandN160.AntName);
      lcd.setCursor(19,1);
      lcd.print("N");
    }
    else { 
      SetByte (AntAdr,  GPIOB, BandAlt160.PortMask);
      lcd.print(BandAlt160.AntName);
      lcd.setCursor(19,1);
      lcd.print("A");
    }
    break;
  case 2: // 80 Meters
    lcd.setCursor(0,1);
    lcd.print("B: 80-");
    if (AltBflag == 0){
      SetByte (AntAdr,  GPIOB, BandN80.PortMask);
      lcd.print(BandN80.AntName);
      lcd.setCursor(19,1);
      lcd.print("N");
    }
    else { 
      SetByte (AntAdr,  GPIOB, BandAlt80.PortMask);
      lcd.print(BandAlt80.AntName);
      lcd.setCursor(19,1);
      lcd.print("A");
    }
    break;
  case 3: // 40 Meters
    lcd.setCursor(0,1);
    lcd.print("B: 40-");
    if (AltBflag == 0){
      SetByte (AntAdr,  GPIOB, BandN40.PortMask);
      lcd.print(BandN40.AntName);
      lcd.setCursor(19,1);
      lcd.print("N");
    }
    else { 
      SetByte (AntAdr,  GPIOB, BandAlt40.PortMask);
      lcd.print(BandAlt40.AntName);
      lcd.setCursor(19,1);
      lcd.print("A");
    }
    break;
  case 4: // 30 Meters
    lcd.setCursor(0,1);
    lcd.print("B: 30-");
    if (AltBflag == 0){
      SetByte (AntAdr,  GPIOB, BandN30.PortMask);
      lcd.print(BandN30.AntName);
      lcd.setCursor(19,1);
      lcd.print("N");
    }
    else { 
      SetByte (AntAdr,  GPIOB, BandAlt30.PortMask);
      lcd.print(BandAlt30.AntName);
      lcd.setCursor(19,1);
      lcd.print("A");
    }
    break;
  case 5: // 20 Meters
    lcd.setCursor(0,1);
    lcd.print("B: 20-");
    if (AltBflag == 0){
      SetByte (AntAdr,  GPIOB, BandN20.PortMask);
      lcd.print(BandN20.AntName);
      lcd.setCursor(19,1);
      lcd.print("N");
    }
    else { 
      SetByte (AntAdr,  GPIOB, BandAlt20.PortMask);
      lcd.print(BandAlt20.AntName);
      lcd.setCursor(19,1);
      lcd.print("A");
    }
    break;
  case 6: // 17 Meters
    lcd.setCursor(0,1);
    lcd.print("B: 17-");
    if (AltBflag == 0){
      SetByte (AntAdr,  GPIOB, BandN17.PortMask);
      lcd.print(BandN17.AntName);
      lcd.setCursor(19,1);
      lcd.print("N");
    }
    else { 
      SetByte (AntAdr,  GPIOB, BandAlt17.PortMask);
      lcd.print(BandAlt17.AntName);
      lcd.setCursor(19,1);
      lcd.print("A");
    }
    break;
  case 7: // 15 Meters
    lcd.setCursor(0,1);
    lcd.print("B: 15-");
    if (AltBflag == 0){
      SetByte (AntAdr,  GPIOB, BandN15.PortMask);
      lcd.print(BandN15.AntName);
      lcd.setCursor(19,1);
      lcd.print("N");
    }
    else { 
      SetByte (AntAdr,  GPIOB, BandAlt15.PortMask);
      lcd.print(BandAlt15.AntName);
      lcd.setCursor(19,1);
      lcd.print("A");
    }
    break;
  case 8: // 12 Meters
    lcd.setCursor(0,1);
    lcd.print("B: 12-");
    if (AltBflag == 0){
      SetByte (AntAdr,  GPIOB, BandN12.PortMask);
      lcd.print(BandN12.AntName);
      lcd.setCursor(19,1);
      lcd.print("N");
    }
    else { 
      SetByte (AntAdr,  GPIOB, BandAlt12.PortMask);
      lcd.print(BandAlt12.AntName);
      lcd.setCursor(19,1);
      lcd.print("A");
    }
    break;
  case 9: // 10 Meters
    lcd.setCursor(0,1);
    lcd.print("B: 10-");
    if (AltBflag == 0){
      SetByte (AntAdr,  GPIOB, BandN10.PortMask);
      lcd.print(BandN10.AntName);
      lcd.setCursor(19,1);
      lcd.print("N");
    }
    else { 
      SetByte (AntAdr,  GPIOB, BandAlt10.PortMask);
      lcd.print(BandAlt10.AntName);
      lcd.setCursor(19,1);
      lcd.print("A");
    }
    break;
  case 10: // 6 Meters
    lcd.setCursor(0,1);
    lcd.print("B:  6-");
    if (AltBflag == 0){
      SetByte (AntAdr,  GPIOB, BandN6.PortMask);
      lcd.print(BandN6.AntName);
      lcd.setCursor(19,1);
      lcd.print("N");
    }
    else { 
      SetByte (AntAdr,  GPIOB, BandAlt6.PortMask);
      lcd.print(BandAlt6.AntName);
      lcd.setCursor(19,1);
      lcd.print("A");
    }
    break;
  default:
    lcd.setCursor(0,1);
    lcd.print("ERROR BCDRB=");
    lcd.print(BCDRB);
    lcd.print(" ");

  }

  // delay (1000);

} // End SetAntB()



//********** Main Program******************************
void loop() {

  //PWSwitch = digitalRead(22); // check if Radio turned on!
  Etherflag = GetBit(B10000000,EthAdr,GPIOB);  // get etherflag as either 1 or 0
  BCDRA = bcdToDec(GetInputs(RadAdr, GPIOA),1); // get Radios A inputs & Convert to decimal
  BCDRB = bcdToDec(GetInputs(RadAdr, GPIOA),2);// get Radio B inputs & Convert to decimal
  BCDRC = bcdToDec(GetInputs(RadAdr, GPIOB),1); // get Radios C inputs & Convert to decimal
  BCDRD = bcdToDec(GetInputs(RadAdr, GPIOB),2);// get Radio D inputs & Convert to decimal
  BCDA1 = GetInputs(AuxAdr,'A'); // get Aux A inputs
  BCDA2 = GetInputs(AuxAdr,'B'); // get Aux B inputs
  BCDE1 = GetInputs(EthAdr,'A'); // get Ethernet 1 inputs  
  BCDE2 = GetInputs(EthAdr,'B'); // Get Ethernet 2 inputs 
  lcd.setCursor(0,2);
  //    lcd.print("EF=");
  //    lcd.print(Etherflag);
  //    lcd.print(" PWS=");
  //    lcd.print(PWSwitch);
  lcd.print("MA=");
  lcd.print(ManAflag);
  //    lcd.setCursor(0,1);
  lcd.print(" MB=");
  lcd.print(ManBflag);
  lcd.print(" LA=");
  lcd.print(AltAflag);
  lcd.print(" LB=");
  lcd.print(AltBflag);

  lcd.setCursor(0,3);
  //    lcd.print("E1=");
  //    lcd.print(BCDE1);
  //    lcd.print(" E2=");
  //    lcd.print(BCDE2);
  //    lcd.print(" RA=");
  //    lcd.print(BCDRA);
  //    lcd.setCursor(0,3);
  //    lcd.print("RB=");
  //    lcd.print(BCDRB);
  lcd.print("DA=");
  lcd.print(DLAflag);
  lcd.print(" DB=");
  lcd.print(DLBflag);
  lcd.print(" PWS=");
  lcd.print(PWSwitch);
  //delay(2000);
  //    SetEtherAntenna();
  //    SetEtherRelays();
  //    while (Etherflag == 0){// Ethernet control active =1
  //        lcd.setCursor(0,1);
  //lcd.print("TEST");
  //
  //} // end while Etherflag == 0)



  while (PWSwitch == 1){ //radio Turned on
    //lcd.setCursor(0,1);
    //lcd.print("TEST");
    char key = kpd.getKey(); // variable for key pressed on keyboad
    //        lcd.setCursor(17,3);
    //    lcd.print(int(key));

    if (key) 
    {  // Key is pressed
      lcd.setCursor(17,3);
      lcd.print(int(key));
      //Serial.println(key);
      //char lastkeypressed = key;
      dokey(key);
    } // end if(key)
    key = -key;
    // Set Antenna A
    if (ManAflag == 0){ // Auto
      digitalWrite(ManALED, LOW);
      if (AltAflag == 0){ // Normal
        digitalWrite(AltALED, LOW);
        if (DLAflag == 1){ // Dummyload
          digitalWrite(DLALED, HIGH);
          SetBit(SwReAdr,  GPIOB,  RY1,  1);// Turn RY1 on (DLA)
        }// end if(DLAflag ==1)
        else if (DLAflag == 0){// to Antenna
          digitalWrite(DLALED, LOW);
          BCDRA = bcdToDec(GetInputs(RadAdr, GPIOA),1); // get Radios A inputs & Convert to decimal
          SetAntA(BCDRA, AltAflag);               
        }// end Else if DLAflag == 0
      }// end if AltAflag == 0
      else if (AltAflag == 1){ //Alternate Antennas
        digitalWrite(AltALED, HIGH);
        if (DLAflag == 1){ // Dummyload
          digitalWrite(DLALED, HIGH);
          SetBit(SwReAdr,  GPIOB,  RY1,  1);// Turn RY1 on (DLA)
        } // end if(DLAflag ==1)
        else if (DLAflag == 0){// to Antenna
          digitalWrite(DLALED, LOW);
          BCDRA = bcdToDec(GetInputs(RadAdr, GPIOA),1); // get Radios A inputs & Convert to decimal
          SetAntA(BCDRA, AltAflag);
        } // end Else if DLAflag == 0
      } // end Else if(AltAflag == 1)
    }// end if ManAflag == 0
    else if (ManAflag == 1) { // MANUAL A
      digitalWrite(ManALED, HIGH);
      if (DLAflag == 1){ // Dummyload
        digitalWrite(DLALED, HIGH);
        SetBit(SwReAdr,  GPIOB,  RY1,  1);// Turn RY1 on (DLA)
      }// end if(DLAflag ==1)
      else if (DLAflag == 0){// to Antenna
        digitalWrite(DLALED, LOW);
        SetBit(SwReAdr,  GPIOB,  RY1,  0);// Turn RY1 off (DLA)

        //       char key = kpd.getKey(); // variable for key pressed on keyboad
        //        if (key <53) {  // Key for AntA is pressed
        //          //Serial.println(key);                    
        //          lastAkeypressed= key;
        //       lcd.setCursor(17,3);
        //      lcd.print(int(key));
        //         dokey(key);
        //        }// endif(key<53)
      }//end else if (DLAflag == 0)
    }// end if ManAflag == 1      

    // Set Antenna B
    if (ManBflag == 0){ // Auto
      digitalWrite(ManBLED, LOW);
      if (AltBflag == 0){ // Normal
        digitalWrite(AltBLED, LOW);
        lcd.setCursor(19,3);
        if (DLBflag == 1){ // Dummyload
          digitalWrite(DLBLED, HIGH);
          SetBit(SwReAdr, GPIOB, RY3,  1);// Turn RY3 on (DLB)
        }// end if(DLBflag ==1)
        else if (DLBflag == 0){// to Antenna
          digitalWrite(DLBLED, LOW);
          BCDRB = bcdToDec(GetInputs(RadAdr, GPIOA),2); // get Radios B inputs & Convert to decimal
          SetAntB(BCDRB,AltBflag);
        }// end Else if DLBflag == 0
      }// end if AltBflag == 0
      else if (AltBflag == 1){ //Alternate Antennas
        digitalWrite(AltBLED, HIGH);
        if (DLBflag == 1){ // Dummyload
          digitalWrite(DLBLED, HIGH);
          SetBit(SwReAdr, GPIOB, RY3, 1);// Turn RY3 on (DLB))
        } // end if(DLBflag ==1)
        else if (DLBflag == 0){// to Antenna
          digitalWrite(DLBLED, LOW);
          BCDRB = bcdToDec(GetInputs(RadAdr, GPIOA),2); // get Radios B inputs & Convert to decimal
          SetAntB(BCDRB, AltBflag);
        } // end Else if DLBflag == 0
      } // end Else if(AltBflag == 1)
    }// end if ManBflag == 0
    else if (ManBflag == 1) { // MANUAL
      digitalWrite(ManBLED, HIGH);
      if (DLBflag == 1){ // Dummyload
        digitalWrite(DLBLED, HIGH);
        SetBit(SwReAdr,  GPIOB, RY3, 1);// Turn RY3 on (DLB)
      }// end if(DLBflag ==1)
      else if (DLBflag == 0){// to Antenna
        digitalWrite(DLBLED, LOW);
        //        char key = kpd.getKey(); // variable for key pressed on keyboad
        //        if ((key >64)&&(key<89)) {  // Key for AntB is pressed
        //          //Serial.println(key);                    
        //       lcd.setCursor(17,3);
        //      lcd.print(int(key));
        //         lastBkeypressed= key;
        //          dokey(key);
        //        }// endif(key<10)
      }//end else if (DLBflag == 0)
    }// end if ManBflag == 1      
    //PWSwitch = digitalRead(22); // check if Radio turned on!
  }// endif(PWSwitch == 1)
  char key = kpd.getKey(); // variable for key pressed on keyboad
  if (key == 76) {  // Key for AntB is pressed          
    dokey(key);
  }// endif

}// end void Loop()

