/*******************************************************************
 * basic_move
 
  Control the direction and speed of motors rotation by pwm,
  to make the car go forward, backward, left turn, right turn and stop.

******************************************************************/
#include <Arduino.h>
#include <SoftPWM.h>

//Flame sensor info
#define FLAME_SENSOR_CHANNEL_D1 7
#define FLAME_SENSOR_CHANNEL_D2 8
#define FLAME_SENSOR_CHANNEL_D3 9
#define FLAME_SENSOR_CHANNEL_D4 2
#define FLAME_SENSOR_CHANNEL_D5 0

#define FLAME_SENSOR_CHANNEL_A3 A4

// Flame orientation times
#define TURN_HALF 100
#define TURN_ONE 200
#define TURN_ONE_AND_HALF 300
#define TURN_TWO 400


bool flameStateChannel1;
bool flameStateChannel2;
bool flameStateChannel3;
bool flameStateChannel4;
bool flameStateChannel5;

uint16_t flameValChannel3;

#define PRINT_STUFF 0

//////////// PUMP STUFF
#define PUMP_ENABLE A5 

//////////////////////////// LED STUFF
// Indicate whether RGB leds are common-anodes or not - 0 common cathode - 1 common anode
#define COMMON_ANODE 0

// Set the pins of RGB leds, the order is R,G,B
#define RGB_PINS \
  (uint8_t[3]) { \
    12, 13, 11 \
  }

// Define colors
const uint8_t key_color_array[9][3]{
  { 255, 0, 0 },      // red
  { 255, 165, 0 },    // orange
  { 255, 255, 0 },    // yellow
  { 0, 255, 0 },      // green
  { 0, 127, 255 },    // cyan
  { 0, 0, 255 },      // blue
  { 139, 0, 255 },    // purple
  { 255, 128, 255 },  // pink
  { 255, 255, 255 },  // white
};

#define LED_RED 1
#define LED_ORANGE 2
#define LED_YELLOW 3
#define LED_GREEN 4
#define LED_CYAN 5
#define LED_BLUE 6
#define LED_PURPLE 7
#define LED_PINK 8
#define LED_WHITE 9

void rgbBegin();
void rgbWrite(uint32_t color);
void rgbWrite(uint8_t r, uint8_t g, uint8_t b);
void rgbOff();

// Limits for random turn angle ms
#define MIN_TURN_ANGLE 200
#define MAX_TURN_ANGLE 1300


////////////////////////////////////////// ULTRASONIC STUFF
// Set collision detection interval (how long robots goes between checks)
#define COLLISION_DETECTION_INTERVAL 0

// Set pins of ultrasonic, to save IO ports, echo and tring could share an identical pin
#define ULTRASONIC_TRIG_PIN 10
#define ULTRASONIC_ECHO_PIN 10

// Configure ultrasonic obstacle avoidance distance, unit cm */
#define ULTRASONIC_AVOIDANCE_THRESHOLD 11
#define MAX_DISTANCE 2000  // unit: cm


////////////////////// MOTOR STUFF
/*  [0]--|||--[1]
  *   |         |
  *   |         |
  *   |         |
  *   |         |
  *  [3]-------[2]
 */
// Set the pins for the motors
#define MOTOR_PINS \
  (uint8_t[8]) { \
    3, 4, 5, 6, A3, A2, A1, A0 \
  }

// Set the positive and negative directions for the motors
#define MOTOR_DIRECTIONS \
  (uint8_t[4]) { \
    1, 0, 0, 1 \
  }

#define MOTOR_POWER_MIN 28  // 28/255
//#define MOTOR_POWER_MIN 23  // 28/255

// Speeds during operation
int8_t power_driveForward = 28;
int8_t power_driveBackward = 28;
int8_t power_turnRight = 30;
int8_t power_turnLeft = 30;

