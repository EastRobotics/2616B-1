int bootstrap() {
	clearLCDLine(0);
	clearLCDLine(1);
	displayLCDCenteredString(0, "Autonomous");
	bLCDBacklight = false;

	intake(-127);
	wait1Msec(500);
	intake(0);

	if (!enabled) return -1;
	if (fieldColor == -1) return -1;
	if (fieldZone == -1) return -1;
	return 0;
}

void driveTicks(int power, int ticks) {
	nMotorEncoder[LCDrive] = 0;
	nMotorEncoder[RCDrive] = 0;
	while (abs(nMotorEncoder[LCDrive]) < ticks || abs(nMotorEncoder[RCDrive]) < ticks) {
		drive(abs(nMotorEncoder[LCDrive]) < ticks ? power : 0, abs(nMotorEncoder[RCDrive]) < ticks ? power : 0);
	}
	drive(0, 0);
}

void turnTicks(bool right, int power, int ticks) {
	nMotorEncoder[LCDrive] = 0;
	nMotorEncoder[RCDrive] = 0;
	int leftScale = right ? 1 : -1;
	int rightScale = right ? -1 : 1;
	while (abs(nMotorEncoder[LCDrive]) < ticks || abs(nMotorEncoder[RCDrive]) < ticks) {
		drive(abs(nMotorEncoder[LCDrive]) < ticks ? leftScale * power : 0, abs(nMotorEncoder[RCDrive]) < ticks ? rightScale * power : 0);
	}
	drive(0, 0);
}

void middleZoneAuton() {
	while (SensorValue[liftHeight] < 1675) { // Actually 1840
		lift(127);
	}
	lift(0);
	driveTicks(127, 525);
	wait1Msec(500);
	driveTicks(-60, 100);
	wait1Msec(500);
	turnTicks((fieldColor == COLOR_BLUE), 60, 275);
	wait1Msec(500);
	driveTicks(60, 450);
	wait1Msec(500);
	turnTicks((fieldColor == COLOR_RED), 60, 275);
	wait1Msec(500);
	driveTicks(60, 100);
	wait1Msec(500);
	driveTicks(-60, 100);
	while (SensorValue[liftHeight] < 1675) { // Actually 1840
		lift(127);
	}
//	driveTicks(127, 500);
//	driveTicks(-127, 250);
//	turnTicks(true, 127, 200);
}

void hangingZoneAuton() {
	intake(-127);
	driveTicks(60, 100);
	wait1Msec(500);
	intake(0);
}
