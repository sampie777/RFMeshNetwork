//
// Created by samuel on 24-05-21.
//

#include "Mesh.h"

Mesh::Mesh(uint8_t txPin, uint8_t rxPin, uint8_t mode) {
    this->mode = mode;

    if (mode & MESH_TRANSMIT) {
        rcSwitch.enableTransmit(txPin);
    }
    if (mode & MESH_RECEIVE) {
        rcSwitch.enableReceive(rxPin);
    }
}

void Mesh::send(const MeshMessage *message) {
    if (!(mode & MESH_TRANSMIT)) {
        return;
    }

    char bytes[sizeof(struct MeshMessage)];
    memcpy(bytes, message, sizeof(bytes));

    markAsReceived(message);

    rcSwitch.send(bytes);
}

Mesh::RESULT Mesh::receive(MeshMessage *message) {
    if (!(mode & MESH_RECEIVE)) {
        return Mesh::RESULT_EMPTY;
    }

    if (!rcSwitch.available()) {
        return Mesh::RESULT_EMPTY;
    }

    // Retrieve message data
    uint32_t data = rcSwitch.getReceivedValue();
    unsigned int receivedBitlength = rcSwitch.getReceivedBitlength();
    rcSwitch.resetAvailable();

    // Check for correct bit size
    if (receivedBitlength != 8 * sizeof(struct MeshMessage)) {
        // Debug output
        Serial.print("Message has invalid bit length: ");
        Serial.println(receivedBitlength);

        return Mesh::RESULT_ERROR;
    }

    // Check if message was already received before
    uint8_t id = data >> 24;
    if (isIdReceived(id)) {
        // Debug output
        Serial.print("Ignoring message with id: ");
        Serial.println(id);

        return Mesh::RESULT_EMPTY;
    }

    // Store message
    message->id = id;
    message->data[0] = data >> 16;
    message->data[1] = data >> 8;
    message->data[2] = data;

    forward(message);

    return Mesh::RESULT_OK;
}

void Mesh::forward(const MeshMessage *message) {
    send(message);
}

void Mesh::markAsReceived(const MeshMessage *message) {
    if (isIdReceived(message->id)) {
        return;
    }

    received_ids[received_ids_pointer++] = message->id;

    // Rolling cache...
    if (received_ids_pointer >= MAX_RECEIVED_IDS) {
        received_ids_pointer = 0;
    }
}

bool Mesh::isIdReceived(uint8_t id) {
    for (uint8_t received_id : received_ids) {
        if (received_id == id) {
            return true;
        }
    }
    return false;
}