void setup() {
  Serial.begin(115200);
  Serial.println("Zeus Car basic move");
  SoftPWMBegin();  //init softpwm, before the motors initialization
  rgbBegin();      // init rgb LEDs
  carBegin();      // init motors

  //confugure pump PIN
  pinMode(PUMP_ENABLE,OUTPUT);

  // TEST OUT PUMP
  /*digitalWrite(PUMP_ENABLE, HIGH);
  delay(500);
   digitalWrite(PUMP_ENABLE, LOW);
  delay(500);
   digitalWrite(PUMP_ENABLE, HIGH);
  delay(500);
   digitalWrite(PUMP_ENABLE, LOW);
  delay(500);*/


  // Configure digital inputs for flame sensor
  pinMode(FLAME_SENSOR_CHANNEL_D1, INPUT);
  pinMode(FLAME_SENSOR_CHANNEL_D2, INPUT);
  pinMode(FLAME_SENSOR_CHANNEL_D3, INPUT);
  pinMode(FLAME_SENSOR_CHANNEL_D4, INPUT);
  pinMode(FLAME_SENSOR_CHANNEL_D5, INPUT);

  Serial.println("Init OK");
  color_show(LED_RED);
  delay(500);
  color_show(LED_GREEN);
  delay(500);
  color_show(LED_BLUE);
  delay(500);
  rgbOff();
}

