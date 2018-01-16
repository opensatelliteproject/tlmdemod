#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Tue Jan 16 20:27:57 2018
##################################################

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

from PyQt4 import Qt
from gnuradio import analog
from gnuradio import blocks
from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import filter
from gnuradio import gr
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from grc_gnuradio import blks2 as grc_blks2
from optparse import OptionParser
import sip
import sys
from gnuradio import qtgui


class top_block(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Top Block")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Top Block")
        qtgui.util.check_set_qss()
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())

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
        self.qtgui_freq_sink_x_0 = qtgui.freq_sink_c(
        	1024, #size
        	firdes.WIN_BLACKMAN_hARRIS, #wintype
        	0, #fc
        	samp_rate, #bw
        	"", #name
        	1 #number of inputs
        )
        self.qtgui_freq_sink_x_0.set_update_time(0.10)
        self.qtgui_freq_sink_x_0.set_y_axis(-140, 10)
        self.qtgui_freq_sink_x_0.set_y_label('Relative Gain', 'dB')
        self.qtgui_freq_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, 0.0, 0, "")
        self.qtgui_freq_sink_x_0.enable_autoscale(False)
        self.qtgui_freq_sink_x_0.enable_grid(True)
        self.qtgui_freq_sink_x_0.set_fft_average(0.1)
        self.qtgui_freq_sink_x_0.enable_axis_labels(True)
        self.qtgui_freq_sink_x_0.enable_control_panel(False)

        if not False:
          self.qtgui_freq_sink_x_0.disable_legend()

        if "complex" == "float" or "complex" == "msg_float":
          self.qtgui_freq_sink_x_0.set_plot_pos_half(not True)

        labels = ['', '', '', '', '',
                  '', '', '', '', '']
        widths = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
                  "magenta", "yellow", "dark red", "dark green", "dark blue"]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_freq_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_freq_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_freq_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_freq_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_freq_sink_x_0.set_line_alpha(i, alphas[i])

        self._qtgui_freq_sink_x_0_win = sip.wrapinstance(self.qtgui_freq_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_freq_sink_x_0_win)
        self.qtgui_const_sink_x_0_0 = qtgui.const_sink_c(
        	1024, #size
        	"GOES East TLM", #name
        	1 #number of inputs
        )
        self.qtgui_const_sink_x_0_0.set_update_time(0.10)
        self.qtgui_const_sink_x_0_0.set_y_axis(-2, 2)
        self.qtgui_const_sink_x_0_0.set_x_axis(-2, 2)
        self.qtgui_const_sink_x_0_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, "")
        self.qtgui_const_sink_x_0_0.enable_autoscale(False)
        self.qtgui_const_sink_x_0_0.enable_grid(False)
        self.qtgui_const_sink_x_0_0.enable_axis_labels(True)

        if not False:
          self.qtgui_const_sink_x_0_0.disable_legend()

        labels = ['', '', '', '', '',
                  '', '', '', '', '']
        widths = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "red", "red", "red",
                  "red", "red", "red", "red", "red"]
        styles = [0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0]
        markers = [0, 0, 0, 0, 0,
                   0, 0, 0, 0, 0]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_const_sink_x_0_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_const_sink_x_0_0.set_line_label(i, labels[i])
            self.qtgui_const_sink_x_0_0.set_line_width(i, widths[i])
            self.qtgui_const_sink_x_0_0.set_line_color(i, colors[i])
            self.qtgui_const_sink_x_0_0.set_line_style(i, styles[i])
            self.qtgui_const_sink_x_0_0.set_line_marker(i, markers[i])
            self.qtgui_const_sink_x_0_0.set_line_alpha(i, alphas[i])

        self._qtgui_const_sink_x_0_0_win = sip.wrapinstance(self.qtgui_const_sink_x_0_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_const_sink_x_0_0_win)
        self.freq_xlating_fir_filter_xxx_0 = filter.freq_xlating_fir_filter_ccc(decim, (firdes.complex_band_pass(1, samp_rate, -samp_rate/(2*decim), samp_rate/(2*decim), 10000)), 1.4e6, samp_rate)
        self.digital_pfb_clock_sync_xxx_0_0 = digital.pfb_clock_sync_ccf(samp_per_sym, 0.1, (firdes.root_raised_cosine(32, 32, 1.0/float(samp_per_sym), 0.35, 11*int(samp_per_sym)*32)), 32, 16, 0.1, 1)
        self.digital_costas_loop_cc_0_0 = digital.costas_loop_cc(50e-3, 2, False)
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
        self.connect((self.digital_costas_loop_cc_0_0, 0), (self.qtgui_const_sink_x_0_0, 0))
        self.connect((self.digital_pfb_clock_sync_xxx_0_0, 0), (self.digital_costas_loop_cc_0_0, 0))
        self.connect((self.freq_xlating_fir_filter_xxx_0, 0), (self.analog_feedforward_agc_cc_0_0, 0))
        self.connect((self.freq_xlating_fir_filter_xxx_0, 0), (self.qtgui_freq_sink_x_0, 0))

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.set_samp_per_sym(float(self.samp_rate/self.decim)/self.baud)
        self.qtgui_freq_sink_x_0.set_frequency_range(0, self.samp_rate)
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

    from distutils.version import StrictVersion
    if StrictVersion(Qt.qVersion()) >= StrictVersion("4.5.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()
    tb.start()
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()
    qapp.connect(qapp, Qt.SIGNAL("aboutToQuit()"), quitting)
    qapp.exec_()


if __name__ == '__main__':
    main()
