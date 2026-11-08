from decimal import Decimal, InvalidOperation

class PulseMeter:
    def __init__(self, pulses_per_kwh, tariff_per_kwh="0"):
        if type(pulses_per_kwh) is not int or pulses_per_kwh <= 0:
            raise ValueError("pulses_per_kwh must be positive integer")
        try:
            self.tariff = Decimal(str(tariff_per_kwh))
        except InvalidOperation as exc:
            raise ValueError("invalid tariff") from exc
        if not self.tariff.is_finite() or self.tariff < 0:
            raise ValueError("invalid tariff")
        self.factor = pulses_per_kwh
        self.last = None
        self.total = 0

    def record(self, timestamp, cumulative_pulses):
        if type(timestamp) is not int or timestamp < 0 or type(cumulative_pulses) is not int or cumulative_pulses < 0:
            raise ValueError("invalid reading")
        if self.last is not None and (timestamp <= self.last[0] or cumulative_pulses < self.last[1]):
            raise ValueError("out-of-order timestamp or counter reset")
        delta = 0 if self.last is None else cumulative_pulses - self.last[1]
        self.total += delta
        self.last = (timestamp, cumulative_pulses)
        kwh = Decimal(self.total) / Decimal(self.factor)
        return {"delta_pulses": delta, "energy_kwh": str(kwh), "estimated_cost": str(kwh * self.tariff)}
