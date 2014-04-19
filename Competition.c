#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    liftHeight,     sensorPotentiometer)
#pragma config(Sensor, in2,    expander,       sensorAnalog)
#pragma config(Sensor, in3,    gyro,           sensorGyro)
#pragma config(Sensor, dgtl1,  solenoid,       sensorDigitalOut)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port1,           RLift,         tmotorVex393HighSpeed_HBridge, openLoop)
#pragma config(Motor,  port2,           RFDrive,       tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           RDDrive,       tmotorVex393HighSpeed_MC29, openLoop, reversed, encoderPort, I2C_1)
#pragma config(Motor,  port4,           RCDrive,       tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           LFDrive,       tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port6,           LDDrive,       tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_2)
#pragma config(Motor,  port7,           LCDrive,       tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port8,           RIntake,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           LIntake,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          LLift,         tmotorVex393HighSpeed_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "Vex_Competition_Includes.c"

#include "Robot.c"
#include "Selection.c"
#include "Autonomous.c"

#define JOYSTICK_THRESHOLD 10
#define NORM(a) (abs(a) <= JOYSTICK_THRESHOLD) ? 0 : a

void pre_auton() {
	bStopTasksBetweenModes = true;
	AddAutonomousRoutine(FieldZoneAny, FieldColorAny, "None", -1);
	AddAutonomousRoutine(FieldZoneMiddle, FieldColorAny, "Cap", 0);
	AddAutonomousRoutine(FieldZoneMiddle, FieldColorAny, "Partner", 1);
	AddAutonomousRoutine(FieldZoneHanging, FieldColorAny, "Clear", 2);
	AddAutonomousRoutine(FieldZoneMiddle, FieldColorRed, "Skills", 3);
	SelectAutonomousRoutine();
}

task autonomous() {
	clearLCDLine(0);
	clearLCDLine(1);
	bLCDBacklight = false;
	AutonomousRoutine *routine = SelectedAutonomousRoutine();
	displayLCDCenteredString(0, routine ? routine->name : "");

	intake(127);
	wait1Msec(500);
	intake(0);

	switch (routine ? routine->tag : -1) {
		case 0:
			middleZoneAutonCap();
			break;
		case 1:
			middleZoneAutonPartner();
			break;
		case 2:
			hangingZoneAutonClear();
			break;
		case 3:
			programmingSkills();
			break;
	}
}

task usercontrol() {
	clearLCDLine(0);
	displayLCDCenteredString(0, "User Control");
	bLCDBacklight = true;

	bool pressed = false;
	while (true) {
		if (vexRT[Btn8U] || vexRT[Btn8R]) {
			int power = vexRT[Btn8U] ? 127 : -127;
			driveAdjusted(power, power);
			lift(0);
			intake(0);
		} else {
			drive(NORM(vexRT[Ch3]), NORM(vexRT[Ch2]));
			lift((nVexRCReceiveState & vrXmit2) ? NORM(vexRT[Ch3Xmtr2]) : (vexRT[Btn6U] ? 127 : vexRT[Btn6D] ? -127 : 0));
			intake((nVexRCReceiveState & vrXmit2) ? NORM(vexRT[Ch2Xmtr2]) : (vexRT[Btn5U] ? 127 : vexRT[Btn5D] ? -127 : 0));
			if (!pressed && vexRT[Btn8D]) launcher(!launched());
			pressed = (vexRT[Btn8D] != 0);
		}
		displayLCDVoltageString(1);
		wait1Msec(20);
	}
}
