/*
 * Display.cpp
 *
 *  Created on: 07/11/2016
 *      Author: Lucas Teske
 *      Edited: Luigi Freitas
 */

#include "Display.h"
#include <iomanip>

Display::Display() {
    this->scid = 0;
    this->vcid = 0;
    this->packetNumber = 0;
    this->frameBits = 0;
    this->rsErrors = 0;
    this->syncCorrelation = 0;
    this->phaseCorrection = 0;
    this->lostPackets = 0;
    this->averageRSCorrections = 0;
    this->droppedPackets = 0;

    for (int i = 0; i < 256; i++) {
        this->receivedPacketsPerChannel[i] = 0;
        this->lostPacketsPerChannel[i] = 0;
    }

    this->totalPackets = 0;
    this->startTime = SatHelper::Tools::getTimestamp();
    this->lock = false;
}

Display::~Display() {
    // TODO Auto-generated destructor stub
}

void Display::show() {
    int runningTime = SatHelper::Tools::getTimestamp() - startTime;
    screenManager.GotoXY(0, 0);
    std::cout << "┌─────────────────────────────────────────────────────────────────────────────┐" << std::endl;
    std::cout << "|                GOES CDA TLM DECODER - Open Satellite Project                |" << std::endl;
    std::cout << "|─────────────────────────────────────────────────────────────────────────────|" << std::endl;
    std::cout << "|         Current Frame Data           |               Statistics             |" << std::endl;
    std::cout << "|──────────────────────────────────────|──────────────────────────────────────|" << std::endl;
    std::cout << "| SC ID:                           " << std::setw(3) << (uint32_t) scid << " |  Total Lost Packets:    " << std::setw(10) << lostPackets
            << "   |" << std::endl;
    std::cout << "| VC ID:                           " << std::setw(3) << (uint32_t) vcid << " |  Average RS Correction:         " << std::setw(2) << (uint32_t) averageRSCorrections<< "   |" << std::endl;
    std::cout << "| Packet Number:            " << std::setw(10) << packetNumber << " |  Total Packets:         " << std::setw(10) << totalPackets << "   |" << std::endl;
    std::cout << "| Total Dropped Packets:    " << std::setw(10) << droppedPackets << " |" << std::setw(39) << "   |" << std::endl;
    std::cout << "| Frame Lock:                    " << std::setw(5) << (lock ? "TRUE" : "FALSE") << " |" << std::setw(39) << "   |"  << std::endl;

    // Reset cout
    std::cout << std::dec << std::setfill(' ');

    std::cout << "| RS Errors:                        " << std::setw(2) << rsErrors <<  " |──────────────────────────────────────|" << std::endl;
    std::cout << "| Sync Correlation:                 " << std::setw(2) << (uint32_t) syncCorrelation << " |             Channel Data             |"
            << std::endl;
    std::cout << "| Phase Correction:                " << std::setw(3) << (uint32_t) phaseCorrection << " |──────────────────────────────────────|"
            << std::endl;
    std::cout << "| Running Time:             " << std::setw(10) << runningTime << " |  Chan  |   Received   |     Lost     |" << std::endl;

    int maxChannels = 8;
    int printedChannels = 0;

    for (int i = 0; i < 255; i++) {
        if (receivedPacketsPerChannel[i] != -1) {
            std::cout << "|                                      |   " << std::setw(2) << i << "   |  " << std::setw(10) << receivedPacketsPerChannel[i]
                    << "  |  " << std::setw(10) << lostPacketsPerChannel[i] << "  |" << std::endl;
            printedChannels++;
            if (printedChannels == maxChannels) {
                break;
            }
        }
    }

    for (int i = 0; i < maxChannels - printedChannels; i++) {
        std::cout << "|                                      |        |              |              |" << std::endl;
    }

    std::cout << "└─────────────────────────────────────────────────────────────────────────────┘";
}

void Display::update(uint8_t scid, uint8_t vcid, uint64_t packetNumber, uint16_t frameBits, int32_t *rsErrors,
        uint8_t syncCorrelation, uint8_t phaseCorrection, uint64_t lostPackets, uint8_t averageRSCorrections,
        uint64_t droppedPackets, int64_t *receivedPacketsPerChannel, int64_t *lostPacketsPerChannel, uint64_t totalPackets, bool lock) {

    this->scid = scid;
    this->vcid = vcid;
    this->packetNumber = packetNumber;
    this->frameBits = frameBits;
    this->rsErrors = rsErrors[0];
    this->syncCorrelation = syncCorrelation;
    this->phaseCorrection = phaseCorrection;
    this->lostPackets = lostPackets;
    this->averageRSCorrections = averageRSCorrections;
    this->droppedPackets = droppedPackets;

    for (int i = 0; i < 256; i++) {
        this->receivedPacketsPerChannel[i] = receivedPacketsPerChannel[i];
        this->lostPacketsPerChannel[i] = lostPacketsPerChannel[i];
    }

    this->totalPackets = totalPackets;
    this->lock = lock;
}
