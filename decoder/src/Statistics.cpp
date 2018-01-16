/*
 * Statistics.cpp
 *
 *  Created on: 01/12/2016
 *      Author: Lucas Teske
 */

#include "Statistics.h"
#include <SatHelper/sathelper.h>
#include <cstring>
#include <chrono>

Statistics::Statistics() {
    this->data.scid = 0;
    this->data.vcid = 0;
    this->data.packetNumber = 0;
    this->data.frameBits = 0;
    this->data.rsErrors = 0;
    this->data.syncCorrelation = 0;
    this->data.phaseCorrection = 0;
    this->data.lostPackets = 0;
    this->data.averageRSCorrections = 0;
    this->data.droppedPackets = 0;

    for (int i = 0; i < 256; i++) {
        this->data.receivedPacketsPerChannel[i] = 0;
        this->data.lostPacketsPerChannel[i] = 0;
    }

    this->data.totalPackets = 0;
    this->data.startTime = SatHelper::Tools::getTimestamp();
    this->data.frameLock = 0;
}

Statistics::~Statistics() {

}

void Statistics::update(const Statistics &statistics) {
    std::memcpy(&this->data, &statistics.data, sizeof(Statistics_st));
}

void Statistics::update(uint8_t scid, uint8_t vcid, uint64_t packetNumber, uint16_t frameBits, int32_t *rsErrors,
        uint8_t syncCorrelation, uint8_t phaseCorrection, uint64_t lostPackets, uint8_t averageRSCorrections,
        uint64_t droppedPackets, int64_t *receivedPacketsPerChannel, int64_t *lostPacketsPerChannel, uint64_t totalPackets, bool frameLock) {

    this->data.scid = scid;
    this->data.vcid = vcid;
    this->data.packetNumber = packetNumber;
    this->data.frameBits = frameBits;
    this->data.rsErrors = rsErrors[0];
    this->data.syncCorrelation = syncCorrelation;
    this->data.phaseCorrection = phaseCorrection;
    this->data.lostPackets = lostPackets;
    this->data.averageRSCorrections = averageRSCorrections;
    this->data.droppedPackets = droppedPackets;

    for (int i = 0; i < 256; i++) {
        this->data.receivedPacketsPerChannel[i] = receivedPacketsPerChannel[i];
        this->data.lostPacketsPerChannel[i] = lostPacketsPerChannel[i];
    }

    this->data.totalPackets = totalPackets;
    this->data.frameLock = (uint8_t) frameLock;
}
