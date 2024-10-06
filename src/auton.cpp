#include "config.hpp"
#include "auton.h"
#include "lemlib/chassis/chassis.hpp"
#include "pros/rtos.hpp"



void RightAWP() { //finished
    chassis.setPose(0, 0, -45);

            request_new_state_mogo(StateMogo::LOCATE);

    chassis.moveToPose(27, -29, -45, 1200, {.forwards = false, .lead = 0, .maxSpeed = 55, .minSpeed = 25});
chassis.waitUntilDone();

    while (Mogo.extend() == false) {
        pros::delay(5);
    }
    
        request_new_state_intake(SCORE);
pros::delay(950);

        request_new_state_intake(MECH);

    chassis.turnToPoint(3, -25, 500);   

    chassis.moveToPoint(3, -25.5, 1000, {.minSpeed = 127});
chassis.waitUntilDone();

    while (DistanceIntakeTop.get() > 16) {
        pros::delay(5);
    }

    chassis.moveToPoint(13, -25.5, 1000, {.minSpeed = 120});

    chassis.turnToHeading(30, 500, {.minSpeed = 100});
chassis.waitUntilDone(); 

    chassis.moveToPose(30, 0, 80, 2500, {.lead = 0.3, .maxSpeed = 90});
chassis.waitUntil(9);

      request_new_state_intake(SCORE);

              LiftPID(425);

                intakePiston.set_value(true);
chassis.waitUntilDone();

pros::delay(250);
                intakePiston.set_value(false); 

                    LiftPID(-415);

    chassis.moveToPose(31, 0, 80, 1000, {.maxSpeed = 70, .minSpeed = 70});
                    
pros::delay(250);

    chassis.moveToPose(24, 0, 80, 1000, {.forwards = false, .minSpeed = 110});
chassis.waitUntilDone();
                    
pros::delay(750);

    chassis.swingToHeading(26, lemlib::DriveSide::RIGHT, 1000);

                    LiftPID(720);

pros::delay(750);

    chassis.moveToPose(33, 12, 26, 1000, {.lead = 0});
chassis.waitUntilDone();

                    LiftPID(-285);
pros::delay(500);

    chassis.moveToPoint(25, -27, 1000, {.forwards = false, .maxSpeed = 110, .earlyExitRange = 3});

                    LiftPID(-425);

    chassis.swingToHeading(-55, lemlib::DriveSide::RIGHT, 700);
chassis.waitUntilDone();

    chassis.moveToPose(38, -36, -55, 1000, {.forwards = false, .maxSpeed = 60});
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BlueRight() {
    chassis.setPose(0, 0, 0);

    request_new_state_intake(MECH);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RedRight(){
    chassis.setPose(0, 0, 0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Forwards() { //finished
    chassis.setPose(0,0,0); 
//sets robot position to 0 on every axes
    chassis.moveToPose(0, 6, 0, 450, {.forwards = false, .minSpeed = 127});
//goes reverse 6 inches at max speed
    Intake.move(127); 
//spins the Intake to release the rubber band
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LeftAWP() { 
    chassis.setPose(0, 0, -45);

            request_new_state_mogo(StateMogo::LOCATE);

    chassis.moveToPose(27, -29, -45, 1200, {.forwards = false, .lead = 0, .maxSpeed = 55, .minSpeed = 25});
chassis.waitUntilDone();

    while (Mogo.extend() == false) {
        pros::delay(5);
    }

pros::delay(250);

        request_new_state_intake(SCORE);
pros::delay(950);

    chassis.turnToPoint(15, -24.8, 500);

        request_new_state_intake(MECH);

    chassis.moveToPoint(15, -25.8, 1000, {.minSpeed = 100});
chassis.waitUntilDone();

    while (DistanceIntakeBottom.get() > 55) {
        pros::delay(5);
    }

    chassis.moveToPoint(17, -25.8, 1000);

    while (DistanceIntakeTop.get() > 15) {
        pros::delay(5);
    }

        request_new_state_intake(UNLOAD);
pros::delay(1000);

    chassis.turnToHeading(60, 1000, {.maxSpeed = 80});
chassis.waitUntilDone(); 

    chassis.moveToPose(30, 0, 80, 2500, {.lead = 0.4, .maxSpeed = 80});
chassis.waitUntil(9);

      request_new_state_intake(SCORE);

              LiftPID(425);

                intakePiston.set_value(true);
chassis.waitUntilDone();

pros::delay(150);
                intakePiston.set_value(false); 

                    LiftPID(-415);

    chassis.moveToPose(28, 0, 90, 1000, {.maxSpeed = 70, .minSpeed = 50});
                    
pros::delay(500);

    chassis.moveToPose(24, 0, 90, 1000, {.forwards = false});
chassis.waitUntilDone();

    chassis.swingToHeading(40, lemlib::DriveSide::RIGHT, 1000);

                    LiftPID(720);

pros::delay(750);

    chassis.moveToPose(32, 12, 39, 1000, {.lead = 0});
chassis.waitUntilDone();

                    LiftPID(-285);
pros::delay(500);

    chassis.moveToPoint(25, -27, 1000, {.forwards = false, .maxSpeed = 110, .earlyExitRange = 3});

                    LiftPID(-425);

    chassis.swingToHeading(-55, lemlib::DriveSide::RIGHT, 1000);
chassis.waitUntilDone();

        request_new_state_intake(BRAKE);

    chassis.moveToPose(38, -36, -55, 1000, {.forwards = false, .maxSpeed = 60});
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void BlueLeft() {
    chassis.setPose(0,0,0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RedLeft(){
    chassis.setPose(0, 0, 0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Skills() {
    chassis.setPose(0,0,0);
    
    LiftPID(720);          //request_new_state_mogo(StateMogo::LOCATE);
}
/** top ring of the double stack
    intakePiston.set_value(true);

    request_new_state_intake(State::LOAD);

    chassis.moveToPoint(0, 3, 1000);
    
    chassis.waitUntilDone();

    intakePiston.set_value(false);
    
    chassis.moveToPoint(0, -5, 1000, {.forwards = false});
 */
