#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    liftHeight,     sensorPotentiometer)
#pragma config(Sensor, in2,    expander,       sensorAnalog)
#pragma config(Sensor, dgtl1,  solenoid,       sensorDigitalOut)
#pragma config(Sensor, dgtl2,  ,               sensorQuadEncoder)
#pragma config(Sensor, dgtl4,  ,               sensorQuadEncoder)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port1,           RLift,         tmotorVex393HighSpeed, openLoop)
#pragma config(Motor,  port2,           LFDrive,       tmotorVex393HighSpeed, openLoop)
#pragma config(Motor,  port3,           LDDrive,       tmotorVex393HighSpeed, PIDControl, encoder, encoderPort, I2C_1, 1000)
#pragma config(Motor,  port4,           LCDrive,       tmotorVex393HighSpeed, openLoop)
#pragma config(Motor,  port5,           RFDrive,       tmotorVex393HighSpeed, openLoop, reversed)
#pragma config(Motor,  port6,           RDDrive,       tmotorVex393HighSpeed, PIDControl, reversed, encoder, encoderPort, I2C_2, 1000)
#pragma config(Motor,  port7,           RCDrive,       tmotorVex393HighSpeed, openLoop, reversed)
#pragma config(Motor,  port8,           RIntake,       tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port9,           LIntake,       tmotorVex393, openLoop)
#pragma config(Motor,  port10,          LLift,         tmotorVex393HighSpeed, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "Vex_Competition_Includes.c"
#include "Robot.c"
#include "Selection.c"
#include "Autonomous.c"

#define JOYSTICK_THRESHOLD 10
#define NORM(a) (abs(a) <= JOYSTICK_THRESHOLD) ? 0 : a

void pre_auton() {
	bStopTasksBetweenModes = true;
	SelectAutonomousRoutine();
}

task autonomous() {
	if (bootstrap() != 0) return;
	switch(fieldZone) {
		case ZONE_MIDDLE:
			middleZoneAuton();
			break;
		case ZONE_HANGING:
			hangingZoneAuton();
			break;
	}
}

task usercontrol() {
	clearLCDLine(0);
	displayLCDCenteredString(0, "User Control");
	bLCDBacklight = true;

	while (true) {
		displayLCDVoltageString(1);
		drive(NORM(vexRT[Ch3]), NORM(vexRT[Ch2]));
		lift((nVexRCReceiveState & vrXmit2) ? NORM(vexRT[Ch3Xmtr2]) : (vexRT[Btn6U] ? 127 : vexRT[Btn6D] ? -127 : 0));
		intake((nVexRCReceiveState & vrXmit2) ? NORM(vexRT[Ch2Xmtr2]) : (vexRT[Btn5U] ? 127 : vexRT[Btn5D] ? -127 : 0));
		wait1Msec(20);
	}
}
