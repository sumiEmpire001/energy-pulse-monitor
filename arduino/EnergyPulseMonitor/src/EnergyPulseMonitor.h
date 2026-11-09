#ifndef LORD_SUMI_ENERGY_PULSE_MONITOR_H
#define LORD_SUMI_ENERGY_PULSE_MONITOR_H
#include <stdint.h>
#include <math.h>
#include <limits.h>
namespace energy {
struct Report {
 bool valid;uint64_t delta_pulses;double energy_kwh,estimated_cost;
 Report(bool v,uint64_t delta,double kwh,double cost):
   valid(v),delta_pulses(delta),energy_kwh(kwh),estimated_cost(cost){}
};
class Meter {
 public:
 Meter(uint64_t pulses_per_kwh,double tariff_per_kwh=0):
   factor_(pulses_per_kwh),tariff_(tariff_per_kwh),
   have_last_(false),last_timestamp_(0),last_pulses_(0),total_(0){}
 Report record(uint32_t timestamp,uint64_t cumulative_pulses){
  if(factor_==0||!isfinite(tariff_)||tariff_<0||
     (have_last_&&(timestamp<=last_timestamp_||cumulative_pulses<last_pulses_)))
   return Report(false,0,0,0);
  uint64_t delta=have_last_?cumulative_pulses-last_pulses_:0;
  if(delta>UINT64_MAX-total_)return Report(false,0,0,0);
  double next_kwh=static_cast<double>(total_+delta)/static_cast<double>(factor_);
  double next_cost=next_kwh*tariff_;
  if(!isfinite(next_kwh)||!isfinite(next_cost))return Report(false,0,0,0);
  total_+=delta;last_pulses_=cumulative_pulses;last_timestamp_=timestamp;have_last_=true;
  return Report(true,delta,next_kwh,next_cost);
 }
 private:
 uint64_t factor_;double tariff_;bool have_last_;
 uint32_t last_timestamp_;uint64_t last_pulses_,total_;
};
}
#endif
