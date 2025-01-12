// Written by Adrian Musceac YO8RZZ , started July 2021.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef GR_DEMOD_MMDVM_MULTI_H
#define GR_DEMOD_MMDVM_MULTI_H

#include <gnuradio/hier_block2.h>
#include <gnuradio/filter/firdes.h>
#include <gnuradio/filter/rational_resampler_base.h>
#include <gnuradio/filter/rational_resampler_base.h>
#include <gnuradio/analog/quadrature_demod_cf.h>
#include <gnuradio/analog/pwr_squelch_ff.h>
#include <gnuradio/filter/fft_filter_ccf.h>
#include <gnuradio/blocks/float_to_short.h>
#include <gnuradio/blocks/multiply_const.h>
#include <gnuradio/blocks/rotator_cc.h>
#include "src/bursttimer.h"
#include "gr_mmdvm_sink.h"

class gr_demod_mmdvm_multi;

typedef boost::shared_ptr<gr_demod_mmdvm_multi> gr_demod_mmdvm_multi_sptr;
gr_demod_mmdvm_multi_sptr make_gr_demod_mmdvm_multi(BurstTimer *burst_timer, int sps=125, int samp_rate=1000000, int carrier_freq=1700,
                                          int filter_width=6250);

class gr_demod_mmdvm_multi : public gr::hier_block2
{
public:
    explicit gr_demod_mmdvm_multi(BurstTimer *burst_timer, int sps=125, int samp_rate=1000000, int carrier_freq=1600,
                               int filter_width=6250);

    void set_filter_width(int filter_width);


private:
    gr::blocks::float_to_short::sptr _float_to_short1;
    gr::blocks::float_to_short::sptr _float_to_short2;
    gr::blocks::float_to_short::sptr _float_to_short3;
    gr::analog::quadrature_demod_cf::sptr _fm_demod1;
    gr::analog::quadrature_demod_cf::sptr _fm_demod2;
    gr::analog::quadrature_demod_cf::sptr _fm_demod3;
    gr::blocks::multiply_const_ff::sptr _level_control1;
    gr::blocks::multiply_const_ff::sptr _level_control2;
    gr::blocks::multiply_const_ff::sptr _level_control3;
    gr::filter::rational_resampler_base_ccf::sptr _first_resampler;
    gr::filter::rational_resampler_base_ccf::sptr _resampler1;
    gr::filter::rational_resampler_base_ccf::sptr _resampler2;
    gr::filter::rational_resampler_base_ccf::sptr _resampler3;
    gr::filter::fft_filter_ccf::sptr _filter1;
    gr::filter::fft_filter_ccf::sptr _filter2;
    gr::filter::fft_filter_ccf::sptr _filter3;
    gr_mmdvm_sink_sptr _mmdvm_sink;
    gr::blocks::rotator_cc::sptr _rotator2;
    gr::blocks::rotator_cc::sptr _rotator3;

    int _samples_per_symbol;
    int _samp_rate;
    int _carrier_freq;
    int _filter_width;


};


#endif // GR_DEMOD_MMDVM_MULTI_H
