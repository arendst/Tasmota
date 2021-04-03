#!env python3

import json
from modenames import mode_names, mode_id
import sys

"""
This script generates json files to generate waveforms from.
"""

FRAME_TIMES_WHITE_TO_GL16 = {
    "ED097OC4": [3.0, 3.0, 2.0, 2.0, 3.0, 3.0, 3.0, 4.0, 4.0, 5.0, 5.0, 5.0, 10.0, 20.0, 30.0],
    "ED097OC1": [3.0, 3.0, 2.0, 2.0, 3.0, 3.0, 3.0, 4.0, 4.0, 5.0, 5.0, 5.0, 10.0, 20.0, 30.0],
    "ED060SC4": [3.0, 3.0, 2.0, 2.0, 3.0, 3.0, 3.0, 4.0, 4.0, 5.0, 5.0, 5.0, 10.0, 20.0, 30.0],
    "ED060XC3": [5.0, 4.0, 3.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 5.0, 5.0, 5.0, 10.0, 20.0, 120.0],
    "ED047TC1": [3.0, 3.0, 2.0, 2.0, 3.0, 3.0, 3.0, 4.0, 4.0, 5.0, 5.0, 5.0, 10.0, 20.0, 30.0],
    "ED097TC2": [1.5, .8, .8, .8, .8, .8, 1.0, 1.0, 1.0, 1.0, 2.0, 2.0, 5.0, 10.0, 20.0],
    "ED060SCT": [1.5, .8, .8, .8, .8, .8, 1.0, 1.0, 1.0, 1.0, 2.0, 2.0, 5.0, 10.0, 20.0],
    "ED133UT2": [6.0, 3.0, 2.0, 2.0, 2.0, 2.0, 2.0, 3.0, 3.0, 3.0, 5.0, 5.0, 10.0, 20.0, 30.0],
}

FRAME_TIMES_BLACK_TO_GL16 = {

    "ED097OC4": [1.0, 1.0, .8, .8, .8, .8, .8, 1.0, 1.0, 1.5, 1.5, 2.0, 2.0, 10.0, 30.0],
    "ED097OC1": [1.0, 1.0, .8, .8, .8, .8, .8, 1.0, 1.0, 1.5, 1.5, 2.0, 2.0, 10.0, 30.0],
    "ED060SC4": [1.0, 1.0, .8, .8, .8, .8, .8, .8, .8, 1.0, 1.0, 2.0, 2.0, 10.0, 30.0],
    "ED060XC3": [1.0, 1.0, .5, .5, .5, .5, .5, .5, .5, 1.0, 1.0, 2.0, 2.0, 10.0, 30.0],
    "ED047TC1": [1.0, 1.0, .8, .8, .8, .8, .8, 1.0, 1.0, 1.5, 1.5, 2.0, 2.0, 10.0, 30.0],
    "ED097TC2": [1.5, .8, .4, .3, .3, .3, .3, .6, .6, .6, .6, 1.5, 2.0, 5.0, 15.0],
    "ED060SCT": [1.5, .8, .4, .3, .3, .3, .3, .6, .6, .6, .6, 1.5, 2.0, 5.0, 15.0],
    "ED133UT2": [2.0, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 2.0, 3.0, 3.0, 5.0, 10.0, 20.0]
}

FRAME_TIMES_DU = {
    "ED097OC4": [100, 100, 100, 100],
    "ED097OC1": [100, 100, 100, 100],
    "ED060SC4": [100, 100, 100, 100],
    "ED060XC3": [100, 100, 100, 100],
    "ED047TC1": [100, 100, 100, 100, 100],
    "ED097TC2": [100, 100, 100, 100, 100],
    "ED060SCT": [100, 100, 100, 100, 100],
    "ED133UT2": [100, 100, 100, 100]
}


def generate_frame(lut_func):
    phase = []
    for t in range(0, 32):
        line = []
        for f in range(0, 32):
            line.append(lut_func(t, f))
        phase.append(line)
    return phase

