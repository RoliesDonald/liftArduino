/*
   GantryClasses.cpp
   Author : Jamorn Saksommon
   Class
  // please see http://www.github.com/MarginallyClever/GcodeCNCDemo for more information.

   Modifly date: 27.01.2017
*/
#include <Arduino.h>
#include "GantryClasses.h"

GantryClasses::GantryClasses() {

  mode_abs  = 1;
  feed_rate = 0;
  is_move   = false;

}

void GantryClasses::setFeedrate(float nfr) {



  nfr = nfr * STEPS_PER_MM / 60;
  if ( ( feed_rate == nfr ) || (nfr > MAX_FEEDRATE || nfr < MIN_FEEDRATE )  ) return; // same as last time?  quit now.

  step_delay = MAX_FEEDRATE / nfr;
  feed_rate = nfr;

}

void GantryClasses::prepareMove( float newx, float newy, float newz  ) {


  maxsteps = 0;

  axis[AXIS_X].delta = (newx - px) * STEPS_PER_MM;
  axis[AXIS_Y].delta = (newy - py) * STEPS_PER_MM;
  axis[AXIS_Z].delta = (newz - pz) * STEPS_PER_MM;


  for (int i = 0; i < num_axis ; ++i) {
    axis[i].absdelta = abs( axis[i].delta );
    axis[i].over = 0;
    if ( maxsteps < axis[i].absdelta ) maxsteps = axis[i].absdelta;
    // set the direction once per movement
    digitalWrite( motors[i].dir_pin, axis[i].delta > 0 ? HIGH : LOW);
  }

  delay_t = MAX_FEEDRATE / 5000;
  accel = 1;
  steps_to_accel = delay_t - step_delay;

  if (steps_to_accel > maxsteps / 2 )
    steps_to_accel = maxsteps / 2;

  steps_to_decel = maxsteps - steps_to_accel;

  step_count = 0;
  prev_time  = 0;

  is_move    = maxsteps > 0;

  if ( is_move) {
    enableMotor();
  }

  Serial.print("START ");
  Serial.println(delay_t);
  Serial.print("STOP ");
  Serial.println(step_delay);

  Serial.print("accel until ");
  Serial.println(steps_to_accel);
  Serial.print("decel after ");
  Serial.println(steps_to_decel);
  Serial.print("total ");
  Serial.println(maxsteps);

  for (int z = 0; z < num_axis ; ++z) {
    axis[z].absdelta = abs( axis[z].delta );
    axis[z].over = 0;
    if ( maxsteps < axis[z].absdelta ) maxsteps = axis[z].absdelta;
    // set the direction once per movement
    digitalWrite( motors[z].dir_pin, axis[z].delta > 0 ? HIGH : LOW);
  }

  delay_t = MAX_FEEDRATE / 5000;
  accel = 1;
  steps_to_accel = delay_t - step_delay;

  if (steps_to_accel > maxsteps / 2 )
    steps_to_accel = maxsteps / 2;

  steps_to_decel = maxsteps - steps_to_accel;

  step_count = 0;
  prev_time  = 0;

  is_move    = maxsteps > 0;

  if ( is_move) {
    enableMotor();
  }


  Serial.print("START ");
  Serial.println(delay_t);
  Serial.print("STOP ");
  Serial.println(step_delay);

  Serial.print("accel until ");
  Serial.println(steps_to_accel);
  Serial.print("decel after ");
  Serial.println(steps_to_decel);
  Serial.print("total ");
  Serial.println(maxsteps);


  setPosition(newx, newy, newz);

}
void GantryClasses::disableMotor() {
  int i;
  for (i = 0; i < num_axis; ++i) {
    digitalWrite(motors[i].enable_pin, HIGH);
  }
}
//void GantryClasses::disableMotorZ() {
//  int z;
//  for (z = 0; z < num_axis; ++z) {
//    digitalWrite(motors[z].enable_pin, HIGH);
//  }
//}

