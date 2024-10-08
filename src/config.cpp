#include "config.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/adi.hpp"
#include "pros/distance.hpp"
#include "pros/imu.hpp"
#include "pros/misc.hpp"
#include "pros/motor_group.hpp"
#include "pros/motors.hpp"
#include "pros/rotation.hpp"
#include "pros/rtos.h"
#include "pros/rtos.hpp"

// controller
using pros::delay;

pros::Controller controller(pros::E_CONTROLLER_MASTER);

// Green Ziptie

 pros::MotorGroup DTLeft({-15, 16, -17}, pros::MotorGearset::blue);
 pros::MotorGroup DTRight({12, -13, 14}, pros::MotorGearset::blue);

 pros::Imu inertial_sensor(9);
//

// Blue Ziptie
 pros::Motor Intake(-11, pros::MotorGearset::blue);

 pros::Distance DistanceIntakeTop(19);

 pros::Distance DistanceIntakeBottom(21);

 pros::adi::Pneumatics intakePiston('B', false);
//

// Yellow Ziptie
 pros::Motor Lift(-1, pros::MotorGearset::green);
//

// Red Ziptie
 pros::Distance DistanceMogo(8);

 pros::adi::Pneumatics Mogo('A', false);
//

//Utilites White ziptie

// drivetrain settings
 lemlib::Drivetrain drivetrain(
    &DTLeft,                    // left motor group
    &DTRight,                   // right motor group
    14,                         // 10 inch track width
    lemlib::Omniwheel::NEW_275, // using new 3.25" omnis
    450,                        // drivetrain rpm is 360
    2 // chase power is 2. If we had traction wheels, it would have been 8
);

// lateral motion controller
 lemlib::ControllerSettings
    linearController(12,   // proportional gain (kP)
                     0,    // integral gain (kI)
                     35,   // derivative gain (kD)
                     0,    // anti windup
                     1,    // small error range, in inches
                     1000, // small error range timeout, in milliseconds
                     30,   // large error range, in inches
                     3000, // large error range timeout, in milliseconds
                     2     // maximum acceleration (slew)
    );

// angular motion controller
 lemlib::ControllerSettings
    angularController(4.05, // proportional gain (kP)
                      0,    // integral gain (kI)
                      22,   // derivative gain (kD)
                      0,    // anti windup
                      1,    // small error range, in degrees
                      500,  // small error range timeout, in milliseconds
                      750,  // large error range, in degrees
                      750,  // large error range timeout, in milliseconds
                      0     // maximum acceleration (slew)
    );

//Rotation Sensors
 pros::Rotation horizontal_sensor(10);
 pros::Rotation vertical_sensor(18);

 lemlib::TrackingWheel horizontal_tracking_wheel(&horizontal_sensor,
                                                lemlib::Omniwheel::NEW_275, 2);
 lemlib::TrackingWheel vertical_tracking_wheel(&vertical_sensor,
                                              lemlib::Omniwheel::NEW_275, -1.5);
// sensors for odometry
// note that in this example we use internal motor encoders (IMEs), so we don't
// pass vertical tracking wheels
 lemlib::OdomSensors sensors(
    &vertical_tracking_wheel, // vertical tracking wheel 1, set to null
    nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
    &horizontal_tracking_wheel, // horizontal tracking wheel 1
    nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a
             // second one
    &inertial_sensor // inertial sensor
);

