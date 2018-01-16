//============================================================================
// Name        : GOES-16 Telemetry Decoder
// Author      : Lucas Teske & Luigi Freitas
// Version     : 1.0
// Copyright   : Copyright 2018
// Description : GOES-16 CDA Telemetry Decoder
//============================================================================

#include <iostream>
#include <memory.h>
#include <SatHelper/sathelper.h>
#include "Display.h"
#include "ChannelWriter.h"
#include "ChannelDispatcher.h"
#include "StatisticsDispatcher.h"
#include "parameters.h"
#include "ExitHandler.h"
#include<iostream>
#include<fstream>

using namespace std;

/*  Hex Word:             0x1ACFFC1D
 *  Binary Word:          0b00011010110011111111110000011101
 *  NRZ-M Word (Normal):  0b00010011011101010101011111101001
 *  NRZ-M Word (Bitflip): 0b11101100100010101010100000010110
 *  NRZ-M Normal Hex:     0x137557E9
 *  NRZ-M Bitflip Hex:    0xEC8AA816
 */

const uint32_t TLM_UW0 = 0x137557E9;
const uint32_t TLM_UW2 = 0xEC8AA816;

bool masterRunning = false;

void setDefaults(SatHelper::ConfigParser &parser) {
    parser[CFG_DISPLAY] = "true";
    parser[CFG_FILE] = "false";
    parser[CFG_DEMOD_PORT] = QUOTE(DEFAULT_DEMODULATOR_PORT);
    parser[CFG_VCHANNEL_PORT] = QUOTE(DEFAULT_VCHANNEL_PORT);
    parser[CFG_STATISTICS_PORT] = QUOTE(DEFAULT_STATISTICS_PORT);
    parser.SaveFile();
}

