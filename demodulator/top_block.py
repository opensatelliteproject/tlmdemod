#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Wed Jan 17 23:44:30 2018
##################################################

from gnuradio import analog
from gnuradio import blocks
from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import filter
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from grc_gnuradio import blks2 as grc_blks2
from optparse import OptionParser


class top_block(gr.top_block):

    def __init__(self):
        gr.top_block.__init__(self, "Top Block")

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 2.5e+06
        self.decim = decim = 3
        self.baud = baud = 40000
        self.samp_per_sym = samp_per_sym = float(samp_rate/decim)/baud
        self.clock_alpha = clock_alpha = 0.0037

        ##################################################
        # Blocks
        ##################################################
        self.freq_xlating_fir_filter_xxx_0 = filter.freq_xlating_fir_filter_ccc(decim, (firdes.complex_band_pass(1, samp_rate, -samp_rate/(2*decim), samp_rate/(2*decim), 10000)), 1.4e6, samp_rate)
        self.digital_pfb_clock_sync_xxx_0_0 = digital.pfb_clock_sync_ccf(samp_per_sym, 0.1, (firdes.root_raised_cosine(32, 32, 1.0/float(samp_per_sym), 0.35, 11*int(samp_per_sym)*32)), 32, 16, 0.1, 1)
        self.digital_costas_loop_cc_0_0 = digital.costas_loop_cc(50e-3, 4, False)
        self.blocks_wavfile_source_0 = blocks.wavfile_source('/Volumes/My Passport/Basebands/Satellites/GOES-16/gqrx_20170124_060211_1694100000_2500000_fc.wav', False)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        self.blocks_stream_to_vector_0 = blocks.stream_to_vector(gr.sizeof_char*1, 16)
        self.blocks_float_to_complex_0 = blocks.float_to_complex(1)
        self.blocks_float_to_char_0 = blocks.float_to_char(1, 127)
        self.blocks_complex_to_real_0 = blocks.complex_to_real(1)
        self.blks2_tcp_sink_0 = grc_blks2.tcp_sink(
        	itemsize=gr.sizeof_char*16,
        	addr='127.0.0.1',
        	port=5000,
        	server=False,
        )
        self.analog_feedforward_agc_cc_0_0 = analog.feedforward_agc_cc(1024, 2.0)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.analog_feedforward_agc_cc_0_0, 0), (self.digital_pfb_clock_sync_xxx_0_0, 0))
        self.connect((self.blocks_complex_to_real_0, 0), (self.blocks_float_to_char_0, 0))
        self.connect((self.blocks_float_to_char_0, 0), (self.blocks_stream_to_vector_0, 0))
        self.connect((self.blocks_float_to_complex_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.blocks_stream_to_vector_0, 0), (self.blks2_tcp_sink_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.freq_xlating_fir_filter_xxx_0, 0))
        self.connect((self.blocks_wavfile_source_0, 0), (self.blocks_float_to_complex_0, 0))
        self.connect((self.blocks_wavfile_source_0, 1), (self.blocks_float_to_complex_0, 1))
        self.connect((self.digital_costas_loop_cc_0_0, 0), (self.blocks_complex_to_real_0, 0))
        self.connect((self.digital_pfb_clock_sync_xxx_0_0, 0), (self.digital_costas_loop_cc_0_0, 0))
        self.connect((self.freq_xlating_fir_filter_xxx_0, 0), (self.analog_feedforward_agc_cc_0_0, 0))

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.set_samp_per_sym(float(self.samp_rate/self.decim)/self.baud)
        self.freq_xlating_fir_filter_xxx_0.set_taps((firdes.complex_band_pass(1, self.samp_rate, -self.samp_rate/(2*self.decim), self.samp_rate/(2*self.decim), 10000)))
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)

    def get_decim(self):
        return self.decim

    def set_decim(self, decim):
        self.decim = decim
        self.set_samp_per_sym(float(self.samp_rate/self.decim)/self.baud)
        self.freq_xlating_fir_filter_xxx_0.set_taps((firdes.complex_band_pass(1, self.samp_rate, -self.samp_rate/(2*self.decim), self.samp_rate/(2*self.decim), 10000)))

    def get_baud(self):
        return self.baud

    def set_baud(self, baud):
        self.baud = baud
        self.set_samp_per_sym(float(self.samp_rate/self.decim)/self.baud)

    def get_samp_per_sym(self):
        return self.samp_per_sym

    def set_samp_per_sym(self, samp_per_sym):
        self.samp_per_sym = samp_per_sym
        self.digital_pfb_clock_sync_xxx_0_0.update_taps((firdes.root_raised_cosine(32, 32, 1.0/float(self.samp_per_sym), 0.35, 11*int(self.samp_per_sym)*32)))

    def get_clock_alpha(self):
        return self.clock_alpha

    def set_clock_alpha(self, clock_alpha):
        self.clock_alpha = clock_alpha


def main(top_block_cls=top_block, options=None):

    tb = top_block_cls()
    tb.start()
    try:
        raw_input('Press Enter to quit: ')
    except EOFError:
        pass
    tb.stop()
    tb.wait()


if __name__ == '__main__':
    main()
