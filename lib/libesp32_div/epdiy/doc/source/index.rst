.. EPDiy documentation master file, created by
   sphinx-quickstart on Sat Apr 18 23:41:08 2020.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

The EPDiy Driver Board
======================

.. toctree::
   :maxdepth: 2
   :caption: Quickstart:

   about.rst
   getting_started.rst
   filegen.rst
   tips.rst
   api.rst

EPDiy is a driver board which talks to affordable E-Paper (or E-Ink) screens, which are usually sold as replacement screens for E-Book readers. Why are they interesting?

* Easy on the eyes and paper-like aesthetics
* No power consumption when not updating
* Sunlight-readable

Ready-made DIY modules for this size and with 4bpp (16 Grayscale) color support are currently quite expensive. This project uses Kindle replacement screens, which are available for 20$ (small) / 30$ (large) on ebay!

The EPDiy driver board targets multiple E-Paper displays. As the driving method for all matrix-based E-ink displays seems to be more or less the same, only the right connector and timings are needed. The EPDiy PCB features a 33pin and a 39pin connector, which allow to drive the following display types: ED097OC4, ED060SC4, ED097TC2

:ref:`getting_started`

.. image:: img/demo.jpg

.. image:: img/board_p1.jpg

.. image:: img/board_p2.jpg
