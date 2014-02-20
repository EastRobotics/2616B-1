static int globalPower;
static int globalTicks;
static bool globalDone;
task drivingTask() {
	globalDone = false;
	driveTicks(globalPower, globalTicks);
	globalDone = true;
}

void driveTicksAsync(int power, int ticks) {
	globalPower = power;
	globalTicks = ticks;
	StartTask(drivingTask);
}

void waitForDrive() {
	while (!globalDone)
		wait1Msec(1);
}

void programmingSkills() {
	intake(-127);
	driveTicks(60, 250);
	wait1Msec(1000);
	driveTicks(-127, 1600);
	intake(0);
	wait1Msec(2000);
	driveTicks(127, 1100);
	driveTicksAsync(72, 450);
	if (liftTarget(LIFT_UPPER_LIMIT) != 0) return;
	lift(30);
	waitForDrive();
	driveTicks(64, 200);
	wait1Msec(500);
	intake(127);
	wait1Msec(1500);
	intake(0);
	driveTicks(-72, 200);
	if (liftTarget(LIFT_LOWER_LIMIT) != 0) return;
	driveTicks(-127, 1200);
	if (liftTarget(LIFT_UPPER_LIMIT - 550) != 0) return;
	wait1Msec(1000);
	driveTicks(127, 200);
	wait1Msec(500);
	driveTicks(-64, 100);
	turnTicks(true, 64, 275);
	driveTicks(127, 3000);
}

void middleZoneAutonCap() {
	driveTicks(127, 1300);
	wait1Msec(500);
	turnTicks((SelectedFieldColor() == FieldColorRed), 96, 100);
	if (liftTarget(LIFT_UPPER_LIMIT) != 0) return;
	lift(30);
	driveTicks(64, 600);
	drive((SelectedFieldColor() == FieldColorRed) ? 0 : 127, (SelectedFieldColor() == FieldColorRed) ? 127 : 0);
	wait1Msec(500);
	drive(0, 0);
	wait1Msec(500);
	intake(127);
	wait1Msec(300);
	intake(0);
	driveTicks(-60, 125);
	lift(0);
	wait1Msec(500);
	intake(54);
	wait1Msec(1250);
	intake(0);
	driveTicks(-127, 100);
	if (liftTarget(LIFT_LOWER_LIMIT) != 0) return;
	turnTicks((SelectedFieldColor() == FieldColorRed), 96, 25);
	driveTicks(-127, 1200);
	if (liftTarget(LIFT_UPPER_LIMIT - 550) != 0) return;
	driveTicks(127, 1000);
	wait1Msec(250);
	driveTicks(-127, 300);
	if (liftTarget(LIFT_LOWER_LIMIT) != 0) return;
}

void middleZoneAutonPartner() {
	driveTicks(127, 1100);
	driveTicksAsync(72, 450);
	if (liftTarget(LIFT_UPPER_LIMIT) != 0) return;
	lift(30);
	waitForDrive();
	driveTicks(64, 200);
	wait1Msec(500);
	intake(127);
	wait1Msec(500);
	intake(0);
	driveTicks(-72, 200);
	if (liftTarget(LIFT_LOWER_LIMIT) != 0) return;
	driveTicks(-127, 1200);

	wait1Msec(2500);

	driveTicks(127, 1100);
	driveTicksAsync(72, 450);
	if (liftTarget(LIFT_UPPER_LIMIT) != 0) return;
	lift(30);
	waitForDrive();
	driveTicks(64, 200);
	wait1Msec(500);
	intake(127);
	wait1Msec(1500);
	intake(0);
	driveTicks(-72, 200);
	if (liftTarget(LIFT_LOWER_LIMIT) != 0) return;
	driveTicks(-127, 1200);

	if (liftTarget(LIFT_UPPER_LIMIT - 550) != 0) return;
	driveTicks(127, 500);
}

void hangingZoneAutonClear() {
	intake(-127);
	driveTicks(60, 250);
	wait1Msec(1000);
	driveTicks(-60, 50);
	turnTicks((SelectedFieldColor() == FieldColorBlue), 60, 400);
	intake(0);
	if (liftTarget(LIFT_UPPER_LIMIT - 400) != 0) return;
	driveTicks(127, 150);
	intake(127);
	driveTicks(127, 100);
	wait1Msec(1500);
	intake(0);
	driveTicks(-127, 300);
	if (liftTarget(LIFT_LOWER_LIMIT) != 0) return;
	intake(-127);
	driveTicks(127, 600);
	wait1Msec(1000);
	turnTicks((SelectedFieldColor() == FieldColorBlue), 60, 200);
	if (liftTarget(LIFT_LOWER_LIMIT + 200) != 0) return;
	intake(0);
	driveTicks(127, 1500);
	if (liftTarget(LIFT_UPPER_LIMIT - 300) != 0) return;
	launcher(true);
	wait1Msec(1000);
	launcher(false);
	if (liftTarget(LIFT_LOWER_LIMIT) != 0) return;
}
