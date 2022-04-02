#include "bursttimer.h"
#include <QDebug>

static const uint64_t BURST_DELAY = 300000000L; // nanosec
static const uint64_t SLOT_TIME = 30000000L;

BurstTimer::BurstTimer()
{
    _sample_counter = 0;
    _last_slot = 0;
    _time_base = 0;
}

BurstTimer::~BurstTimer()
{

}

uint64_t BurstTimer::get_time_delta()
{
    t2 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1).count();
}

void BurstTimer::reset_timer()
{
    std::unique_lock<std::mutex> guard(_slot_mutex);
    _sample_counter = 0;
    _time_base = 0;
    t1 = std::chrono::high_resolution_clock::now();
    //_last_slot = _time_base + _burst_timer.nsecsElapsed();
    qDebug() << "Restarted burst timer";
}

void BurstTimer::set_timer(uint64_t value)
{
    std::unique_lock<std::mutex> guard(_slot_mutex);
    qDebug() << "Set timer: " << value;
    _time_base = value;
    t1 = std::chrono::high_resolution_clock::now();
}

void BurstTimer::increment_sample_counter()
{
    std::unique_lock<std::mutex> guard(_slot_mutex);
    _sample_counter++;
}

void BurstTimer::add_slot(uint8_t slot_no, uint64_t slot_time)
{
    slot *s = new slot;
    s->slot_no = slot_no;
    s->slot_time = slot_time;
    std::unique_lock<std::mutex> guard(_slot_mutex);
    _slot_times.append(s);
}

void BurstTimer::pop_slot()
{
    std::unique_lock<std::mutex> guard(_slot_mutex);
    if(_slot_times.size() > 0)
        _slot_times.removeFirst();
}

int BurstTimer::check_time()
{
    std::unique_lock<std::mutex> guard(_slot_mutex);
    if(_slot_times.size() < 1)
        return 0;
    slot *s = _slot_times[0];
    uint64_t sample_time = (_sample_counter * 41667L) / 1000L;
    if(sample_time >= s->slot_time && s->slot_sample_counter == 0)
    {
        s->slot_sample_counter++;
        return s->slot_no;
    }
    else if(sample_time >= s->slot_time)
    {
        if(s->slot_sample_counter >= 719)
        {
            delete _slot_times[0];
            _slot_times.removeFirst();
            //qDebug() << "Slots: " << _slot_times.size();
            return 0;
        }
        s->slot_sample_counter++;
    }
    return 0;
}

uint64_t BurstTimer::allocate_slot(int slot_no)
{
    std::unique_lock<std::mutex> guard(_slot_mutex);
    slot *s = new slot;
    s->slot_no = (uint8_t)slot_no;
    uint64_t elapsed = get_time_delta();
    if(elapsed < _last_slot)
    {
        _last_slot = _last_slot + SLOT_TIME;
    }
    else if(_last_slot == 0)
    {
        _last_slot = elapsed;
    }
    else if((elapsed - _last_slot) > (10L * SLOT_TIME))
    {

        _last_slot = elapsed;
    }
    else
    {
        _last_slot = _last_slot + SLOT_TIME;
    }
    uint64_t usec = (_last_slot + BURST_DELAY) / 1000L;
    s->slot_time = usec;
    s->slot_sample_counter = 0;
    _slot_times.append(s);
    return usec;
}



