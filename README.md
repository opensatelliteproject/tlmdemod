GOES-16 CDA Telemetry
=========================================

These three programs demodulate, decode and demux the **GOES-16 CDA Telemetry Signal** generating `.tlm` files that can be further processed by the user. The final products of this signal aren't fully known yet. Probably they consist of satellite metadata like temperature, voltage, firmware version, equipment status and various analog readings. 

This project currently supports the 40 Kilosymbol CDA Telemetry Signal coming from GOES-16. In the future it will *probably* support all Thrid Generation NOAA's GOES Satellites (GOES-R, GOES-S, GOES-T, GOES-U). 

Learn more: [Reverse Engineering GOES-16 CDA Telemetry Signal]().

* [Demodulator](demodulator/)
* [Decoder](decoder/)
* [Demuxer](demuxer/)
