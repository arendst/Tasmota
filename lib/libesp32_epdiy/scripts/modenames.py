mode_names = {
    0: "MODE_INIT",
    1: "MODE_DU",
    2: "MODE_GC16",
    3: "MODE_GC16_FAST",
    4: "MODE_A2",
    5: "MODE_GL16",
    6: "MODE_GL16_FAST",
    7: "MODE_DU4",
    10: "MODE_GL4",
    11: "MODE_GL16_INV",
    0x10: "MODE_EPDIY_WHITE_TO_GL16",
    0x11: "MODE_EPDIY_BLACK_TO_GL16",
    0x3F: "MODE_UNKNOWN_WAVEFORM",
}

def mode_id(name):
    for i, n in mode_names.items():
        if name == n:
            return i
    return 0x3F
