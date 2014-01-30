#ifndef CONFIG
#define CONFIG

// If 1, test methods will run instead of normal methods
#define TEST_MODE 0
// If 1, the pathfinder will print additional debug messages
#define PATHFINDER_DEBUG 1

// Pins
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
#define MAP_SIZE 4
// Default cost to travel to each chunk. Should be high
#define DEFAULT_COST 1000

#endif