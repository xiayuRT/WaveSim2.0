#include <SPI.h>
#include <HighPowerStepperDriver.h>

// Varaibles
const uint8_t PIN_CS = 53;
const int STEP_DIVISION_RATIO = 128;
const float STEP_FULL = 360.0 / 200.0;
const float STEP_MICRO = STEP_FULL / STEP_DIVISION_RATIO;
uint8_t rec_counter = 0; // counting valid input
uint16_t Amplitude = 0;
uint16_t Angle_range = 0;
uint16_t Test_time = 0;  
uint32_t test_start_time = 0;
uint32_t test_elapsed_time = 0;
bool start_flag = true;
float Frequency = 0;
float Parameter = 1.0;

// Macros
#define MICROSECONDS_FROM_SECONDS(SECONDS) (SECONDS * 1000)
#define MILLISECONDS_FROM_SECONDS(SECONDS) (SECONDS * 1000000)
#define Angle_range (Amplitude * 2)
#define Period (1.0 / Frequency)
#define STEP_NUM_PERSEC (Angle_range / STEP_MICRO)
#define STEP_PERIOD (MILLISECONDS_FROM_SECONDS(Period / STEP_NUM_PERSEC) - 6.0) * Parameter

// High power stepper motor driver 
HighPowerStepperDriver sd;

// Serial communication and flags
String inputString = "";  // string to hold incoming data
bool stringComplete = false;  // whether the string is complete
bool ampValid = false; // whether the amplitude is valid
bool freqValid = false; // whether the frequency is valid
bool timeValid = false; // wether the testing time is valid

// State machine
enum system_state{
  // Stepper motor is working
  STATE_RUN, 
  // Stepper motor is stopped
  STATE_STOP, 
  // Stepper motor is waiting for configuration
  STATE_CONF
} sys_state;

// Functions 
void para_init();
void wave_config();
void wave_rock();
void wave_perform();
void wave_stop();
void serialEvent();


/************************************************************ SET UP *********************************************************/


void setup() {
  // Config serial communication
  Serial.begin(9600);  // Buadrate 
  inputString.reserve(200);  // reserve 200 bytes for the input string
  while(!Serial){;}   // Wait for serial port to connect

  // Initialize the state machine
  sys_state = STATE_CONF;

  // Set up SPI and driver congiguration
  SPI.begin();
  delay(1);
  sd.setChipSelectPin(PIN_CS);
  sd.resetSettings(); 
  sd.clearStatus();  // Clear default settings and clear latched status
  sd.setDecayMode(HPSDDecayMode::AutoMixed); // Set thr Decay mode of driver 
  sd.setCurrentMilliamps36v4(2500); // Set current here
  sd.setStepMode(HPSDStepMode::MicroStep128); // Set micro steping mode -> 1/2^n, up to n = 8
  sd.enableDriver(); // Start the driver 
}


/************************************************************ LOOP ***********************************************************/


void loop() {
  switch(sys_state){
    case (STATE_RUN): 
      wave_rock(); // Rocking motion
      break;

    case (STATE_STOP):  
      wave_stop(); // Do nothing    
      break;

    case (STATE_CONF):
      wave_config(); // Motor configuration
      break;
  }
}


/*********************************************************** FUNCTIONS *********************************************************/


/**
 * @brief initialize the parameters needed for configuration
 * @return -
 */
void para_init(){
  // Initialize data
  Amplitude = 0;
  Frequency = 0;
  Parameter = 1.0;
  Test_time = 0;  
  test_start_time = 0;
  test_elapsed_time = 0;
  // Initialize flags
  ampValid = false;
  freqValid = false;
  timeValid = false;
  start_flag = true;
}


/**
 * @brief rocking motion for one whole period
 * @param step_period period in each step
 * @param step_num_persec step numbers per second
 * @return -
 */
void wave_perform(float step_period, float step_num_persec){
  sd.setDirection(0);
  for (unsigned int x = 0; x < step_num_persec / 2; x++){
    sd.step();
    delayMicroseconds(step_period);
  }

  sd.setDirection(1);
  for (unsigned int x = 0; x < step_num_persec; x++){
    sd.step();
    delayMicroseconds(step_period);
  }

  sd.setDirection(0);
  for (unsigned int x = 0; x < step_num_persec / 2; x++){
    sd.step();
    delayMicroseconds(step_period);
  }
}


/**
 * @brief config the wave angle and period through serial and change states
 * @return -
 */
void wave_config(){
  if (Serial){
    serialEvent();
    // print the string when a newline arrives:
    if (stringComplete) {
      int length = inputString.length();
      int message_length = 0;
      int delimeter_found_at = inputString.indexOf(' ');
      String message_values[length];
      while ( delimeter_found_at != -1 && message_length < length){
        message_values[message_length++] = inputString.substring(0, delimeter_found_at);
        inputString = inputString.substring(delimeter_found_at + 1);
        delimeter_found_at = inputString.indexOf(' ');
      }
      message_values[message_length++] = inputString;

      if (message_values[1] == "amp"){
        rec_counter++;
        Amplitude = message_values[2].toInt();
        if (Amplitude && Amplitude <= 60){
          ampValid = true;
        }
      }
      else if (message_values[1] == "freq"){
        rec_counter++;
        Frequency = message_values[2].toFloat();
        if (Frequency <= 1 && Frequency > 0){
          freqValid = true;
        }
      }
      else if (message_values[1] == "time"){
        rec_counter++;
        Test_time = message_values[2].toInt();
        if (Test_time) {
          timeValid = true;
        }
      }

      // clear the string:
      inputString = "";
      stringComplete = false;
    }

    if (rec_counter == 3){
      if (timeValid){
        if (ampValid && freqValid) {
          sys_state = STATE_RUN;
        }
        else if ((!ampValid && freqValid) || (ampValid && !freqValid)) {
          sys_state = STATE_STOP;
        }
      } 
      // reset the counter
      rec_counter = 0;
    }
  }
  return;
}


/**
 * @brief control the rocking motion according to the configuration
 * @return -
 */
void wave_rock(){
  // Time variables initialize
  test_start_time = millis();

  // Start rocking
  do{
    wave_perform(STEP_PERIOD, STEP_NUM_PERSEC);
    test_elapsed_time = millis() - test_start_time;
    // self-adaptive calculation
    if (start_flag){
      Parameter = (Period / test_elapsed_time) * 2000;
      start_flag = false;
    }
  } while(test_elapsed_time < MICROSECONDS_FROM_SECONDS(Test_time));

  // Update the state machine
  sys_state = STATE_CONF; 

  // Initialize the parameters
  para_init(); 
}


/**
 * @brief stop the motor for a specific time
 * @return -
 */
void wave_stop(){
  // Time variables initialize
  test_start_time = millis();
  while (millis() - test_start_time < MICROSECONDS_FROM_SECONDS(Test_time)) {;}

  // Update the state machine
  sys_state = STATE_CONF; 

  // Initialize the parameters
  para_init(); 
}


/**
 * @brief SerialEvent occurs whenever a new data comes in the hardware serial RX. 
 * @return -
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}