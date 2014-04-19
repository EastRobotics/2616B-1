/*
 * These enums represent the field color and zone of a routine
 *
 * They are bitmasks so that they can represent both at once
 */
typedef enum {
	FieldZoneUnknown = 0x0,
	FieldZoneHanging = 0x1,
	FieldZoneMiddle = 0x2,
	FieldZoneAny = FieldZoneHanging | FieldZoneMiddle
} FieldZone;

typedef enum {
	FieldColorUnknown = 0x0,
	FieldColorRed = 0x1,
	FieldColorBlue = 0x2,
	FieldColorAny = FieldColorRed | FieldColorBlue
} FieldColor;

static bool doneSelection = false;
static FieldZone selectedFieldZone = -1;
static FieldColor selectedFieldColor = -1;

/*
 * This structure defines an autonomous routine
 *
 * It includes pointers to be part of a doubly linked list
 */

struct AutonomousRoutine {
	AutonomousRoutine *prev;
	AutonomousRoutine *next;
	FieldZone zone;
	FieldColor color;
	char *name;
	int tag;
} AutonomousRoutine;

static AutonomousRoutine routines[20];
static int slot = 0;

static AutonomousRoutine *first = NULL;
static AutonomousRoutine *selected = NULL;

/*
 * Creates a new autonomous routine and adds it to the linked list
 */
void AddAutonomousRoutine(FieldZone zone, FieldColor color, char *name, int tag) {
	routines[slot].prev = NULL;
	routines[slot].next = NULL;
	routines[slot].zone = zone;
	routines[slot].color = color;
	routines[slot].name = name;
	routines[slot].tag = tag;

	if (first == NULL) {
		first = &routines[slot];
	} else {
		AutonomousRoutine *prev = first;
		while (prev->next)
			prev = prev->next;
		routines[slot].prev = prev;
		prev->next = &routines[slot];
	}

	slot++;
}

/*
 * Wait for screen buttons to be pressed
 *
 * Takes a bitmask of which buttons are being waited for
 * and returns which button was pressed
 */
TControllerButtons waitForButtons(TControllerButtons allowed) {
	TControllerButtons previous = 0;
	TControllerButtons selected = 0;
	while (selected == 0) {
		if ((allowed & kButtonLeft) != 0 && (previous & kButtonLeft) == kButtonLeft && (nLCDButtons & kButtonLeft) == 0) {
			selected = kButtonLeft;
		} else if ((allowed & kButtonCenter) != 0 && (previous & kButtonCenter) == kButtonCenter && (nLCDButtons & kButtonCenter) == 0) {
			selected = kButtonCenter;
		} else if ((allowed & kButtonRight) != 0 && (previous & kButtonRight) == kButtonRight && (nLCDButtons & kButtonRight) == 0) {
			selected = kButtonRight;
		}

		previous = nLCDButtons;
		wait1Msec(20);
	}
	return selected;
}

task selection() {
	doneSelection = false;

	// Select the starting tile zone
	displayLCDCenteredString(0, "Select Zone");
	displayLCDString(1, 0, "Hanging");
	displayLCDString(1, 10, "Middle");
	switch (waitForButtons(kButtonLeft | kButtonRight)) {
		case kButtonLeft:
			selectedFieldZone = FieldZoneHanging;
			break;
		case kButtonRight:
			selectedFieldZone = FieldZoneMiddle;
			break;
	}
	clearLCDLine(0);
	clearLCDLine(1);

	// Select the starting tile color
	displayLCDCenteredString(0, "Select Color");
	displayLCDString(1, 0, "Red");
	displayLCDString(1, 12, "Blue");
	switch (waitForButtons(kButtonLeft | kButtonRight)) {
		case kButtonLeft:
			selectedFieldColor = FieldColorRed;
			break;
		case kButtonRight:
			selectedFieldColor = FieldColorBlue;
			break;
	}
	clearLCDLine(0);
	clearLCDLine(1);

	// Filter the linked list based on the selected zone and color
	AutonomousRoutine *routine = first;
	while (routine) {
		if ((routine->zone & selectedFieldZone) == 0 || (routine->color & selectedFieldColor) == 0) {
			if (routine->prev)
				routine->prev->next = routine->next;
			if (routine->next)
				routine->next->prev = routine->prev;
		}
		routine = routine->next;
	}

	// Select from the routines remaining in the linked list
	displayLCDCenteredString(0, "Select Routine");
	AutonomousRoutine *current = first;
	while (selected == NULL) {
		displayLCDCenteredString(1, current->name);
		displayLCDString(1, 0, (current != NULL && current->prev != NULL) ? "<-" : "  ");
		displayLCDString(1, 14, (current != NULL && current->next != NULL) ? "->" : "  ");
		switch (waitForButtons(kButtonLeft | kButtonCenter | kButtonRight)) {
			case kButtonLeft:
				if (current != NULL && current->prev != NULL)
					current = current->prev;
				break;
			case kButtonRight:
				if (current != NULL && current->next != NULL)
					current = current->next;
				break;
			case kButtonCenter:
				if (current)
					selected = current;
				break;
		}
	}

	doneSelection = true;
}

/*
 * Select the autonomous routine
 *
 * This is a blocking function that selects the autonomous routine
 * while the robot is disabled
 */
void SelectAutonomousRoutine() {
	if (!bIfiRobotDisabled) return;

	bLCDBacklight = true;
	startTask(selection);

	while (!doneSelection) {
		if (!bIfiRobotDisabled) doneSelection = true;
		wait1Msec(25);
	}

	stopTask(selection);
	bLCDBacklight = false;
}

/*
 * Return the selected zone, color, and routine
 */
FieldZone SelectedFieldZone() {
	return selectedFieldZone;
}

FieldColor SelectedFieldColor() {
	return selectedFieldColor;
}

AutonomousRoutine * SelectedAutonomousRoutine() {
	return selected;
}
