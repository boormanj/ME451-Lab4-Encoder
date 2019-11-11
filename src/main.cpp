#include <Arduino.h>

#define channelA 2
#define channelB 3
#define baudRate 57600

const int ticksPerRevolution = 480;
const float wheelDiam = 9.0F / PI;
const float wheelRad = wheelDiam / 2.0F;

volatile byte stateA;
volatile byte stateB;
long ticks;
long rotation;
int remainder;

float previousDistance;
float currentDistance;
float previousTimeSec;
float currentTimeSec;
float degree;
float velocity;

void encoderInterrupt() {
    stateA = !stateA;
    stateB = digitalRead(channelB);

    if (stateB == LOW) {
        ticks++;
    } else if (stateB == HIGH) {
        ticks--;
    }
}

void setup() {
    pinMode(channelA, INPUT);
    attachInterrupt(digitalPinToInterrupt(channelA), encoderInterrupt, RISING);

    Serial.begin(baudRate);
    Serial.print("ticks,angle,distance,velocity");
}

void loop() {
    if ((ticks > -480) && (ticks < 480)) {
        remainder = ticks;
    } else {
        remainder = ticks % ticksPerRevolution;
    }

    
    if (remainder <= 240) {
        degree = (180 / 240) * remainder;
    } else {
        degree = ((180 / 240) * remainder) - 360;
    }

    currentDistance = (float)ticks * (9.0F / 480.0F);
    currentTimeSec = (float)millis() / 1000.0F;

    velocity = (currentDistance - previousDistance) / (currentTimeSec - previousTimeSec);

    Serial.print(ticks);
    Serial.print(",");
    Serial.print(degree);
    Serial.print(",");
    Serial.print(currentDistance);
    Serial.print(",");
    Serial.println(velocity);

    previousTimeSec = currentTimeSec;
    previousDistance = currentDistance;

    delay(1);
}


