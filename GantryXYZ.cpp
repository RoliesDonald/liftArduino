
#include <arduino.h>
#include "GantryXYZ.h"

GantryXYZClasses::GantryXYZClasses()
{
  num_axis = 1;
  num_axis = 2;

  motors = new Motor[num_axis];
  axis   = new Axis[num_axis];

  /*
      // X
      motors[0].step_pin=46;
      motors[0].dir_pin=48;
      motors[0].enable_pin=62;
      motors[0].limit_switch_pin=14;
  */
  // Y
  motors[1].step_pin = 9;
  motors[1].dir_pin = 10;
  motors[1].enable_pin = 11;
  motors[1].home_switch_pin = 17;
  motors[1].rear_switch_pin = 21;
  // Z
  motors[2].step_pin = 35;
  motors[2].dir_pin = 39;
  motors[2].enable_pin = 29;
  motors[2].limit_switchZ_pin = 19;
  motors[2].limit_Top_pinZ = 18;


  for (int i = 0; i < num_axis; ++i) {
    // set the motor pin & scale
    pinMode(motors[i].step_pin, OUTPUT);
    pinMode(motors[i].dir_pin, OUTPUT);
    pinMode(motors[i].enable_pin, OUTPUT);
    pinMode(motors[i].home_switch_pin, INPUT); // Home limit
    pinMode(motors[i].rear_switch_pin, INPUT); // Rear limit
  }
  disableMotor();
  setPosition(0, 0, 0);
  
  for (int z = 0; z < num_axis; ++z) {
    pinMode(motors[z].step_pin, OUTPUT);
    pinMode(motors[z].dir_pin, OUTPUT);
    pinMode(motors[z].enable_pin, OUTPUT);
    pinMode(motors[z].limit_switchZ_pin, INPUT); // Z limit
    pinMode(motors[z].limit_Top_pinZ, INPUT);// Z top limit
  }
  disableMotor();
  setPosition(0, 0, 0);
}
void GantryXYZClasses::processCommand() {

  int cmd = 1;

  switch (cmd) {
    case  0:
    case  1: { //  move


        setFeedrate( parsenumber('f', feed_rate));

        prepareMove( parsenumber('x', (mode_abs ? px : 0)) + (mode_abs ? 0 : px),
                     parsenumber('y', (mode_abs ? py : 0)) + (mode_abs ? 0 : py),
                     parsenumber('z', (mode_abs ? pz : 0)) + (mode_abs ? 0 : pz)
                   );

        break;
      }
    case  2:
    case  4:  ;  break;  // dwell
    case 90: mode_abs = 1; break; // absolute mode
    case 91: mode_abs = 0; break; // relative mode
    case 92:  // set logical position

      break;
    default:  break;
  }

  ready();
}