void GantryClasses::enableMotor() {

  int i;
  for (i = 0; i < num_axis; ++i) {
    digitalWrite(motors[i].enable_pin, LOW);
  }
}
//void GantryClasses::enableMotorZ() {
//  int z;
//  for (z = 0; z < num_axis; ++z) {
//    digitalWrite(motors[z].enable_pin, LOW);
//  }
//}
void GantryClasses::setPosition(float npx, float npy, float npz) {

  px = npx;
  py = npy;
  pz = npz;

}
void GantryClasses::move() {

  unsigned long curtime = micros();
  unsigned int _delay;

  if ( is_move &&   ( (curtime - prev_time) >=  delay_t) ) {

    prev_time = curtime;

    for (int j = 0; j < num_axis; ++j) {
      axis[j].over += axis[j].absdelta;

      if (  ( axis[j].over >= maxsteps ) ) {
        axis[j].over -= maxsteps;

        _delay = 100;

        Serial.println();
        Serial.print( j );
        Serial.print(" ");
        Serial.print( digitalRead( motors[j].dir_pin ) );
        Serial.print(" ");
        Serial.print( digitalRead( motors[j].home_switch_pin) );

        if ( ( digitalRead( motors[j].dir_pin) == LOW) && ( digitalRead( motors[j].home_switch_pin) == HIGH)) {
          Serial.println("home front");
          step_count = maxsteps;
          setPosition(0, 0, 0); // set to zero
        } else if ( ( digitalRead( motors[j].dir_pin) == HIGH) && ( digitalRead( motors[j].rear_switch_pin) == HIGH)) {
          Serial.println("rear");
          step_count = maxsteps;
          setPosition(0, 0, 0); // set to zero
        } else {
          digitalWrite(motors[j].step_pin, HIGH);
          delayMicroseconds(_delay);
          digitalWrite(motors[j].step_pin, LOW);
          delayMicroseconds(_delay);
        }

      }
    }
    if ( step_count < steps_to_accel) {
      delay_t -= accel;
    }
    if ( step_count >= steps_to_decel) {
      delay_t += accel;
    }

    step_count++;

    is_move = !(step_count >= maxsteps );

    if ( !is_move ) {
      disableMotor();
    }
  }

  if ( is_move &&   ( (curtime - prev_time) >=  delay_t) ) {

    prev_time = curtime;

    for (int z = 0; z < num_axis; ++z) {
      axis[z].over += axis[z].absdelta;

      if ((axis[z].over >= maxsteps)) {
        axis[z].over -= maxsteps;

        _delay = 100;

        Serial.println();
        Serial.print( z );
        Serial.print(" ");
        Serial.print( digitalRead( motors[z].dir_pin) );
        Serial.print(" ");
        Serial.print( digitalRead( motors[z].limit_switchZ_pin) );

        if ( ( digitalRead( motors[z].dir_pin) == HIGH) && ( digitalRead( motors[z].limit_switchZ_pin) == HIGH))
        {
          Serial.println("home top");
          step_count = maxsteps;
          setPosition(0, 0, 0); // set to zero
        } else if ( ( digitalRead( motors[z].dir_pin) == LOW) && ( digitalRead( motors[z].limit_Top_pinZ) == LOW))
        {
          Serial.println("botom z");
          step_count = maxsteps;
          setPosition(0, 0, 0); // set to zero
        } else {
          digitalWrite(motors[z].step_pin, LOW);
          delayMicroseconds(_delay);
          digitalWrite(motors[z].step_pin, HIGH);
          delayMicroseconds(_delay);
        }
      }
      if ( step_count < steps_to_accel) {
        delay_t -= accel;
      }
      if ( step_count >= steps_to_decel) {
        delay_t += accel;
      }

      step_count++;

      is_move = !(step_count >= maxsteps );

      if ( !is_move ) {
        disableMotor();
      }
    }
  }
}