void loop() {
  unsigned long randomTurnDistance;

  bool isAligned;

  delay(500);
  ////////////////////////////////////////////// To do: sense flame
  flameStateChannel1 = digitalRead(FLAME_SENSOR_CHANNEL_D1);
  flameStateChannel2 = digitalRead(FLAME_SENSOR_CHANNEL_D2);
  flameStateChannel3 = digitalRead(FLAME_SENSOR_CHANNEL_D3);
  flameValChannel3 = analogRead(FLAME_SENSOR_CHANNEL_A3);
  flameStateChannel4 = digitalRead(FLAME_SENSOR_CHANNEL_D4);
  flameStateChannel5 = digitalRead(FLAME_SENSOR_CHANNEL_D5);

  if (PRINT_STUFF == 1)
  {
    Serial.print("CH1 DIG: ");
    Serial.print(flameStateChannel1);
    Serial.print(" \\ CH2 DIG: ");
    Serial.print(flameStateChannel2);
    Serial.print(" \\ CH3 DIG: ");
    Serial.print(flameStateChannel3);
    Serial.print(" \\ CH3 ANA: ");
    Serial.print(flameValChannel3);
    Serial.print(" \\ CH4 DIG: ");
    Serial.print(flameStateChannel4);
    Serial.print(" \\ CH5 DIG: ");
    Serial.println(flameStateChannel5);
  }

  isAligned = 0;
  
  if ((flameStateChannel1 == 1) || (flameStateChannel2 == 1) || (flameStateChannel3 == 1) || (flameStateChannel4 == 1) || (flameStateChannel5 == 1)) {
    carStop();
    delay(250);
    color_show(LED_PINK);


    // Begin single LED check
    if ((flameStateChannel1 == 1) && (flameStateChannel2 == 0) && (flameStateChannel3 == 0) && (flameStateChannel4 == 0) && (flameStateChannel5 == 0)) 
    {
      carTurnRight(power_turnRight);
      delay(TURN_TWO);
      // Stop the car
      carStop();
    } 
    else if ((flameStateChannel1 == 0) && (flameStateChannel2 == 1) && (flameStateChannel3 == 0) && (flameStateChannel4 == 0) && (flameStateChannel5 == 0)) 
    {
      carTurnRight(power_turnRight);
      delay(TURN_ONE);
      // Stop the car
      carStop();
    } 
    else if ((flameStateChannel1 == 0) && (flameStateChannel2 == 0) && (flameStateChannel3 == 1) && (flameStateChannel4 == 0) && (flameStateChannel5 == 0)) {
      isAligned=1;
    } 
    else if ((flameStateChannel1 == 0) && (flameStateChannel2 == 0) && (flameStateChannel3 == 0) && (flameStateChannel4 == 1) && (flameStateChannel5 == 0)) {
      carTurnLeft(power_turnLeft);
      delay(TURN_ONE);
      // Stop the car
      carStop();
    } 
    else if ((flameStateChannel1 == 0) && (flameStateChannel2 == 0) && (flameStateChannel3 == 0) && (flameStateChannel4 == 0) && (flameStateChannel5 == 1)) {
      carTurnLeft(power_turnLeft);
      delay(TURN_TWO);
      // Stop the car
      carStop();
      
    }
    //////////////////////////////////////////////////////////Begin double LED check
    else if ((flameStateChannel1 == 1) && (flameStateChannel2 == 1) && (flameStateChannel3 == 0) && (flameStateChannel4 == 0) && (flameStateChannel5 == 0)) 
    {
      carTurnRight(power_turnRight);
      delay(TURN_ONE_AND_HALF);
      // Stop the car
      carStop();
    } else if ((flameStateChannel1 == 0) && (flameStateChannel2 == 1) && (flameStateChannel3 == 1) && (flameStateChannel4 == 0) && (flameStateChannel5 == 0)) {
      carTurnRight(power_turnRight);
      delay(TURN_HALF);
      // Stop the car
      carStop();
    } else if ((flameStateChannel1 == 0) && (flameStateChannel2 == 0) && (flameStateChannel3 == 1) && (flameStateChannel4 == 1) && (flameStateChannel5 == 0)) {
      carTurnLeft(power_turnLeft);
      delay(TURN_HALF);
      // Stop the car
      carStop();
    } else if ((flameStateChannel1 == 0) && (flameStateChannel2 == 0) && (flameStateChannel3 == 0) && (flameStateChannel4 == 1) && (flameStateChannel5 == 1)) {
      carTurnLeft(power_turnLeft);
      delay(TURN_ONE_AND_HALF);
      // Stop the car
      carStop();
    }
    //begin triple IR check

    else if ((flameStateChannel1 == 1) && (flameStateChannel2 == 1) && (flameStateChannel3 == 1) && (flameStateChannel4 == 0) && (flameStateChannel5 == 0)) {
      carTurnRight(power_turnRight);
      delay(TURN_ONE);
      // Stop the car
      carStop();
    } else if ((flameStateChannel1 == 0) && (flameStateChannel2 == 1) && (flameStateChannel3 == 1) && (flameStateChannel4 == 1) && (flameStateChannel5 == 0)) {
      isAligned=1;

    } else if ((flameStateChannel1 == 0) && (flameStateChannel2 == 0) && (flameStateChannel3 == 1) && (flameStateChannel4 == 1) && (flameStateChannel5 == 1)) {
      carTurnLeft(power_turnLeft);
      delay(TURN_ONE);
      // Stop the car
      carStop();
    }
    //Begin quad IR check
    else if ((flameStateChannel1 == 1) && (flameStateChannel2 == 1) && (flameStateChannel3 == 1) && (flameStateChannel4 == 1) && (flameStateChannel5 == 0)) {
      carTurnRight(power_turnRight);
      delay(TURN_HALF);
      // Stop the car
      carStop();
    } else if ((flameStateChannel1 == 0) && (flameStateChannel2 == 1) && (flameStateChannel3 == 1) && (flameStateChannel4 == 1) && (flameStateChannel5 == 1)) {
      carTurnLeft(power_turnLeft);
      delay(TURN_HALF);
      // Stop the car
      carStop();
    }
    //Begin penta IR check
    else if ((flameStateChannel1 == 1) && (flameStateChannel2 == 1) && (flameStateChannel3 == 1) && (flameStateChannel4 == 1) && (flameStateChannel5 == 1)) {
      isAligned=1;

    }
  
  } else { // if no flame detected
    rgbOff();
  }

  //begin distance adjustment if aligned
  if(isAligned==1){
    flameValChannel3 = analogRead(FLAME_SENSOR_CHANNEL_A3);

    if(flameValChannel3 > 700){
      if (PRINT_STUFF == 1)
      {
        Serial.println("TOO CLOSE");
      }

      carBackward(power_driveBackward); 
      delay(random(80, 120));
      carStop();
      delay(250);
    }

    else if(flameValChannel3 < 400){
      if (PRINT_STUFF == 1)
      {
        Serial.println("TOO FAR");
      }

      carForward(power_driveForward);
      delay(random(80, 120));
      carStop();
      delay(250);
    }

    else{
      if (PRINT_STUFF == 1)
      {
        Serial.println("JUST RIGHT");
      }

      spray();
    }
  }
  /////////////////////////////////////////////

  //float distance = ultrasonicRead();
  //Serial.print("distance: ");Serial.print(distance);
  bool is_obstacle = ultrasonicIsObstacle();
  //Serial.print("  is_obstacle: ");
  //Serial.println(is_obstacle);

  ///////////// The obstacle detection stuff
  if (is_obstacle == 1)
  {
    while (is_obstacle)  // true if obstacle detected
    {
      color_show(LED_RED);

      // Stop the car and wait 1 second
      carStop();
      delay(1000);

      // Compute a random turn time and then turn the car that amount of time
      randomTurnDistance = random(MIN_TURN_ANGLE, MAX_TURN_ANGLE);
      //Serial.print("Turn Time: "); 
      //Serial.println(randomTurnDistance);
      carTurnRight(power_turnRight);
      delay(randomTurnDistance);

      // Stop the car and wait 1/2 a second
      carStop();
      delay(500);

      // Check to see if there is still an obstscle
      is_obstacle = ultrasonicIsObstacle();
    }
  }
else
{
  rgbOff();
  carForward(power_driveForward);
  //delay(200);
  //carStop();
  //delay(COLLISION_DETECTION_INTERVAL);
}

  //delay(200);

 
}