// create the chassis
 lemlib::Chassis chassis(drivetrain, linearController, angularController,
                        sensors);

 void waitUntilTankDist(
    double inches) { // creates a new function with the parameter inches
  lemlib::Pose lastPose =
      chassis.getPose(); // creates a new value under the lemlib:pose class that
                         // holds the initial position of the robot
  while (inches > 0) {   // while inches is greater than 0, the inside bit runs
    inches -= chassis.getPose().distance(
        lastPose); // redeclares param inches as inches minus distance away from
                   // the last position
    lastPose = chassis.getPose(); // updates current position of robot
    pros::delay(10);              // adds delay to not overload sensors
  } // slowly whittles down at inches until it is equal to 0, in which it will
    // run the next line of code in queue
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Intake State Machine

// the current state of the mechanism
StateIntake current_state_intake = BRAKE;

// functions used to request a new state
void request_new_state_intake(StateIntake requested_state_intake) {
  if (requested_state_intake < current_state_intake) {
    current_state_intake = requested_state_intake;
  }
  if (requested_state_intake > current_state_intake) {
    current_state_intake = requested_state_intake;
  }
}

// function which constantly updates the state of the mechanism
void state_machine_intake() {
  // run forever
  while (true) {
    // switch statement to select what to do based on the current state
    switch (current_state_intake) {
      // the Intake should be spinning
      case StateIntake::LOAD: {
        if (DistanceIntakeTop.get() < 20) current_state_intake = StateIntake::MECH; // if the Sensor does detect something, stop the intake
       
        else Intake.move(-127); // if the Sensors doesn't detect anything, keep spinning the intake
       
        break; // break out of the switch statement
      }
      case StateIntake::MECH: {
        while (DistanceIntakeTop.get_distance() > 16) {
          Intake.move(-110);
        }

        if (DistanceIntakeTop.get() < 17) current_state_intake = StateIntake::UNLOAD;

        break;
      }
      case StateIntake::STOP: {
        while (DistanceIntakeBottom.get() > 65) {
          Intake.move(-127);
          pros::delay(5);
        }
        if (DistanceIntakeBottom.get() > 60) current_state_intake = StateIntake::BRAKE;

        break;
      }
      case StateIntake::SCORE: {
        Intake.move(-127);

        break; // break out of the switch statement
      }
      case StateIntake::UNLOAD: {
        //reverse Intake for Driver Control
        Intake.move(127);

        break; // break out of the switch statement
      }
      case StateIntake::BRAKE: {
        //keep the Intake from spinning
        Intake.brake();

        break; // break out of the switch statement
      }
    }
    // delay to save resources
    pros::delay(10);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// the current state of the mechanism
StateMogo current_state2 = RELEASE;

// function used to request a new state
void request_new_state_mogo(StateMogo requested_state_mogo) {
  if (requested_state_mogo < current_state2) {
    current_state2 = requested_state_mogo;
  }
  if (requested_state_mogo > current_state2) {
    current_state2 = requested_state_mogo;
  }
}

// function which constantly updates the state of the mechanism
void state_machine_mogo() {
  // run forever
  while (true) {
    // switch statement to select what to do based on the current state
    switch (current_state2) {
      // the MoGo Mech should be open
      case StateMogo::LOCATE: {
        // if the Sensor does detect something, stop the intake
        if (DistanceMogo.get() <= 79) current_state2 = StateMogo::GRAB; //if the sensor does detect a goal, it goes to the GRAB state
        
        else Mogo.set_value(false); //if the sensors doesn't detect anything, keep the mech open

        break; // break out of the switch statement
      }
      case StateMogo::GRAB:{
        pros::delay(50);

        Mogo.set_value(true); //Extends the pistons, grabbing the goal

        pros::delay(250);

        break; // break out of the switch statement
      }
      case StateMogo::RELEASE:{
        Mogo.set_value(false); //Retracts the pistons, allowing the goal to slip out

        break; // break out of the switch statement
      }
    }
    // delay to save resources
    pros::delay(10);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LiftPID(double targetAngle){
  double kP;
  double kI;
  double kD;
  lemlib::PID LiftController(
        kP = 0.15,
        kI = 0.0175,
        kD = 0.75,
        5,
        true
  );

  double error;
  double prevError = 0;
  double integral = 0;
  while ((error < 1) && (error > -1)) {
	  error = (Lift.get_position() - targetAngle) - 1.25; //proportional
    integral = integral + error; //integral

	  if (error == 0) {
		  integral = 0;
	  }

	  if (std::abs(error) > 1200) {
		  integral = 0;
	  }

	  double derivative = error - prevError; //derivative
	  prevError = error;

	  double speed = (kP*error + kI*integral + kD*derivative) * 1.4;

    if (targetAngle < 0) {
      speed = speed * 75;
    }

    Lift.move_absolute(error, speed);

    if ((error < 1) && (error > -1)) {
      break;
    }
}
}

//Alliance = 660
//Wall = 1150