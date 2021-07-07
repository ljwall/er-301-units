"""
"""
import matplotlib.pyplot as plt
import math
import numpy as np
from scipy import signal

CROSSINGS = 16
OVER_SAMPLE = 256


@np.vectorize
def sinc(x):
    """
    Calculate sin(pi*x)/(pi*x)

    Use Taylor expansion near zero.
    """
    x = x*np.pi
    if abs(x) < 0.1:
        return 1 - x**2 / 6 + x**4 / 120
    return math.sin(x)/x


def windowed_bl_impulse(over_sampling, crossings):
    """
    Creates a np array for a windowed band-limited impulse function

    args:
        over_sampling - factor to oversample this function
        crossings - how many zero crossings should be included in the window (on each side of x=0)

    returns:
        numpy array
    """
    N = 2*crossings*over_sampling
    times = np.linspace(-crossings, crossings, N)
    window = signal.windows.blackman(N)
    return window * sinc(times)


if __name__ == '__main__':
    data = windowed_bl_impulse(OVER_SAMPLE, CROSSINGS)
    step = signal.lfilter([1/OVER_SAMPLE], [1, -1], data)
    # Ensure really well normaled to one
    step_size = step[len(step) - 1]
    step /= step_size

    step -= np.concatenate((np.zeros(int(len(step)/2)), np.ones(int(len(step)/2))))

    plt.plot(data)
    plt.plot(step)

    plt.show()
    N = len(data)
    print('namespace ljw\n{')
    print(f'  float bli[{N}] = {{', end='')
    for x in data:
      print(f'{x:.9f},', end='')
    print('};\n')
    print(f'  float step_corrections[{N}] = {{', end='')
    for x in step:
      print(f'{x:.9f},', end='')
    print('};\n}')
