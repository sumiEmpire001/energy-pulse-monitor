#include <cassert>
#include <cmath>
#include <limits>
#include "EnergyPulseMonitor.h"
int main(){
 using namespace energy;
 Meter m(1000,150);
 Report baseline=m.record(1,200);
 assert(baseline.valid&&baseline.energy_kwh==0);
 Report second=m.record(2,700);
 assert(second.valid&&second.delta_pulses==500);
 assert(fabs(second.energy_kwh-0.5)<1e-9&&fabs(second.estimated_cost-75.0)<1e-9);
 assert(!m.record(3,699).valid);
 assert(!m.record(2,701).valid);
 assert(m.record(4,700).valid);
 Meter bad(0);assert(!bad.record(1,100).valid);
 Meter neg(100,-1);assert(!neg.record(1,100).valid);
 Meter nan(100,std::numeric_limits<double>::quiet_NaN());
 assert(!nan.record(1,100).valid);
}
