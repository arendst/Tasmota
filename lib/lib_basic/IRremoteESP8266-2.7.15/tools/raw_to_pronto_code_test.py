#!/usr/bin/python3
"""Unit tests for raw_to_pronto_code.py"""
from io import StringIO
import unittest
import raw_to_pronto_code as pronto

class TestRawToPronto(unittest.TestCase):
  """Unit tests for the methods in raw_to_pronto_code."""

  def test_parse_and_report_at_38000(self):
    """Tests for the parse_and_report() function @ 38kHz."""

    output = StringIO()
    input_str = """
        uint16_t rawData[7] = {
            20100, 20472, 15092, 30704, 20102, 20472, 15086};"""
    pronto.parse_and_report(input_str, 38000, 100000, True, False, False,
                            output)
    self.assertEqual(
        output.getvalue(),
        "Pronto code = "
        "'0000 006D 0004 0000 02fb 0309 023d 048e 02fb 0309 023d 0ed8'\n")

  def test_parse_and_report_at_36000(self):
    """Tests for the parse_and_report() function @ 36kHz."""

    output = StringIO()
    input_str = """
        uint16_t rawData[7] = {
            20100, 20472, 15092, 30704, 20102, 20472, 15086};"""
    pronto.parse_and_report(input_str, 36000, 100000, True, False, False,
                            output)
    self.assertEqual(
        output.getvalue(),
        "Pronto code = "
        "'0000 0073 0004 0000 02d3 02e0 021f 0451 02d3 02e0 021f 0e10'\n")

  def test_parse_and_report_at_57600(self):
    """Tests for the parse_and_report() function @ 57.6kHz."""

    output = StringIO()
    input_str = """
      uint16_t rawData[7] = {
          20100, 20472, 15092, 30704, 20102, 20472, 15086};"""
    pronto.parse_and_report(input_str, 57600, 100000, True, False, False,
                            output)
    self.assertEqual(
        output.getvalue(),
        "Pronto code = "
        "'0000 0047 0004 0000 0485 049b 0365 06e8 0485 049b 0364 1680'\n")

  def test_using_repeat(self):
    """Tests for the parse_and_report() function @38kHz using repeat section."""

    output = StringIO()
    input_str = """
        uint16_t rawData[7] = {
            20100, 20472, 15092, 30704, 20102, 20472, 15086};"""
    pronto.parse_and_report(input_str, 38000, 30000, False, False, False,
                            output)
    self.assertEqual(
        output.getvalue(),
        "Pronto code = "
        "'0000 006D 0000 0004 02fb 0309 023d 048e 02fb 0309 023d 0474'\n")

  def test_generate_code_output(self):
    """Tests for the parse_and_report() function geneating code output."""

    output = StringIO()
    input_str = """
        uint16_t rawData[7] = {
            20100, 20472, 15092, 30704, 20102, 20472, 15086};"""
    pronto.parse_and_report(input_str, 38000, 30000, True, True, False, output)
    self.assertEqual(
        output.getvalue(),
        "uint16_t pronto[12] = {0x0000, 0x006D, 0x0004, 0x0000, 0x02fb, "
        "0x0309, 0x023d, 0x048e, 0x02fb, 0x0309, 0x023d, 0x0474};\n")


if __name__ == '__main__':
  unittest.main(verbosity=2)