def generate_mode_x_to_GL16(display, mode):
    """
    Generates LUTs for drawing successive black frames
    on a white screen.
    """
    frame_times = None
    dirfunc = None
    if mode_names[mode] == "MODE_EPDIY_WHITE_TO_GL16":
        frame_times = FRAME_TIMES_WHITE_TO_GL16[display]
        def dirfunc(f, t, frame):
            return (t < (30 - 2 * frame) and f == 30)
        dirfunc = dirfunc

    elif mode_names[mode] == "MODE_EPDIY_BLACK_TO_GL16":
        frame_times = FRAME_TIMES_BLACK_TO_GL16[display]
        def dirfunc(f, t, frame):
            return 2 * (t > (2 * frame) and f == 0)
        dirfunc = dirfunc
    else:
        raise ValueError(f"Cannot generate {mode} here.")

    phases = []
    for frame in range(15):
        phase = generate_frame(lambda t, f: dirfunc(f, t, frame))
        phases.append(phase)
    return {"mode": mode, "ranges": [{"index": 0, "phases": phases, "phase_times": frame_times}]}

def generate_mode_GC16(display):
    """
    Do arbitrary transitions by going to full black, then the desired value.
    """
    phases = []
    for frame in range(15):
        phase = generate_frame(lambda t, f: f >= (30 - 2 * frame))
        phases.append(phase)
    for frame in range(15):
        phase = generate_frame(lambda t, f: 2 * (t > (2 * frame)))
        phases.append(phase)

    frame_times = FRAME_TIMES_WHITE_TO_GL16[display] + FRAME_TIMES_BLACK_TO_GL16[display]
    return {"mode": mode_id("MODE_GC16"), "ranges": [{"index": 0, "phases": phases, "phase_times": frame_times}]}

def generate_mode_GL16(display):
    """
    Do non-flashing arbitrary transitions by going to full black, then the desired value.
    """
    phases = []
    for frame in range(15):
        phase = generate_frame(lambda t, f: f >= (30 - 2 * frame))
        phase[30][30] = 0
        phase[0][0] = 0
        phases.append(phase)
    for frame in range(15):
        phase = generate_frame(lambda t, f: 2 * (t > (2 * frame)))
        phase[30][30] = 0
        phase[0][0] = 0
        phases.append(phase)

    frame_times = FRAME_TIMES_WHITE_TO_GL16[display] + FRAME_TIMES_BLACK_TO_GL16[display]
    return {"mode": mode_id("MODE_GL16"), "ranges": [{"index": 0, "phases": phases, "phase_times": frame_times}]}


def generate_du(display):
    """
    Generate the "direct update" mode for a display.
    """
    frame_times = FRAME_TIMES_DU[display];
    num_phases = len(frame_times)

    phases = []
    for frame in range(num_phases):
        def lutfunc(t, f):
            if t == 0 and f > 0:
                return 1
            elif t == 30 and f < 30:
                return 2
            else:
                return 0
        phase = generate_frame(lutfunc)
        phases.append(phase)

    return {"mode": mode_id("MODE_DU"), "ranges": [{"index": 0, "phases": phases, "phase_times": frame_times}]}


def generate_epdiy_waveform(display):
    # we say that these temperature waveforms are valid in ~room temperature.
    default_temperature_range = {"from": 20, "to": 30}

    white_to_gl16 = generate_mode_x_to_GL16(display, mode_id("MODE_EPDIY_WHITE_TO_GL16"))
    black_to_gl16 = generate_mode_x_to_GL16(display, mode_id("MODE_EPDIY_BLACK_TO_GL16"))
    gc16 = generate_mode_GC16(display)
    gl16 = generate_mode_GL16(display)
    du = generate_du(display)

    temperature_ranges = {"range_bounds": [default_temperature_range]}
    return {"temperature_ranges": temperature_ranges, "modes": [
        du,
        gc16,
        gl16,
        white_to_gl16,
        black_to_gl16
    ]}


waveform = generate_epdiy_waveform(sys.argv[1])
print (json.dumps(waveform))
