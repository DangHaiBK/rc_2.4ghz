#define RX_INPUT_CHANNEL_1 8
#define RX_INPUT_CHANNEL_2 9
#define RX_INPUT_CHANNEL_3 10

#define MOTOR_INPUT_PIN_1       6
#define MOTOR_INPUT_PIN_2       7

#define RX_INPUT_MAP_CHANNEL_1_TO_NUMBER 0
#define RX_INPUT_MAP_CHANNEL_2_TO_NUMBER 1
#define RX_INPUT_MAP_CHANNEL_3_TO_NUMBER 2

#define RECEIVER_PWM_NEUTRAL          1500    /* Typical value */
#define RECEIVER_PWM_MAX              2000    /* Typical value */
#define RECEIVER_PWM_MIN              1000    /* Typical value */
#define RECEIVER_ERROR_RATE           30      /* Typical value +- this value */
#define RECEIVER_FRESH_RATE           100     /* [MS] Frame rate, typical 10 - 300 Hz */

#define RECEIVER_STICK_MIDDLE         0
#define RECEIVER_STICK_INCREASING     1
#define RECEIVER_STICK_DECREASING     2

#define LED_FORWARD         0
#define LED_BEACON          1
#define LED_REVERSED        2
#define LED_BRAKE           3
#define LED_LEFT            4
#define LED_RIGHT           5

#define TIME_PERIOD       500

#define A_INC     0.46
#define B_INC    -690

#define A_DEC    -0.25
#define B_DEC     375

#define RX_TO_PWM(x, a, b) ((a) * (x) + (b))

//unsigned long ulValue = 0;
volatile bool rx_new_pulse[] = {false, false, false};
volatile unsigned long rx_pulse_begin[] = {0, 0, 0};
volatile unsigned long rx_channel_width[] = {0, 0, 0};

bool brake_active_flag = false;
bool turn_signal_flag = false;
bool beacon_flag = true;

uint8_t count_hazard = 0;
uint8_t count_signal = 0;
uint8_t count_stop = 0;
uint8_t count_beacon = 0;

uint8_t directIdle = 0;
uint16_t valPWMIdle = 0;

uint8_t direct[] = { 0, 0, 0 };
uint16_t val_pwm[] = { 0, 0, 0 };

void setup() {
  // put your setup code here, to run once:
  rx_init();
  MOTOR_Init();
  Serial1.begin(115200);

  while (valPWMIdle != RECEIVER_PWM_NEUTRAL)
  {
    RECEIVER_Read_Pulse(0, &directIdle, &valPWMIdle);
    if (valPWMIdle == RECEIVER_PWM_NEUTRAL)
    {
      break;
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(rx_channel_width[0]);
  for(uint8_t i=0; i<3; i++)
  {
    RECEIVER_Read_Pulse(i, &direct[i], &val_pwm[i]);
  }
  Serial1.println(val_pwm[2]);
  Serial1.println(direct[2]);

  /* Beacon light toggle */
  if (beacon_flag == true) {
    count_beacon ++;
    LED_Beacon(count_beacon);
  }

  /* Forward and Reversed Directions */
  if ((direct[0] == RECEIVER_STICK_MIDDLE) && (val_pwm[0] == RECEIVER_PWM_NEUTRAL))
  {
     count_stop ++;
     if (count_stop == 5) {
        brake_active_flag = true; 
     }
     if (brake_active_flag == true) {
        LED_Brake_Active(count_stop);
     }
     if (count_stop > 5) {
        count_stop = 6;
     }
     MOTOR_Set_Stop();
  }
  if (direct[0] == RECEIVER_STICK_INCREASING)
  {
    count_stop = 0;
      if (val_pwm[0] >= RECEIVER_PWM_MAX)
      {
          MOTOR_Set_Full_Speed_Forward();
      }
      else
      {
          MOTOR_Set_Forward_PWM(val_pwm[0]);
      }
  }
  if (direct[0] == RECEIVER_STICK_DECREASING)
  {
    count_stop = 0;
      if (val_pwm[0] <= RECEIVER_PWM_MIN)
      {
          MOTOR_Set_Full_Speed_Reversed();
      }
      else
      {
          MOTOR_Set_Reversed_PWM(val_pwm[0]);
      }
  }
  
  /* Turn Left and turn Right */
  if (direct[1] == RECEIVER_STICK_MIDDLE)
  {
    count_signal = 0;
  }
  if (direct[1] == RECEIVER_STICK_INCREASING)
  {
    count_signal ++;
    if (turn_signal_flag == true) {
      LED_Left(count_signal);
    }
  }
  if (direct[1] == RECEIVER_STICK_DECREASING)
  {
    count_signal ++;
    if (turn_signal_flag == true) {
      LED_Right(count_signal);
    }
  }
  
  /* AUX Function for Forward Led and Hazard Leds */
  if (direct[2] == RECEIVER_STICK_MIDDLE)
  {
    count_hazard = 0;
    turn_signal_flag = true;
    LED_Forward_Off();
  }
  if (direct[2] == RECEIVER_STICK_INCREASING)
  {
    LED_Forward_On();
  }
  if (direct[2] == RECEIVER_STICK_DECREASING)
  {
    count_hazard ++;
    turn_signal_flag = false;
    LED_Hazard(count_hazard);
  }
  
  /* Refresher Rate */
  delay(RECEIVER_FRESH_RATE);
}

void rx_init(void)
{
  pinMode(RX_INPUT_CHANNEL_1, INPUT);
  pinMode(RX_INPUT_CHANNEL_2, INPUT);
  pinMode(RX_INPUT_CHANNEL_3, INPUT);
  attachInterrupt(digitalPinToInterrupt(RX_INPUT_CHANNEL_1), measuring_CH1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RX_INPUT_CHANNEL_2), measuring_CH2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RX_INPUT_CHANNEL_2), measuring_CH2, CHANGE);
}

