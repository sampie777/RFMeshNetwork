//
// Created by samuel on 24-05-21.
//

#ifndef RFMESHNETWORK_MESHMESSAGE_H
#define RFMESHNETWORK_MESHMESSAGE_H

#include <Arduino.h>

struct MeshMessage {
    uint8_t id;
//    uint16_t timeToLive;
//    uint8_t length;
    uint8_t data[3];
};


#endif //RFMESHNETWORK_MESHMESSAGE_H
