//config.h




// ==============
// rotary encoder
// ==============


// enable encoder reading
#define enableEncoder 1
 
// pin definition

// rotary encoder - these pins should have interrupt abilities
// escpecially if the rotary encoder got a high resolution
#define encoderApin 2
#define encoderBpin 3

// pins for the rotary switch for axis selction
// switch = pin against GND (internal pull up is on)
#define encoderXpin 8
#define encoderYpin 9
#define encoderZpin 10

// resolution
// there are three diffrent resolution or speeds for the 
// rotary encoder
// this values need to be adjusted depending on the resolution of the 
// rotary encoder. these values here are working with an encoder with
// 600 pulses per revolution
// jog command = pulses of the rotary encoder / encoderResolution
// for example encoderResolution1 = 2000 -> 0.002mm per pulse -> 1.2mm for a whole revolution on the encoder
#define encoderResolution1 2000
#define encoderResolution2 200
#define encoderResolution3 50


// feed rate for the jog command. this is the theoretical max speed in mm/min 
// while jogging. in reality it will be slowed down by the acceleration
// management of grbl
#define jogFeedRate1 150
#define jogFeedRate2 1200
#define jogFeedRate3 4000


// pins for the rotafy switch of the encoder resolution
#define encoderResolutionPin1 5
#define encoderResolutionPin2 6
#define encoderResolutionPin3 7

// timing for canceling jogging, when the rotary encoder stopped moving
// that's needed for fast jogs and slow machine acceleration
// not too low, otherwise GRBL is being hammered with cancel commands and hangs it self
#define encoderCancelIntervall 500

// max step size
// defining the maximal step size for the jog commands. In some rare cases read errors
// can happen. Those would result in large distance jog commands which would be very dangerous
// value is micrometer
#define encoderMaxSteps 1000

// send jog cmd intervall
// limiting the time between the jog commands
// rotary encoders with a very high resolution (like 600 pulses per rev) can
// produce errors when turned too fast
// milliseconds
#define encoderCmdIntervall 100





// ===============================================
// potentiometer for feed and spindle overrides
// pin goes to the center pin of the potentiometer
// the other two pins are connected to GND and 5V
// ===============================================

// offset values
// because potentiometers are analog devices there might be the need for some 
// calibration especially for potentiometers that snap on the center position
// with this values you can make sure the center position of the potentiometer
// equals 100
//

// enable or disable the potentiometer reading
#define feedSpindleOvEnable 1

#define feedSpeedReadingIntervalle 500

#define feedOffset 1
#define spindleOffset 2

#define feedPin A0
#define spindlePin A1
// smoothing the analog read
#define smoothSteps 10
// define the min and max ranges of the overrides
#define spindleOvMax 200
#define spindleOvMin 25
#define feedOvMax 200
#define feedOvMin 25

// =============================
// 7 segment 8 digit LED display
// =============================

// pins
#define LED_X_DataIn 53
#define LED_X_CLK 52
#define LED_X_LOAD 51

#define LED_Y_DataIn 50
#define LED_Y_CLK 49
#define LED_Y_LOAD 48

#define LED_Z_DataIn 47
#define LED_Z_CLK 46
#define LED_Z_LOAD 45

#define LED_I_DataIn 44
#define LED_I_CLK 43
#define LED_I_LOAD 42

// brightness of the LED display
#define intensity 2

// ================================
// interval for sending "?" to GRBL
// ================================
// should not be below 200ms - hammering too fast will freeze grbl
#define updateInterval 200

// timer for sending "?" after last movement of jog wheel or change of override knobs
// this is just to stop samming with "?" request when it's not needed. 
// otherwise working with the console is not really possible
#define updatePostTimer 1000

// ===============================================================
// baud rate
// the baud rate is set to a pretty low value to give 
// the slow arduino mega enough time to to all it's calucations
// while making sure the communication is working. 
// It's not as bad as it sounds: 9600 baud = 1200 byte/sec
// that's roughly 42 lines of gcode per second. For a milling 
// machine more then enough to keep the buffer full.
// GRBL got to be set to the same baud rate (in the config.h file)
// worth a try if a faster board like the teensy will work in full
// speed. this code compiles on a teensy 3.6. coudn't test it, 
// because I don't have a teensy
// ===============================================================

#define baudRate 9600

// =====================
// enable debug messages
// =====================

#define debugEnable 0
