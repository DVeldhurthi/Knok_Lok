#define SOUND_SENSOR_PIN 36
#define THRESHOLD 1000
#define MAX_KNOCKS 10
#define TIMEOUT 3000
#define MIN_RATIO 0.6
#define MAX_RATIO 1.4
#define LED_PIN 13

unsigned long knockTimes[MAX_KNOCKS];  
int knockCount = 0;  
unsigned long lastKnockTime = 0;

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    pinMode(SOUND_SENSOR_PIN, INPUT);
}

void loop() {
    int soundLevel = analogRead(SOUND_SENSOR_PIN);

    if (soundLevel > THRESHOLD) {
        unsigned long now = millis();

        if (knockCount == 0 || (now - lastKnockTime) > 50) {
            Serial.println("Knock detected!");
            knockTimes[knockCount++] = now;
            lastKnockTime = now;
        }

        delay(50);
    }

    if (knockCount > 0 && millis() - lastKnockTime > TIMEOUT) {
        comparePattern();
        knockCount = 0;
    }
}

void comparePattern() {
    if (knockCount < 2) {
        Serial.println("Not enough knocks to compare.");
        return;
    }

    unsigned long timeDifferences[MAX_KNOCKS - 1];
    for (int i = 0; i < knockCount - 1; i++) {
        timeDifferences[i] = knockTimes[i + 1] - knockTimes[i];
    }

    bool patternAccepted = true;
    for (int i = 0; i < knockCount - 2; i++) {
        float ratio = (float)timeDifferences[i + 1] / (float)timeDifferences[i];
        if (ratio < MIN_RATIO || ratio > MAX_RATIO) {
            patternAccepted = false;
            break;
        }
    }

    if (patternAccepted) {
        Serial.println("Pattern ACCEPTED!");
        digitalWrite(LED_PIN, HIGH);
        delay(0.7*1000);
        digitalWrite(LED_PIN, LOW);
        delay(100);

    } else {
        Serial.println("Pattern differs!");
        digitalWrite(LED_PIN, HIGH);
        delay(1000/3);
        digitalWrite(LED_PIN, LOW);
        delay(1000/3);
        digitalWrite(LED_PIN, HIGH);
        delay(1000/3);
        digitalWrite(LED_PIN, LOW);
        delay(100);
    }
}