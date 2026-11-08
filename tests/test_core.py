import unittest
from core import PulseMeter

class MeterTests(unittest.TestCase):
    def test_meter(self):
        m = PulseMeter(1000, "150")
        self.assertEqual(m.record(1, 200)["energy_kwh"], "0")
        r = m.record(2, 700)
        self.assertEqual(r["energy_kwh"], "0.5")
        self.assertEqual(r["estimated_cost"], "75.0")
    def test_counter_reset(self):
        m = PulseMeter(1000)
        m.record(1, 10)
        with self.assertRaises(ValueError): m.record(2, 9)
        with self.assertRaises(ValueError): m.record(1, 11)
    def test_validation(self):
        with self.assertRaises(ValueError): PulseMeter(0)
        with self.assertRaises(ValueError): PulseMeter(100, "-1")
        with self.assertRaises(ValueError): PulseMeter(100, "NaN")
