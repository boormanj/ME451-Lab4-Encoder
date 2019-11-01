#include <Arduino.h>
#include <Math.h>

#define channelA 2
#define channelB 3
#define baudRate 9600

const float ticksPerRevolution = 480.0F;
const float wheelDiam = 9.0F / M_PI;
const float wheelRad = wheelDiam / 2.0F;

volatile byte stateA;
volatile byte stateB;
long ticks;
long rotation;

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
    rotation = abs((int)(ticks / ticksPerRevolution));
    if (rotation == 0) {
        rotation = 1;
    }

    degree = ((float)ticks / (480.0F / 360.0F));
    currentDistance = (float)ticks * (9.0F / 480.0F);
    currentTimeSec = (float)millis() / 1000.0F;

    velocity = (currentDistance - previousDistance) / (currentTimeSec - previousTimeSec);

    if (degree > 180) {
        degree -= (rotation * 360);
    } else if (degree < -180) {
        degree += (rotation * 360);
    }

    Serial.print(ticks);
    Serial.print(",");
    Serial.print(degree);
    Serial.print(",");
    Serial.print(currentDistance);
    Serial.print(",");
    Serial.println(velocity);

    previousTimeSec = currentTimeSec;
    previousDistance = currentDistance;

    delay(5);
}