void carBegin() {
  for (uint8_t i = 0; i < 8; i++) {
    SoftPWMSet(MOTOR_PINS[i], 0);
    SoftPWMSetFadeTime(MOTOR_PINS[i], 100, 100);
  }
}

void spray()
{
  uint8_t sprayTime = 200;
  uint8_t turnTimeForSprayLeft = 90;
  uint8_t turnTimeForSprayRight = 130;

  color_show(LED_BLUE);

//Turn spray on and off 
digitalWrite(PUMP_ENABLE, HIGH);
delay(sprayTime);
digitalWrite(PUMP_ENABLE, LOW);
delay(500);

// Turn right and stop
carTurnRight(power_turnRight);
delay(turnTimeForSprayRight);
carStop();
delay(500);

//Turn spray on and off
digitalWrite(PUMP_ENABLE, HIGH);
delay(sprayTime);
digitalWrite(PUMP_ENABLE, LOW);
delay(500);

//Turn left and stop
carTurnLeft(power_turnLeft);
delay(turnTimeForSprayRight+turnTimeForSprayLeft);
carStop();
delay(500);

//Turn spray on and off
digitalWrite(PUMP_ENABLE, HIGH);
delay(sprayTime);
digitalWrite(PUMP_ENABLE, LOW);
delay(500);

//Return to original rotation
carTurnRight(power_turnRight);
delay(turnTimeForSprayLeft);
carStop();
delay(500);

}

void carSetMotors(int8_t power0, int8_t power1, int8_t power2, int8_t power3) {
  bool dir[4];
  int8_t power[4] = { power0, power1, power2, power3 };
  int8_t newPower[4];

  for (uint8_t i = 0; i < 4; i++) {
    dir[i] = power[i] > 0;

    if (MOTOR_DIRECTIONS[i]) dir[i] = !dir[i];

    if (power[i] == 0) {
      newPower[i] = 0;
    } else {
      newPower[i] = map(abs(power[i]), 0, 100, MOTOR_POWER_MIN, 255);
    }
    SoftPWMSet(MOTOR_PINS[i * 2], dir[i] * newPower[i]);
    SoftPWMSet(MOTOR_PINS[i * 2 + 1], !dir[i] * newPower[i]);
  }
}

