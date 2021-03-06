#ifndef CONFIG
#define CONFIG

// If 1, test methods will run instead of normal methods
#define TEST_MODE 0
// If 1, the pathfinder will print additional debug messages
#define PATHFINDER_DEBUG 0

#define MAPPING_DEBUG 1

// Pins
#define START 3

// Bumper pins
#define BUMP_L 11
#define BUMP_R 10

// Motor direction pins
#define DIR_L 7
#define DIR_R 4

// Motor pwm pins
#define PWM_L 6
#define PWM_R 5

// IndicationLED pin
#define LED_FINISHED 9

// Map size
#define MAP_SIZE 5
// Default cost to travel to each chunk. Should be high
#define DEFAULT_COST 1000

// Motor configuration
// Maximum power to use by the motors. Between 0 and 255
#define MAX_POWER 220

// The time the motors should run when turning 90 degrees
#define ROTATE_DELAY 1030

// Two values indicating how long the motors should run when moving forward to the next chunk
#define MOTION_COUNT 30
#define MOTION_DELAY 100

// Deviation value for the left motor
#define DEVIATION 0


#endif