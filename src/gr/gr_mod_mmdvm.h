#ifndef GR_MOD_MMDVM_H
#define GR_MOD_MMDVM_H

#include <gnuradio/hier_block2.h>
#include <gnuradio/analog/frequency_modulator_fc.h>
#include <gnuradio/blocks/short_to_float.h>
#include <gnuradio/blocks/add_blk.h>
#include <gnuradio/filter/firdes.h>
#include <gnuradio/filter/fft_filter_ccf.h>
#include <gnuradio/filter/fft_filter_fff.h>
#include <gnuradio/filter/rational_resampler_base.h>
#include <gnuradio/blocks/multiply_const.h>


class gr_mod_mmdvm;

typedef boost::shared_ptr<gr_mod_mmdvm> gr_mod_mmdvm_sptr;
gr_mod_mmdvm_sptr make_gr_mod_mmdvm(int sps=125, int samp_rate=1000000, int carrier_freq=1700,
                                          int filter_width=5000);

class gr_mod_mmdvm : public gr::hier_block2
{
public:
    explicit gr_mod_mmdvm(int sps=125, int samp_rate=1000000, int carrier_freq=1700,
                             int filter_width=5000);
    void set_bb_gain(float value);

private:

    gr::analog::frequency_modulator_fc::sptr _fm_modulator;
    gr::filter::rational_resampler_base_ccf::sptr _resampler;
    gr::blocks::multiply_const_cc::sptr _amplify;
    gr::blocks::multiply_const_cc::sptr _bb_gain;
    gr::blocks::multiply_const_ff::sptr _audio_amplify;
    gr::filter::fft_filter_ccf::sptr _filter;
    gr::blocks::short_to_float::sptr _short_to_float;


    int _samp_rate;
    int _sps;
    int _carrier_freq;
    int _filter_width;

};
#endif // GR_MOD_MMDVM_H