#include <IRremote.h>

// number key remote control
const int KEY_RC = 3772790473;

int RECV_PIN = 13;

// led pin
const int LED_PIN = 8;

// led status
int LED_STATUS = 0;

IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  Serial.begin(9600);

  // define LED_PIN digital output
  pinMode(LED_PIN, OUTPUT);
  
  // start the receiver
  irrecv.enableIRIn();
}

void loop() {
  if (irrecv.decode(&results)) {
    // read just one
    delay(100);

     // receive the next value
    irrecv.resume();

    if (results.value == 3772790473){
      if (LED_STATUS == 0){
        digitalWrite(LED_PIN, HIGH);
        LED_STATUS = 1;
      }else{
        LED_STATUS = 0;
        digitalWrite(LED_PIN, LOW);
      }
    }
  }
}
