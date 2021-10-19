#include <Arduino.h>
#include "Mesh.h"

#define LED_BUILTIN 2
Mesh mesh = Mesh(13, 12, MESH_TRANSMIT_AND_RECEIVE);

// Get random value for device ID (will change on each reboot)
uint8_t device_id = random(0, 255);

void pulseLight() {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(80);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
}

void dumpMessage(const MeshMessage &message) {
    Serial.print("Message: id=");
    Serial.print(message.id, DEC);
    Serial.print("; data=");
    Serial.print(message.data[0], HEX);
    Serial.print(" ");
    Serial.print(message.data[1], HEX);
    Serial.print(" ");
    Serial.println(message.data[2], HEX);
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
    Serial.print("Device random ID: ");
    Serial.println(device_id);

    Serial.println("Ready.");
    pulseLight();
    pulseLight();
    pulseLight();
}

void loop() {
    static MeshMessage message;
//    static unsigned long nextSendTime = random(0, 8000);
    static unsigned long nextSendTime = 3000;
    static uint8_t id = 0;

    if (mesh.mode & MESH_RECEIVE  && mesh.receive(&message) == Mesh::RESULT_OK) {
        pulseLight();
        pulseLight();
        dumpMessage(message);
        id = message.id + 1;
    }

    if (mesh.mode & MESH_TRANSMIT && millis() >= nextSendTime) {
        Serial.print("Sending message... ");

        message.id = id++;
        message.data[0] = device_id;
        message.data[1] = 0x33;
        message.data[2] = 0x45;
        mesh.send(&message);

        pulseLight();
        Serial.print("Message sent with id: ");
        Serial.println(message.id);

//        nextSendTime = millis() + random(1000, 8000);
        nextSendTime = millis() + 3000;
    }
}