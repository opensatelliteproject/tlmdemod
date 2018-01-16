/*
 * Display.h
 *
 *  Created on: 07/11/2016
 *      Author: Lucas Teske
 *      Edited: Luigi Freitas
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <cstdint>
#include <SatHelper/sathelper.h>

class Display {
private:
    SatHelper::ScreenManager screenManager;
    uint8_t scid;
    uint8_t vcid;
    uint64_t packetNumber;
    uint16_t frameBits;
    int32_t rsErrors;
    uint8_t syncCorrelation;
    uint8_t phaseCorrection;
    uint64_t lostPackets;
    uint8_t averageRSCorrections;
    uint64_t droppedPackets;
    int64_t receivedPacketsPerChannel[256];
    int64_t lostPacketsPerChannel[256];
    uint64_t totalPackets;
    uint32_t startTime;
    bool lock;

public:
    Display();
    virtual ~Display();

    inline void start() {
        this->startTime = SatHelper::Tools::getTimestamp();
    }
    void show();

    void update(uint8_t scid, uint8_t vcid, uint64_t packetNumber, uint16_t frameBits, int32_t *rsErrors,
            uint8_t syncCorrelation, uint8_t phaseCorrection, uint64_t lostPackets, uint8_t averageRSCorrections,
            uint64_t droppedPackets, int64_t *receivedPacketsPerChannel, int64_t *lostPacketsPerChannel, uint64_t totalPackets, bool lock);
};

#endif /* DISPLAY_H_ */
