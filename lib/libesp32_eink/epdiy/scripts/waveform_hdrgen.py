#!env python3

import json
import sys
import argparse
from modenames import mode_names

parser = argparse.ArgumentParser()
parser.add_argument("--list-modes", help="list the available modes for tis file.", action = "store_true");
parser.add_argument("--temperature-range", help="only export waveforms in the temperature range of min,max °C.");
parser.add_argument("--export-modes", help="comma-separated list of waveform mode IDs to export.");
parser.add_argument("name", help="name of the waveform object.");

args = parser.parse_args()

waveforms = json.load(sys.stdin);


total_size = 0

def phase_to_c(phase,bits_per_pixel_c=4):
    
    N1 = len(phase)
    N2 = len(phase[0])
    N = 2**bits_per_pixel_c
   
    if N1%N != 0:
        raise ValueError(f"first dimension of phases is {N1}. Allowed are multiples of {N}")
    
    step1 = int(N1/N)
        
    if N2%N != 0:
        raise ValueError(f"second dimension of phases is {N2}. Allowed are multiples of {N}")
    
    step2 = int(N2/N)

    targets = []
    for t in range(0, N1, step1):
        chunk = 0
        line = []
        i = 0
        for f in range(0, N2, step2):
            fr = phase[t][f]
            chunk = (chunk << 2) | fr
            i += 1
            if i == 4:
                i = 0
                line.append(chunk)
                chunk = 0
        targets.append(line)

    return targets

def list_to_c(l):
    if isinstance(l, list):
        children = [list_to_c(c) for c in l]
        return "{" + ",".join(children) + "}"
    elif isinstance(l, int):
        return f"0x{l:02x}"
    else:
        assert(false)

if args.list_modes:
    for mode in waveforms["modes"]:
        print(f"""{mode["mode"]}: {mode_names[mode["mode"]]}""" )
    sys.exit(0)

tmin = -100
tmax = 1000

if args.temperature_range:
    tmin, tmax = map(int, args.temperature_range.split(","))

modes = []

mode_filter = [wm["mode"] for wm in waveforms["modes"]]

if args.export_modes:
    mode_filter = list(map(int, args.export_modes.split(",")))

mode_filter = [m for m in mode_filter if any([wm["mode"] == m for wm in waveforms["modes"]])]

num_modes = len(mode_filter)

temp_intervals = []
for bounds in waveforms["temperature_ranges"]["range_bounds"]:
    if bounds["to"] < tmin or bounds["from"] > tmax:
        continue
    temp_intervals.append(f"{{ .min = {bounds['from']}, .max = {bounds['to']} }}")

modes = []
num_ranges = -1
for m_index, mode in enumerate(waveforms["modes"]):

    if not mode["mode"] in mode_filter:
        continue

    ranges = []
    for i, r in enumerate(mode["ranges"]):
        bounds = waveforms["temperature_ranges"]["range_bounds"][i]
        if bounds["to"] < tmin or bounds["from"] > tmax:
            continue

        phases = []
        phase_count = len(r["phases"])
        prev_phase = None
        for phase in r["phases"]:
            phases.append(phase_to_c(phase))

        name = f"epd_wp_{args.name}_{mode['mode']}_{r['index']}"

        phase_times= None
        if r.get("phase_times"):
            phase_times = [str(int(t * 10)) for t in r["phase_times"]]
            print(f"const int {name}_times[{len(phase_times)}] = {{ {','.join(phase_times) } }};")


        phase_times_str = f"&{name}_times[0]" if phase_times else "NULL"
        print(f"const uint8_t {name}_data[{phase_count}][16][4] = {list_to_c(phases)};")
        print(f"const EpdWaveformPhases {name} = {{ .phases = {phase_count}, .phase_times = {phase_times_str}, .luts = (const uint8_t*)&{name}_data[0] }};")
        ranges.append(name)

    assert(num_ranges < 0 or num_ranges == len(ranges))

    num_ranges = len(ranges)
    name = f"epd_wm_{args.name}_{mode['mode']}"
    range_pointers = ','.join(['&' + n for n in ranges])
    print(f"const EpdWaveformPhases* {name}_ranges[{len(ranges)}] = {{ {range_pointers} }};")
    print(f"const EpdWaveformMode {name} = {{ .type = {mode['mode']}, .temp_ranges = {len(ranges)}, .range_data = &{name}_ranges[0] }};");
    modes.append(name)

mode_pointers = ','.join(['&' + n for n in modes])
range_data = ",".join(temp_intervals)

print(f"const EpdWaveformTempInterval {args.name}_intervals[{len(temp_intervals)}] = {{ {range_data} }};");
print(f"const EpdWaveformMode* {args.name}_modes[{num_modes}] = {{ {mode_pointers} }};");
print(f"const EpdWaveform {args.name} = {{ .num_modes = {num_modes}, .num_temp_ranges = {num_ranges}, .mode_data = &{args.name}_modes[0], .temp_intervals = &{args.name}_intervals[0] }};");