static void rx_read_raw(uint8_t pin, uint8_t channel)
{
  if (digitalRead(pin) == HIGH)
  {
    rx_pulse_begin[channel] = micros();
    //Serial.println("rising edge");
  }
  else 
  {
    //Serial.println("falling edge");
    rx_channel_width[channel] = micros() - rx_pulse_begin[channel];
    rx_new_pulse[channel] = true;
  }
}

void measuring_CH1(void)
{
  rx_read_raw(RX_INPUT_CHANNEL_1, RX_INPUT_MAP_CHANNEL_1_TO_NUMBER);
}

void measuring_CH2(void)
{
  rx_read_raw(RX_INPUT_CHANNEL_2, RX_INPUT_MAP_CHANNEL_2_TO_NUMBER);
}

void measuring_CH3(void)
{
  rx_read_raw(RX_INPUT_CHANNEL_3, RX_INPUT_MAP_CHANNEL_3_TO_NUMBER);
}

static bool rx_data_is_receiving (uint8_t channel)
{
    return rx_new_pulse[channel];
}

static void rx_remove_incoming_data_flag (uint8_t channel)
{
    rx_new_pulse[channel] = false;
}

unsigned long rx_read_pwm (uint8_t channel)
{
    //if (rx_data_is_receiving(channel))
    //{
        rx_remove_incoming_data_flag(channel);
        return rx_channel_width[channel];
   // }
}

void RECEIVER_Read_Pulse (uint8_t channel, uint8_t *direct, uint16_t *val)
{
    unsigned long ulValue = rx_read_pwm(channel);

    if (ulValue > RECEIVER_PWM_NEUTRAL)
    {
       if ((ulValue - RECEIVER_PWM_NEUTRAL) > RECEIVER_ERROR_RATE)
       {
         *direct = RECEIVER_STICK_INCREASING;
         *val = ulValue;
       }
       else 
       {
         *direct = RECEIVER_STICK_MIDDLE;
         *val = RECEIVER_PWM_NEUTRAL;
       }
    }
    if (ulValue < RECEIVER_PWM_NEUTRAL)
    {
       if ((RECEIVER_PWM_NEUTRAL - ulValue) > RECEIVER_ERROR_RATE)
       {
         *direct = RECEIVER_STICK_DECREASING;
         *val = ulValue;
       }
       else 
       {
         *direct = RECEIVER_STICK_MIDDLE;
         *val = RECEIVER_PWM_NEUTRAL;
       }
    }

    if (ulValue > RECEIVER_PWM_MAX)
    {
      *direct = RECEIVER_STICK_INCREASING;
      *val = RECEIVER_PWM_MAX;
    }
    else 
    {
      if ((RECEIVER_PWM_MAX - ulValue) <= RECEIVER_ERROR_RATE)
      {
        *direct = RECEIVER_STICK_INCREASING;
        *val = RECEIVER_PWM_MAX;
      }
    }

    if (ulValue <= RECEIVER_PWM_MIN)
    {
      *direct = RECEIVER_STICK_DECREASING;
      *val = RECEIVER_PWM_MIN;
    }
    else
    {
      if ((ulValue - RECEIVER_PWM_MIN) <= RECEIVER_ERROR_RATE)
      {
        *direct = RECEIVER_STICK_DECREASING;
        *val = RECEIVER_PWM_MIN;
      }
    }
}

void drv8833_attach_signal_motorA (uint8_t ip1, uint8_t ip2)
{
    pinMode(ip1, OUTPUT);
    pinMode(ip2, OUTPUT);
}

