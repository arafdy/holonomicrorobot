//holonomic robot by rafdy amestira
//featuring rendy devara 
// 28 nov 2018


#define BRAKE 0
#define CW    1
#define CCW   2
#define CS_THRESHOLD 15   // Definition of safety current (Check: "1.3 Monster Shield Example").

//MOTOR 1
#define MOTOR_A1_PIN 22
#define MOTOR_B1_PIN 24
#define PWM_MOTOR_1 2
#define MOTOR_1 0

//MOTOR 2
#define MOTOR_A2_PIN 28
#define MOTOR_B2_PIN 26
#define PWM_MOTOR_2 3
#define MOTOR_2 1


//MOTOR 3
#define MOTOR_A3_PIN 32
#define MOTOR_B3_PIN 30
#define PWM_MOTOR_3 4
#define MOTOR_3 2

//tuning
int trim1 = 0;
int trim2 = 0;
int trim3 = 0;

// variable xyz
int X, Y, Z;
float kz = 0.5;

short usSpeed = 150;  //default motor speed
unsigned short usMotor_Status = BRAKE;

void setup()
{
  pinMode(MOTOR_A1_PIN, OUTPUT);
  pinMode(MOTOR_B1_PIN, OUTPUT);

  pinMode(MOTOR_A2_PIN, OUTPUT);
  pinMode(MOTOR_B2_PIN, OUTPUT);

  pinMode(MOTOR_A3_PIN, OUTPUT);
  pinMode(MOTOR_B3_PIN, OUTPUT);

  pinMode(PWM_MOTOR_1, OUTPUT);
  pinMode(PWM_MOTOR_2, OUTPUT);

  //  pinMode(CURRENT_SEN_1, OUTPUT);
  //  pinMode(CURRENT_SEN_2, OUTPUT);

  //  pinMode(EN_PIN_1, OUTPUT);
  //  pinMode(EN_PIN_2, OUTPUT);

  Serial.begin(115200);              // Initiates the serial to do the monitoring
  Serial3.begin(115200);        //nunchuck Bluetooth

  Serial.println("Begin motor control");
  Serial.println(); //Print function list for user selection
  Serial.println("Enter number for control option:");
  Serial.println("1. STOP");
  Serial.println("2. FORWARD");
  Serial.println("3. REVERSE");
  Serial.println("4. READ CURRENT");
  Serial.println("+. INCREASE SPEED");
  Serial.println("-. DECREASE SPEED");
  Serial.println();

}

void menuSerial() {

  char user_input;

  while (Serial.available())
  {
    user_input = Serial.read(); //Read user input and trigger appropriate function
    //    digitalWrite(EN_PIN_1, HIGH);
    //    digitalWrite(EN_PIN_2, HIGH);

    if (user_input == '1')
    {
      Stop();
    }
    else if (user_input == '2')
    {
      Forward();
    }
    else if (user_input == '3')
    {
      Reverse();
    }
    else if (user_input == '+')
    {
      IncreaseSpeed();
    }
    else if (user_input == '-')
    {
      DecreaseSpeed();
    }
    else if (user_input == 'w')
    {
      Serial.println("Majuuu");
      calcMotors(0, 500, 0);
    }
    else if (user_input == 'a')
    {
      Serial.println("Kirii");
      calcMotors(500, 0, 0);
    } else if (user_input == 's')
    {
      Serial.println("mundurrr"); 
      calcMotors(0, -500, 0);
    }
    else if (user_input == 'd')
    {
      Serial.println("Kanannn");
      calcMotors(500, 0, 0);
    }
    else
    {
      Serial.println("Invalid option entered.");
    }

  }
}

void calcMotors(float x, float y, float z)
{
  if (x > 500)
    x = 500;
  if (x < -500)
    x = -500;
  if (y > 500)
    x = 500;
  if (y < -500)
    x = -500;


  //STEP 2. Convert them to a direction vector
  float theta = atan2(x, y);
  float magnitude = sqrt((x * x) + (y * y));

  // magnitudes less than about 50 are neutral stick positions
  // and should probably just be ignored
  if (magnitude > 75.0f || Z != 0) {

    Serial.print("x: ");
    Serial.print(x);
    Serial.print(" y: ");
    Serial.print(y);

    Serial.print(" Angle: ");
    Serial.print(degrees(theta));

    Serial.print(" Magnitude: ");
    Serial.print(magnitude);

    float vx = magnitude * cos(theta);
    float vy = magnitude * sin(theta);
    const float sqrt3o2 = 1.0 * sqrt(3) / 2;
    //STEP 3. Find wheel vecotrs
    float w0 = -vx;                   // v dot [-1, 0] / 25mm
    float w1 = 0.5 * vx - sqrt3o2 * vy; // v dot [1/2, -sqrt(3)/2] / 25mm
    float w2 = 0.5 * vx + sqrt3o2 * vy; // v dot [1/2, +sqrt(3)/2] / 25mm

    z = z * kz;
    w0 += Z;
    w1 += Z;
    w2 += Z;

    boolean w0_ccw = w0 < 0 ? true : false;
    boolean w1_ccw = w1 < 0 ? true : false;
    boolean w2_ccw = w2 < 0 ? true : false;
    byte w0_speed = (byte) map(abs(w0), 0, 600, 0, 255);
    byte w1_speed = (byte) map(abs(w1), 0, 600, 0, 255);
    byte w2_speed = (byte) map(abs(w2), 0, 600, 0, 255);

    Serial.print(" vx: ");
    Serial.print(vx);
    Serial.print(" vy: ");
    Serial.print(vy);

    Serial.print(" w0: ");
    Serial.print(w0_speed);
    if (w0_ccw) Serial.print(" CCW"); else Serial.print(" CW");
    Serial.print(" w1: ");
    Serial.print(w1_speed);
    if (w1_ccw) Serial.print(" CCW"); else Serial.print(" CW");
    Serial.print(" w2: ");
    Serial.print(w2_speed);
    if (w2_ccw) Serial.print(" CCW"); else Serial.print(" CW");
    Serial.println();

    if (w0_speed > 255)
      w0_speed = 255;
    if (w0_speed < 0)
      w0_speed = 0;
    if (w1_speed > 255)
      w1_speed = 255;
    if (w1_speed < 0)
      w1_speed = 0;
    if (w2_speed > 255)
      w2_speed = 255;
    if (w2_speed < 0)
      w2_speed = 0;


    motorGo(MOTOR_1, (w0_ccw ? CCW : CW), w0_speed);
    motorGo(MOTOR_2, (w1_ccw ? CCW : CW), w1_speed);
    motorGo(MOTOR_3, (w2_ccw ? CCW : CW), w2_speed);


  }
  else { // stick is idle, stop the wheels
    //    m0.setSpeed(0);
    //    m1.setSpeed(0);
    //    m2.setSpeed(0);

    motorGo(MOTOR_1, BRAKE, 0);
    motorGo(MOTOR_2, BRAKE, 0);
    motorGo(MOTOR_3, BRAKE, 0);
  }
}
void loop()
{


  pollSerial3();
  menuSerial();

}

