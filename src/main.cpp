#include <Arduino.h>
#include "Mesh.h"

Mesh mesh = Mesh(3, 0);

// Get random value for device ID (will change on each reboot)
uint8_t device_id = random(0, 255);

void setup() {
    Serial.begin(115200);
    Serial.print("Device random ID: ");
    Serial.println(device_id);

    Serial.println("Ready.");
}

void dumpMessage(const MeshMessage &message) {
    Serial.print("Message: id=");
    Serial.print(message.id, HEX);
    Serial.print("; data=");
    Serial.print(message.data[0], HEX);
    Serial.print(" ");
    Serial.print(message.data[1], HEX);
    Serial.print(" ");
    Serial.println(message.data[2], HEX);
}

void loop() {
    static MeshMessage message;
    static unsigned long nextSendTime = random(0, 8000);
    static uint8_t id = 0;

    if (mesh.receive(&message) == Mesh::RESULT_OK) {
        dumpMessage(message);
        id = message.id + 1;
    }

    if (millis() >= nextSendTime) {
        nextSendTime = millis() + random(1000, 8000);

        message.id = id++;
        message.data[0] = device_id;
        message.data[1] = 0x33;
        message.data[2] = 0x45;
        mesh.send(&message);

        Serial.print("Message sent with id: ");
        Serial.println(message.id);
    }
}