int main(int argc, char **argv) {
    SatHelper::ConfigParser parser("tlmdecoder.cfg");
    int demodulatorPort = DEFAULT_DEMODULATOR_PORT;
    int vChannelPort = DEFAULT_VCHANNEL_PORT;
    int statisticsPort = DEFAULT_STATISTICS_PORT;

    uint8_t decodedData[FRAMESIZE];
    uint8_t codedData[CODEDFRAMESIZE];

    uint8_t rsCorrectedData[FRAMESIZE];
    uint8_t rsWorkBuffer[255];

    uint64_t droppedPackets = 0;
    uint64_t averageRSCorrections = 0;
    uint64_t frameCount = 0;
    uint64_t lostPackets = 0;
    int64_t lostPacketsPerFrame[256];
    int64_t lastPacketCount[256];
    int64_t receivedPacketsPerFrame[256];
    uint32_t checkTime = 0;
    bool runUi = false;
    bool dump = false;
    bool saveToFile = false;
    Statistics statistics;
    bool isCorrupted;
    bool lastFrameOK = false;
    int flywheelCount = 0;
    int flywheelRecheck = DEFAULT_FLYWHEEL_RECHECK;

    SatHelper::Correlator correlator;
    SatHelper::PacketFixer packetFixer;
    SatHelper::ReedSolomon reedSolomon;
    SatHelper::DeRandomizer deRandomizer;
    ChannelWriter channelWriter("channels");

    Display display;

    std::cout << "GOES-16 CDA Telemetry Decoder - v" << QUOTE(MAJOR_VERSION) << "." << QUOTE(MINOR_VERSION) << "." << QUOTE(MAINT_VERSION) << " -- " << QUOTE(GIT_SHA1) << std::endl;
    std::cout << "  Compilation Date/Time: " << __DATE__ << " - " << __TIME__ << std::endl;
    std::cout << "  SatHelper Version: " << SatHelper::Info::GetVersion() << " - " << SatHelper::Info::GetGitSHA1() << std::endl;
    std::cout << "  SatHelper Compilation Date/Time: " << SatHelper::Info::GetCompilationDate() << " - " << SatHelper::Info::GetCompilationTime() << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    // Load Configuration
    if (!parser.LoadFile()) {
        std::cout << "No config file found. Defaulting to CDA_TLM and creating config file." << std::endl;
        std::cout << "The config file will be created with both \"mode\" and specific parameters. " << std::endl;
        setDefaults(parser);
    }

    if (parser.hasKey(CFG_DISPLAY)) {
        runUi = parser.getBool(CFG_DISPLAY);
    }

    if (parser.hasKey(CFG_DUMP_PACKET)) {
        dump = parser.getBool(CFG_DUMP_PACKET);
    }

    if (parser.hasKey(CFG_FILE)) {
        saveToFile = parser.getBool(CFG_FILE);
    }

    if (parser.hasKey(CFG_DEMOD_PORT)) {
        demodulatorPort = parser.getInt(CFG_DEMOD_PORT);
    }

    if (parser.hasKey(CFG_VCHANNEL_PORT)) {
        vChannelPort = parser.getInt((CFG_VCHANNEL_PORT));
    }

    if (parser.hasKey(CFG_STATISTICS_PORT)) {
        statisticsPort = parser.getInt(CFG_STATISTICS_PORT);
    }

    // Set Defaults

    reedSolomon.SetCopyParityToOutput(true);

    for (int i = 0; i < 256; i++) {
        lostPacketsPerFrame[i] = 0;
        lastPacketCount[i] = -1;
        receivedPacketsPerFrame[i] = -1;
    }

    correlator.addWord(TLM_UW0);
    correlator.addWord(TLM_UW2);

    masterRunning = true;

    // Dispatchers
    ChannelDispatcher channelDispatcher(vChannelPort);
    StatisticsDispatcher statisticsDispatcher(statisticsPort);

    try {
	    channelDispatcher.Start();
    } catch (SatHelperException &e) {
	    std::cerr << "Cannot start channel dispatcher at port " << vChannelPort << ": " << e.reason() << std::endl;
	    masterRunning = false;
    }

    try {
	    statisticsDispatcher.Start();
    } catch (SatHelperException &e) {
	    std::cerr << "Cannot start statistics dispatcher at port " << statisticsPort << ": " << e.reason() << std::endl;
	    masterRunning = false;
    }

    ExitHandler::setCallback([](int signal) {
        std::cout << std::endl << "Got Ctrl + C! Closing..." << std::endl;
        if (masterRunning) {
            masterRunning = false;
        } else {
            exit(1);
        }
    });

    ExitHandler::registerSignal();

    // Socket Init
    SatHelper::TcpServer tcpServer;
    cout << "Starting Demod Receiver at port " << demodulatorPort << std::endl;
    try {
	    tcpServer.Listen(demodulatorPort);
    } catch (SatHelperException &e) {
	    std::cerr << "Cannot listen to port " << demodulatorPort << ": " << e.reason() << std::endl;
	    masterRunning = false;
    }

    // Main Loop

    while (masterRunning) {
        cout << "Waiting for a client connection" << endl;
		SatHelper::TcpSocket client;
		try {
	        client = tcpServer.Accept();
		} catch (SatHelper::SocketAcceptException &e) {
			std::cerr << "Error accepting client: " << e.reason() << std::endl;
			continue;
		}

        cout << "Client connected!" << endl;

        if (runUi) {
            SatHelper::ScreenManager::Clear();
        }

        while (masterRunning) {
            uint32_t chunkSize = CODEDFRAMESIZE;
            try {
                client.WaitForData(CODEDFRAMESIZE, TIMEOUT);
                client.Receive((char *) codedData, chunkSize);

                if (flywheelCount == flywheelRecheck) {
                    lastFrameOK = false;
                    flywheelCount = 0;
                }

                if (!lastFrameOK) {
                    correlator.correlate(codedData, chunkSize);
                } else {
                    // If we got a good lock before, let's just check if the sync is in correct pos.
                    correlator.correlate(codedData, chunkSize / 32);
                    if (correlator.getHighestCorrelationPosition() != 0) {
                        // Oh no, that means something happened :/
                        std::cerr << "Something happened. Pos: " << correlator.getHighestCorrelationPosition() << std::endl;
                        correlator.correlate(codedData, chunkSize);
                        lastFrameOK = false;
                        flywheelCount = 0;
                    }
                }
                flywheelCount++;

                uint32_t word = correlator.getCorrelationWordNumber();
                uint32_t pos = correlator.getHighestCorrelationPosition();
                uint32_t corr = correlator.getHighestCorrelation();
                SatHelper::PhaseShift phaseShift = word == 0 ? SatHelper::PhaseShift::DEG_0 : SatHelper::PhaseShift::DEG_180;
                
                if (corr < MINCORRELATIONBITS) {
                    cerr << "Correlation didn't match criteria of " << MINCORRELATIONBITS << " bits." << endl;
                    continue;
                }

                // Sync Frame
                if (pos != 0) {
                    // Shift position
                    char *shiftedPosition = (char *) codedData + pos;
                    memcpy(codedData, shiftedPosition, CODEDFRAMESIZE - pos); // Copy from p to chunk size to start of codedData
                    chunkSize = pos; // Read needed bytes to fill a frame.
                    checkTime = SatHelper::Tools::getTimestamp();
                    while (client.AvailableData() < chunkSize) {
                        if (SatHelper::Tools::getTimestamp() - checkTime > TIMEOUT) {
                            throw SatHelper::ClientDisconnectedException();
                        }
                    }

                    client.Receive((char *) (codedData + CODEDFRAMESIZE - pos), chunkSize);
                }

                // Convert soft to hard bits. 
                for (int i = 0; i < FRAMEBITS; i += 8) {
                  uint8_t byte = 0x00;
                  for (int j = i; j < i + 8 && j < FRAMEBITS; j++)
                    byte = (byte << 1) | ((codedData[j] < 128) ? 0b00000000 : 0b00000001);
                  decodedData[i/8] = byte;
                }      

                // Differential Decoding (NRZ-M)
                SatHelper::DifferentialEncoding::nrzmDecode(decodedData, FRAMESIZE);
                
                // De-Randomize data
                uint8_t skipsize = (SYNCWORDSIZE / 8);
                memmove(decodedData, decodedData + skipsize, static_cast<size_t>(FRAMESIZE - skipsize));
                deRandomizer.DeRandomize(decodedData, FRAMESIZE - skipsize);

                frameCount++;

                // Reed Solomon Error Correction
                int32_t derrors[1] = { 0 };

                for (uint8_t i = 0; i < RSBLOCKS; i++) {
                    reedSolomon.deinterleave(decodedData, rsWorkBuffer, i, RSBLOCKS);
                    derrors[i] = reedSolomon.decode_ccsds(rsWorkBuffer);
                    reedSolomon.interleave(rsWorkBuffer, rsCorrectedData, i, RSBLOCKS);
                }

                if (derrors[0] == -1) {
                    droppedPackets++;
                    isCorrupted = true;
                    lastFrameOK = false;
                } else {
                    averageRSCorrections += derrors[0] != -1 ? derrors[0] : 0;
                    isCorrupted = false;
                    lastFrameOK = true;
                }

                // Packet Header Filtering
                uint8_t scid = static_cast<uint8_t>(((*rsCorrectedData) & 0x3F) << 2 | (*(rsCorrectedData + 1) & 0xC0) >> 6);
                uint8_t vcid = static_cast<uint8_t>((*(rsCorrectedData + 1)) & 0x3F);
                
                //cout << "RS: " << derrors[0] << " | SCID: " << int(scid) << " | VCID: " << int(vcid) << endl;

                // Packet Counter from Packet
                uint32_t counter = *((uint32_t *) (rsCorrectedData + 2));
                counter = SatHelper::Tools::swapEndianess(counter);
                counter &= 0xFFFFFF00;
                counter = counter >> 8;

                uint8_t phaseCorr = static_cast<uint8_t>(phaseShift == SatHelper::PhaseShift::DEG_180 ? 180 : 0);
                uint8_t partialRSCorrections = (uint8_t) (averageRSCorrections / frameCount);

                if (!isCorrupted) {
                    channelDispatcher.add((char *) rsCorrectedData, FRAMESIZE - RSPARITYBLOCK - (SYNCWORDSIZE / 8));
                    
                    if (saveToFile) {
                        ofstream out("./telemetry.raw", ios::out | ios::app | ios::binary);
                        out.write((char*)rsCorrectedData, (FRAMESIZE - RSPARITYBLOCK - (SYNCWORDSIZE / 8))*sizeof(uint8_t)); 
                    }

                    if (lastPacketCount[vcid] + 1 != counter && lastPacketCount[vcid] > -1) {
                        int lostCount = (int)(counter - lastPacketCount[vcid] - 1);
                        lostPackets += lostCount;
                        lostPacketsPerFrame[vcid] += lostCount;
                    }

                    lastPacketCount[vcid] = counter;
                    receivedPacketsPerFrame[vcid] = receivedPacketsPerFrame[vcid] == -1 ? 1 : receivedPacketsPerFrame[vcid] + 1;

                    statistics.update(scid, vcid, (uint64_t) counter, FRAMEBITS, derrors, static_cast<uint8_t>(corr), phaseCorr,
                            lostPackets, partialRSCorrections, droppedPackets, receivedPacketsPerFrame, lostPacketsPerFrame, frameCount, true);

                    if (runUi) {
                        display.update(scid, vcid, (uint64_t) counter, FRAMEBITS, derrors, static_cast<uint8_t>(corr), phaseCorr,
                                lostPackets, partialRSCorrections, droppedPackets, receivedPacketsPerFrame, lostPacketsPerFrame, frameCount, true);

                        display.show();
                    }
                } else {
                    statistics.update(0, 0, 0, FRAMEBITS, derrors, static_cast<uint8_t>(corr), 0, lostPackets,
                            partialRSCorrections, droppedPackets, receivedPacketsPerFrame, lostPacketsPerFrame, frameCount, false);

                    if (runUi) {
                        display.update(0, 0, 0, FRAMEBITS, derrors, static_cast<uint8_t>(corr), phaseCorr, lostPackets,
                        partialRSCorrections, droppedPackets, receivedPacketsPerFrame, lostPacketsPerFrame, frameCount, false);

                        display.show();
                    }
                }

                statisticsDispatcher.Update(statistics);
                statisticsDispatcher.Work();
            } catch (SatHelper::SocketException &e) {
                cerr << endl;
                cerr << "Client disconnected" << endl;
                cerr << "   " << e.what() << endl;
                break;
            }
        }

        client.Close();
    }

	statisticsDispatcher.Stop();
    channelDispatcher.Stop();
    std::cout << "Closing main server" << std::endl;
    tcpServer.Close();
    std::cout << "Main server closed. " << std::endl;
    return 0;
}
