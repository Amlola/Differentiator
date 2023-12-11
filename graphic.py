import numpy as np
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt


def sin(x):
    return np.sin(x)


def cos(x):
    return np.cos(x)


def lg(x):
    return np.log10(x)


def ln(x):
    return np.log(x)


def tg(x):
    return np.tan(x)


def ctg(x):
    return 1 / tg(x)


f = open('input.txt')
function = f.readline().replace('^', '**')
a = -10
b = 10
if 'lg' in function or 'ln' in function:
    a = 1
    b = 20
x = np.linspace(a, b, 1000)

plt.title("График функции")

plt.plot(x, eval(function), color='red')

plt.savefig('plot.png')