void carForward(int8_t power) {
  carSetMotors(power, power, power, power);
}

void carBackward(int8_t power) {
  carSetMotors(-power, -power, -power, -power);
}

void carLeft(int8_t power) {
  carSetMotors(-power, power, -power, power);
}

void carRight(int8_t power) {
  carSetMotors(power, -power, power, -power);
}

void carLeftForward(int8_t power) {
  carSetMotors(0, power, 0, power);
}

void carLeftBackward(int8_t power) {
  carSetMotors(-power, 0, -power, 0);
}

void carRightForward(int8_t power) {
  carSetMotors(power, 0, power, 0);
}

void carRightBackward(int8_t power) {
  carSetMotors(0, -power, 0, -power);
}

void carTurnLeft(int8_t power) {
  carSetMotors(-power, power, power, -power);
}

void carTurnRight(int8_t power) {
  carSetMotors(power, -power, -power, power);
}

void carStop() {
  carSetMotors(0, 0, 0, 0);
}

/** Return the distance read by the ultrasonic module, unit cm */
float ultrasonicRead() {
  delay(4);  // A delay is required between consecutive readings, otherwise the reading may be 0

  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);

  // noInterrupts(); // Pause all interruptions to avoid affecting data
  // However，turning off interrupts affects the functionality of softpwm, such as motors

  float duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);
  float distance = duration * 0.017;  // S = vt = 340m/s * t_us / 2= (340 * 100 cm/s) * 0.5 * (t / 10^6)s = 0.017 * t (divided by 2 (go and back))

  // interrupts(); // resume interruptions

  if (distance > MAX_DISTANCE) {
    return -1;
  }
  return distance;
}


/** Determine whether there is an obstacle ahead according to the set ULTRASONIC_AVOIDANCE_THRESHOLD */
bool ultrasonicIsObstacle() {
  return ultrasonicRead() < ULTRASONIC_AVOIDANCE_THRESHOLD;
}

/** Determine whether there is clear ahead according to the set ULTRASONIC_AVOIDANCE_THRESHOLD */
bool ultrasonicIsClear() {
  return ultrasonicRead() > ULTRASONIC_AVOIDANCE_THRESHOLD;
}

/** Initialize RGB leds */
void rgbBegin() {
  for (uint8_t i = 0; i < 3; i++) {
    SoftPWMSet(RGB_PINS[i], 0);
    SoftPWMSetFadeTime(RGB_PINS[i], 100, 100);
  }
}

/** Set LED color in HEX */
void rgbWrite(uint32_t color) {
  uint8_t r = (color >> 16) & 0xFF;
  uint8_t g = (color >> 8) & 0xFF;
  uint8_t b = (color >> 0) & 0xFF;
  rgbWrite(r, g, b);
}


/** Set LED color in 8bit R,G,B (0 ~ 255)*/
void rgbWrite(uint8_t r, uint8_t g, uint8_t b) {
  // calibrate brightness
  r = int(r * 1);
  g = int(g * 0.16);
  b = int(b * 0.30);
// COMMON_ANODE reverse
#if COMMON_ANODE
  r = 255 - r;
  g = 255 - g;
  b = 255 - b;
#endif
  // set volatge
  SoftPWMSet(RGB_PINS[0], r);
  SoftPWMSet(RGB_PINS[1], g);
  SoftPWMSet(RGB_PINS[2], b);
}

/** Turn odd RGB LEDs*/
void rgbOff() {
  rgbWrite(0, 0, 0);
}

void color_show(uint8_t index) {
  uint8_t r = key_color_array[index][0];
  uint8_t g = key_color_array[index][1];
  uint8_t b = key_color_array[index][2];

  //Serial.print("r:"); Serial.print(r);
  //Serial.print(" ,g:"); Serial.print(g);
  //Serial.print(" ,b:"); Serial.println(b);

  rgbWrite(r, g, b);
  delay(20);
}