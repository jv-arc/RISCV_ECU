#!/home/jvctr/0/POLIno_qsys/quartus_project/rtl/tb/data_generation/.venv/bin/python

import argparse
from veriloghex import VerilogHex
from pathlib import Path
import numpy as np


#==================DEFAULT VALUES=========================
MEM_PATH = str(Path.cwd().parent / "fake_data.hex")
WORD_SIZE = 12
BYTES_WIDTH = int(WORD_SIZE/4)
MAX_ADC_VALUE = (2**WORD_SIZE)-1

MAX_MAP = 110
MIN_MAP = 10

SAMPLES   = 256




#==================GENERATION FUNCTIONS=========================

def generate_ramp(samples):
    return np.linspace(MIN_ADC_VALUE, MAX_ADC_VALUE, samples, dtype=int)


def generate_sine(cycles=4):
    t = np.linspace(0, 2*np.pi*cycles, SAMPLES)
    sine = (np.sin(t) + 1) * MAX_VAL / 2
    return sine.astype(int)


def generate_step(steps=8):
    step_size = SAMPLES // steps
    data = []
    for i in range(steps):
        value = (i * MAX_VAL) // (steps - 1)
        data.extend([value] * step_size)
    return np.array(data[:SAMPLES])


def generate_noise(mean=2048):
    std_dev = MAX_VAL // 8  # ~12% of range
    noise = np.random.normal(mean, std_dev, SAMPLES)
    return np.clip(noise, 0, MAX_VAL).astype(int)


def generate_saw(cycles=4):
    samples_per_cycle = SAMPLES // cycles
    saw = np.tile(np.linspace(0, MAX_VAL, samples_per_cycle, dtype=int), cycles)
    return saw[:SAMPLES]


def generate_map(cycles=6):
    x = np.linspace(0, 2*np.pi*cycles, SAMPLES)
    pressure = (MAX_MAP - MIN_MAP) / 2 * (np.sin(x) + 1) + MIN_MAP
    return np.round((pressure - MIN_MAP) / (MAX_MAP - MIN_MAP) * MAX_ADC_VALUE).astype(int)


def write_to_file(data):
    hex_width = (WORD_SIZE + 3) // 4  # Number of hex digits needed

    with open(MEM_PATH, 'w') as f:
        for value in data:
            f.write(f"{value:0{hex_width}x}\n")

    print(f"Data written to: {MEM_PATH}")
    print(f"Samples: {len(data)}, Range: [{min(data)}, {max(data)}]")


def main():
    parser = argparse.ArgumentParser(description='Generate ADC test patterns')
    parser.add_argument('pattern', choices=['ramp', 'sine', 'step', 'noise', 'saw', 'map'], help='Type of test pattern')
    args = parser.parse_args()

    generators = {
        'ramp': generate_ramp,
        'sine': generate_sine,
        'step': generate_step,
        'noise': generate_noise,
        'saw': generate_saw,
        'map': generate_map
    }
 
    data = generators[args.pattern]()
    write_to_file(data)


if __name__ == "__main__":
    main()
