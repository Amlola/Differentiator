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


f = open('input_taylor.txt')
function = f.readline().replace('^', '**').strip()
a = -10
b = 10
if 'lg' in function or 'ln' in function:
    a = 0.001
    b = 20
x = np.linspace(a, b, 1000)

plt.title("График функции")

plt.plot(x, eval(function), color='red', label=function)
colors = ['yellow', 'green', 'blue', 'purple']
functions = []
for i in range(1, 5):
    function_s = []
    for j in range(i):
        function_s.append(f.readline().replace('^', '**').strip())
    func = ' + '.join(function_s)
    functions.append(func)
    plt.plot(x, eval(func), color=colors[i - 1], label=func)
plt.ylim(-5, 5)
plt.legend()
plt.savefig('plot.png')
plt.cla()
for i in range(len(functions)):
    diff = function + ' - ' + functions[i]
    plt.plot(x, eval(diff), color=colors[i], label=functions[i])
plt.legend()
plt.ylim(-5, 5)
plt.savefig('difference.png')
