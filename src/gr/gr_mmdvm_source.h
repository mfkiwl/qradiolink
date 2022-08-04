// Written by Adrian Musceac YO8RZZ , started March 2021.
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

#ifndef GR_MMDVM_SOURCE_H
#define GR_MMDVM_SOURCE_H

#include <gnuradio/sync_block.h>
#include <gnuradio/sync_interpolator.h>
#include <gnuradio/io_signature.h>
#include "src/bursttimer.h"
#include <zmq.hpp>
#include <chrono>


class gr_mmdvm_source;

typedef boost::shared_ptr<gr_mmdvm_source> gr_mmdvm_source_sptr;

gr_mmdvm_source_sptr make_gr_mmdvm_source(BurstTimer *burst_timer, uint8_t cn=0);

class gr_mmdvm_source : public gr::sync_block
{
public:
    gr_mmdvm_source(BurstTimer *burst_timer, uint8_t cn=0);
    ~gr_mmdvm_source();

    int work(int noutput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    bool start();
    void get_zmq_message();

private:

    void add_time_tag(uint64_t nsec, int offset);
    void handle_idle_time(int timing_adjust, short *out, int noutput_items);
    int handle_data_bursts(short *out, unsigned int n);
    void alternate_slots();
    BurstTimer *_burst_timer;
    zmq::context_t _zmqcontext;
    zmq::socket_t _zmqsocket;
    std::vector<uint8_t> control_buf;
    std::vector<int16_t> data_buf;
    gr::thread::mutex _mutex;
    int _channel_number;
    std::chrono::high_resolution_clock::time_point t1;
    std::chrono::high_resolution_clock::time_point t2;
    int _sn;
    int64_t _correction_time;
    bool _add_time_tag;

};



#endif // GR_MMDVM_SOURCE_H
