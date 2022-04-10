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

#ifndef BURSTTIMER_H
#define BURSTTIMER_H

#include <mutex>
#include <chrono>
#include <QVector>

static const uint64_t BURST_DELAY = 100000000L; // nanosec
static const uint64_t SLOT_TIME = 30000000L;

/// Delay between FPGA timestamping logic and antenna
/// Seems to also depend on sample rate, the higher the sample rate the smaller the delay
static const uint64_t PHY_DELAY = 50000L; // nanosec (B20X: 293000L)

class BurstTimer
{
public:
    BurstTimer(uint64_t samples_per_slot=720, uint64_t time_per_sample=41667L,
               uint64_t slot_time=SLOT_TIME, uint64_t burst_delay=BURST_DELAY);
    ~BurstTimer();
    void set_enabled(bool value);
    void set_params(uint64_t samples_per_slot, uint64_t time_per_sample, uint64_t slot_time, uint64_t burst_delay);
    void reset_timer();
    uint64_t get_time_delta();
    void set_timer(uint64_t value);
    void increment_sample_counter();
    int check_time();
    uint64_t allocate_slot(int slot_no);

private:
    bool _enabled;
    std::mutex _timing_mutex;
    std::mutex _slot_mutex;
    struct slot {
        uint8_t slot_no;
        uint64_t slot_time;
        uint64_t slot_sample_counter;
    };
    uint64_t _samples_per_slot;
    uint64_t _time_per_sample;
    uint64_t _slot_time;
    uint64_t _burst_delay;
    uint64_t _sample_counter;
    uint64_t _last_slot;
    std::chrono::high_resolution_clock::time_point t1;
    std::chrono::high_resolution_clock::time_point t2;
    QVector<slot*> _slot_times;
    uint64_t _time_base;
};


#endif // BURSTTIMER_H
