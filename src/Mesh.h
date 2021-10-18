//
// Created by samuel on 24-05-21.
//

#ifndef RFMESHNETWORK_MESH_H
#define RFMESHNETWORK_MESH_H


#include "MeshMessage.h"
#include "../lib/rc-switch-master/RCSwitch.h"

#define MAX_RECEIVED_IDS 64

#define MESH_TRANSMIT 1
#define MESH_RECEIVE 2
#define MESH_TRANSMIT_AND_RECEIVE 3

class Mesh {
public:
    enum RESULT {
        RESULT_OK = 0,
        RESULT_ERROR = 1,
        RESULT_EMPTY = 2,
    };

    Mesh(uint8_t txPin, uint8_t rxPin, uint8_t mode);

    void send(const MeshMessage *message);

    RESULT receive(MeshMessage *message);

    void forward(const MeshMessage *message);

    uint8_t mode = MESH_TRANSMIT_AND_RECEIVE;

private:
    RCSwitch rcSwitch = RCSwitch();

    void markAsReceived(const MeshMessage *message);

    bool isIdReceived(uint8_t id);

    uint8_t received_ids[MAX_RECEIVED_IDS] = {0};
    uint8_t received_ids_pointer = 0;
};


#endif //RFMESHNETWORK_MESH_H