void Stop()
{
  Serial.println("Stop");
  usMotor_Status = BRAKE;
  motorGo(MOTOR_1, usMotor_Status, 0);
  motorGo(MOTOR_2, usMotor_Status, 0);
  motorGo(MOTOR_3, usMotor_Status, 0);
}

void Forward()
{
  Serial.println("Forward");
  usMotor_Status = CW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed);
  motorGo(MOTOR_3, usMotor_Status, usSpeed);
}

void Reverse()
{
  Serial.println("Reverse");
  usMotor_Status = CCW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed);
  motorGo(MOTOR_3, usMotor_Status, usSpeed);
}

void IncreaseSpeed()
{
  usSpeed = usSpeed + 10;
  if (usSpeed > 255)
  {
    usSpeed = 255;
  }

  Serial.print("Speed +: ");
  Serial.println(usSpeed);

  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed);
  motorGo(MOTOR_3, usMotor_Status, usSpeed);
}

void DecreaseSpeed()
{
  usSpeed = usSpeed - 10;
  if (usSpeed < 0)
  {
    usSpeed = 0;
  }

  Serial.print("Speed -: ");
  Serial.println(usSpeed);

  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed);
  motorGo(MOTOR_3, usMotor_Status, usSpeed);
}

void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm)         //Function that controls the variables: motor(0 ou 1), direction (cw ou ccw) e pwm (entra 0 e 255);
{
  if (motor == MOTOR_1)
  {
    if (direct == CW)
    {
      digitalWrite(MOTOR_A1_PIN, LOW);
      digitalWrite(MOTOR_B1_PIN, HIGH);
    }
    else if (direct == CCW)
    {
      digitalWrite(MOTOR_A1_PIN, HIGH);
      digitalWrite(MOTOR_B1_PIN, LOW);
    }
    else
    {
      digitalWrite(MOTOR_A1_PIN, LOW);
      digitalWrite(MOTOR_B1_PIN, LOW);
    }

    analogWrite(PWM_MOTOR_1, pwm + trim1);
  }
  else if (motor == MOTOR_2)
  {
    if (direct == CW)
    {
      digitalWrite(MOTOR_A2_PIN, LOW);
      digitalWrite(MOTOR_B2_PIN, HIGH);
    }
    else if (direct == CCW)
    {
      digitalWrite(MOTOR_A2_PIN, HIGH);
      digitalWrite(MOTOR_B2_PIN, LOW);
    }
    else
    {
      digitalWrite(MOTOR_A2_PIN, LOW);
      digitalWrite(MOTOR_B2_PIN, LOW);
    }

    analogWrite(PWM_MOTOR_2, pwm + trim2);
  }
  else if (motor == MOTOR_3)
  {
    if (direct == CW)
    {
      digitalWrite(MOTOR_A3_PIN, LOW);
      digitalWrite(MOTOR_B3_PIN, HIGH);
    }
    else if (direct == CCW)
    {
      digitalWrite(MOTOR_A3_PIN, HIGH);
      digitalWrite(MOTOR_B3_PIN, LOW);
    }
    else
    {
      digitalWrite(MOTOR_A3_PIN, LOW);
      digitalWrite(MOTOR_B3_PIN, LOW);
    }

    analogWrite(PWM_MOTOR_3, pwm + trim3);
  }
}
void pollSerial3()
{
  if (Serial3.available())
  {
    //    Serial.print((char)Serial3.read());
    if (Serial3.read() == '$')
    {
      char rxData[16];
      memset((void*)rxData, 0, sizeof(rxData));
      Serial3.readBytesUntil('\n', rxData, 16);
      //          Serial.println(rxData);
      sscanf(rxData, "%d,%d,%d", &X, &Y, &Z);
      //          Serial.println("X="+(String)X + " Y="+(String)Y + " Z="+ (String)Z);
      calcMotors(X, Y, Z);
    }
  }
}