void MOTOR_Init(void)
{
  drv8833_attach_signal_motorA(MOTOR_INPUT_PIN_1, MOTOR_INPUT_PIN_2);
}

void drv8833_set_stop_motorA (uint8_t p1, uint8_t p2)
{
    digitalWrite(p1, LOW);
    digitalWrite(p2, LOW);
}

void MOTOR_Set_Stop (void)
{
    drv8833_set_stop_motorA(MOTOR_INPUT_PIN_1, MOTOR_INPUT_PIN_2);
}

void drv8833_set_full_forward_motorA (uint8_t p1, uint8_t p2)
{
    digitalWrite(p1, LOW);
    analogWrite(p2, 230);
}

void MOTOR_Set_Full_Speed_Forward (void)
{
    drv8833_set_full_forward_motorA(MOTOR_INPUT_PIN_1, MOTOR_INPUT_PIN_2);
}

void drv8833_set_full_reversed_motorA (uint8_t p1, uint8_t p2)
{
    digitalWrite(p2, LOW);
    analogWrite(p1, 127);
}

void MOTOR_Set_Full_Speed_Reversed (void)
{
    drv8833_set_full_reversed_motorA(MOTOR_INPUT_PIN_1, MOTOR_INPUT_PIN_2);
}

void drv8833_set_forward_pwm_motorA (uint8_t p1, uint8_t p2, uint16_t val)
{
    digitalWrite(p1, LOW);
    analogWrite(p2, val);
}

void MOTOR_Set_Forward_PWM (uint16_t val)
{
    drv8833_set_forward_pwm_motorA(MOTOR_INPUT_PIN_1, MOTOR_INPUT_PIN_2, RX_TO_PWM(val, A_INC, B_INC));
}

void drv8833_set_reversed_pwm_motorA (uint8_t p1, uint8_t p2, uint16_t val)
{
    digitalWrite(p2, LOW);
    analogWrite(p1, val);
}

void MOTOR_Set_Reversed_PWM (uint16_t val)
{
    drv8833_set_reversed_pwm_motorA(MOTOR_INPUT_PIN_1, MOTOR_INPUT_PIN_2, RX_TO_PWM(val, A_DEC, B_DEC));
}

void LED_Init (void)
{
  pinMode(LED_FORWARD, OUTPUT);
  pinMode(LED_REVERSED, OUTPUT);
  pinMode(LED_BRAKE, OUTPUT);
  pinMode(LED_LEFT, OUTPUT);
  pinMode(LED_RIGHT, OUTPUT);
  pinMode(LED_BEACON, OUTPUT);
}

void LED_Forward_On (void)
{
  digitalWrite(LED_FORWARD, HIGH);
}

void LED_Forward_Off (void)
{
  digitalWrite(LED_FORWARD, LOW);
}

void LED_Reversed_On (void)
{
  digitalWrite(LED_REVERSED, HIGH);
}

void LED_Reversed_Off (void)
{
  digitalWrite(LED_REVERSED, LOW);
}

void LED_Left (uint8_t period)
{
  digitalWrite(LED_RIGHT, LOW);
  if (period % TIME_PERIOD == 0)
  {
    if (digitalRead(LED_LEFT) == LOW) {
      digitalWrite(LED_LEFT, HIGH);
    }
    else {
      digitalWrite(LED_LEFT, LOW);
    }
  }
}

void LED_Right (uint8_t period)
{
  digitalWrite(LED_LEFT, LOW);
  if (period % TIME_PERIOD == 0)
  {
    if (digitalRead(LED_RIGHT) == LOW) {
      digitalWrite(LED_RIGHT, HIGH);
    }
    else {
      digitalWrite(LED_RIGHT, LOW);
    }
  }
}

void LED_Brake_Active (uint8_t period)
{
  digitalWrite(LED_BRAKE, HIGH);
  if (period % TIME_PERIOD == 0) {
    digitalWrite(LED_BRAKE, LOW);
  }
}

void LED_Brake_Off (void)
{
  digitalWrite(LED_BRAKE, LOW);
}

void LED_Beacon (uint8_t period)
{
  if (period % TIME_PERIOD == 0)
  {
    if (digitalRead(LED_BEACON) == LOW) {
      digitalWrite(LED_BEACON, HIGH);
    }
    else {
      digitalWrite(LED_BEACON, LOW);
    }
  }
}

void LED_Hazard (uint8_t period)
{
  if ((period / TIME_PERIOD) % 2 == 0) {
      digitalWrite(LED_LEFT, LOW);
      digitalWrite(LED_RIGHT, LOW);
  }
  else {
      digitalWrite(LED_LEFT, HIGH);
      digitalWrite(LED_RIGHT, HIGH);
  }
}
