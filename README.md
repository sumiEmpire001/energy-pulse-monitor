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
