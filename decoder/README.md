GOES-16 CDA Telemetry Decoder
=================

This program receives the **GOES-16 CDA Telemetry** BPSK symbol stream via TCP from the [GNURadio Demodulator](../demodulator) and perform all the decoding necessary to extract the CCSDS Transport Frames. After the decoding process, the Transport Frames will be sent via TCP to [tlmdump](../demux) for further processing required to generate de `.tlm` files.

## Decoding Process

1. Frame Synchronization by CCSDS Word Correlation.
2. Soft to Hard Symbols Conversion.
3. Differential Decoding (NRZ-M).
4. Derandomization.
5. Reed Solomon.
6. Frames sent via TCP to [tlmdump](../demux).

## Installation

```
git clone https://github.com/luigifreitas/tlmdemod.git
mkdir build
cd build
cmake ..
make -j4
./tlmDecoder
```
This project depends on [SatHelper](https://github.com/opensatelliteproject/libsathelper).

## Usage 

Compile the project and run `./tlmDecoder` after initializing the [GNURadio Demodulator](../demodulator).
