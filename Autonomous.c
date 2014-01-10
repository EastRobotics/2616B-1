int bootstrap() {
	clearLCDLine(0);
	clearLCDLine(1);
	displayLCDCenteredString(0, "Autonomous");
	bLCDBacklight = false;

	intake(127);
	wait1Msec(500);
	intake(0);

	if (!enabled) return -1;
	if (fieldColor == -1) return -1;
	if (fieldZone == -1) return -1;
	return 0;
}

void driveTicks(int power, int ticks) {
	nMotorEncoder[LDDrive] = 0;
	nMotorEncoder[RDDrive] = 0;
	while (abs(nMotorEncoder[LDDrive]) < ticks || abs(nMotorEncoder[RDDrive]) < ticks) {
		drive(abs(nMotorEncoder[LDDrive]) < ticks ? power : 0, abs(nMotorEncoder[RDDrive]) < ticks ? power : 0);
	}
	drive(0, 0);
}

void turnTicks(bool right, int power, int ticks) {
	nMotorEncoder[LDDrive] = 0;
	nMotorEncoder[RDDrive] = 0;
	int leftScale = right ? 1 : -1;
	int rightScale = right ? -1 : 1;
	while (abs(nMotorEncoder[LDDrive]) < ticks || abs(nMotorEncoder[RDDrive]) < ticks) {
		drive(abs(nMotorEncoder[LDDrive]) < ticks ? leftScale * power : 0, abs(nMotorEncoder[RDDrive]) < ticks ? rightScale * power : 0);
	}
	drive(0, 0);
}

int liftTarget(int target) {
	ClearTimer(T4);
	int maxTime = 2000;
	int difference = target - SensorValue[liftHeight];
	if (difference > 0) {
		while (SensorValue[liftHeight] < target && time1[T4] < maxTime) {
			lift(127);
		}
	} else if (difference < 0) {
		while (SensorValue[liftHeight] > target && time1[T4] < maxTime) {
			lift(-127);
		}
	}
	lift(20);
	if (time1[T4] >= maxTime) return -1;
	else return 0;
}

void middleZoneAuton() {
	if (liftTarget(LIFT_UPPER_LIMIT - 500) != 0) return;
	driveTicks(127, 525);
	intake(127);
	wait1Msec(1000);
	intake(0);
	driveTicks(-60, 100);
	wait1Msec(500);
	turnTicks((fieldColor == COLOR_RED), 60, 275);
	wait1Msec(500);
	driveTicks(127, 450);
	wait1Msec(500);
	turnTicks((fieldColor == COLOR_BLUE), 60, 250);
	wait1Msec(500);
	driveTicks(60, 300);
	wait1Msec(500);
	driveTicks(-60, 300);
	if (liftTarget(LIFT_LOWER_LIMIT) != 0) return;
	// turnTicks((fieldColor == COLOR_BLUE), 60, 35);
	// driveTicks(127, 1000);
	// if (liftTarget(2000) != 0) return;
	// driveTicks(127, 200);
	// wait1Msec(500);
	// intake(127);
	// wait1Msec(750);
	// intake(0);
	// driveTicks(-127, 100);
	// if (liftTarget(1500) != 0) return;
	// lift(0);
}

void hangingZoneAuton() {
	// intake(-127);
	// driveTicks(60, 450);
	// wait1Msec(500);
	// driveTicks(-60, 50);
	// intake(0);
	// turnTicks((fieldColor == COLOR_BLUE), 60, 550);
	// if (liftTarget(1575) != 0) return;
	// driveTicks(127, 175);
	// intake(127);
	// driveTicks(127, 100);
	// wait1Msec(2000);
	// intake(0);
	// if (liftTarget(1500) != 0) return;
	// driveTicks(-127, 300);
	// turnTicks((fieldColor == COLOR_RED), 60, 225);
	// driveTicks(127, 250);
	// turnTicks((fieldColor == COLOR_BLUE), 60, 225);
	// driveTicks(127, 325);
}
