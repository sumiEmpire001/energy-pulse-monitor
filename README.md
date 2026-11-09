# energy-pulse-monitor

Converts cumulative meter pulses into energy usage and estimated costs using decimal arithmetic. First reading establishes baseline; resets are rejected. This is not a billing-grade meter and does not persist data.

## Requirements and tests

Python 3.10+; standard library only.

```bash
python -m unittest discover -s tests -v
```

## Example

```python
from core import PulseMeter
meter = PulseMeter(1000, "150")
meter.record(1, 200)
print(meter.record(2, 700))
```

GitHub Actions runs the unit tests across Python 3.10, 3.12 and 3.13. Check CI status before treating a push as verified.

## Arduino / ESP32 C++ implementation

The Python decimal-based prototype is preserved. The C++11
`arduino/EnergyPulseMonitor/src/EnergyPulseMonitor.h` provides monotonic
counter checks, a baseline reading, accumulated pulse-to-kWh calculations,
overflow checks and **approximate** tariff estimates. Unlike Python Decimal,
embedded C++ uses floating point and is not appropriate for financial billing.

The `PulseIndicator` sketch counts isolated logic-level pulses on an
interrupt pin (Uno D2; ESP32 GPIO27) and displays activity on an LED
(Uno D8; ESP32 GPIO18). Use only a safely isolated pulse output or a
test button. **Never connect Arduino or ESP32 GPIO to live AC mains.**
Debouncing, counter persistence, rollover recovery and metering calibration
are still required for real deployments.

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```
GitHub Actions attempts GCC/Clang tests and Uno/ESP32 firmware compilation.
This prototype has not been validated on electricity metering hardware.
