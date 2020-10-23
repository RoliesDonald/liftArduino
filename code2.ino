#include "GantryXYZ.h"

#define home_sw 34
#define load_sw 22
#define eject_sw 30
#define up_sw 40
#define down_sw 46

GantryXYZClasses gantryXYZ;

volatile boolean homePress  = false;
volatile boolean loadPress = false;
volatile boolean ejectPress = false;
volatile boolean upPress = false;
volatile boolean downPress = false;

uint8_t homeBtnState = 0;
uint8_t lastHomeBtnState = 0;

uint8_t loadBtnState = 0;
uint8_t lastLoadBtnState = 0;

uint8_t ejectBtnState = 0;
uint8_t lastEjectBtnState = 0;

uint8_t upBtnState = 0;
uint8_t lastUpBtnState = 0;

uint8_t downBtnState = 0;
uint8_t lastdownBtnState = 0;

void setup() {

  pinMode(home_sw, INPUT_PULLUP);
  pinMode(load_sw, INPUT_PULLUP);
  pinMode(eject_sw, INPUT_PULLUP);
  pinMode(up_sw, INPUT_PULLUP);
  pinMode(down_sw, INPUT_PULLUP);

  Serial.begin(9600);  // open coms
  gantryXYZ.setFeedrate(1000);  // set default speed

}

void loop() {

  /*
      Home limit switch Hall Effect sensor normal = 1
                                           Hit    = 0

  */

  if ( !gantryXYZ.is_move) {

    checkIfHomeIsPressed();
    checkIfLoadIsPressed();
    checkIfEjectIsPressed();

    if ( homePress) {
      homePress = false;
      gantryXYZ.prepareMove( 0,-2000, 0);
    }
    else if (loadPress) {
      loadPress = false;
      gantryXYZ.prepareMove( 0, 2000, 0);
    }
    else if (ejectPress) {
      ejectPress = false;
      gantryXYZ.prepareMove( 0, 2000, 0);
    }
  }

  if ( !gantryXYZ.is_move) {
    checkIfUpIsPressed();
    checkIfDownIsPressed();
    if (upPress) {
      upPress = false;
      gantryXYZ.prepareMove( 0, 0,-2000);
    }
    else if (downPress) {
      downPress = false;
      gantryXYZ.prepareMove( 0, 0, 2000);
    }
  }

  gantryXYZ.move();
}

void checkIfHomeIsPressed()
{
  homeBtnState = digitalRead(home_sw);
  if (homeBtnState != lastHomeBtnState)
  {
    if (homeBtnState == 0) {
      homePress = true;
      Serial.println("home kepencet");
    }
    delay(50);
  }
  lastHomeBtnState = homeBtnState;
}
void checkIfLoadIsPressed()
{
  loadBtnState = digitalRead(load_sw);
  if (loadBtnState != lastLoadBtnState)
  {
    if (loadBtnState == 0) {
      loadPress = true;
      Serial.println("Load kepencet");
    }
    delay(50);
  }
  lastLoadBtnState = loadBtnState;
}
void checkIfEjectIsPressed()
{
  ejectBtnState = digitalRead(eject_sw);

  if (ejectBtnState != lastEjectBtnState)
  {
    if (ejectBtnState == 0) {
      ejectPress = true;
      Serial.println("Eject kepencet");
    }
    delay(50);
  }
  lastEjectBtnState = ejectBtnState;
}

void checkIfUpIsPressed()
{
  upBtnState = digitalRead(up_sw);

  if (upBtnState != lastUpBtnState)
  {
    if (upBtnState == 0) {
      upPress = true;
      Serial.println("UP kepencet");
    }
    delay(50);
  }
  lastUpBtnState = upBtnState;
}

void checkIfDownIsPressed()
{
  downBtnState = digitalRead(down_sw);

  if (downBtnState != lastdownBtnState)
  {
    if (downBtnState == 0) {
      downPress = true;
      Serial.println("DOWN kepencet");
    }
    delay(50);
  }
  lastdownBtnState = downBtnState;
}
