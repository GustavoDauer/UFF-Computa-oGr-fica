# Aluno: Gustavo Henrique Mello Dauer
# Computaçao Grafica 2014.2 - Ciencia da Computaçao - Universidade Federal Fluminense

import numpy as numplib
import matplotlib.pyplot as plotlib
import scipy.interpolate as interpolatelib

pontos = [[0, 0], [0, 2], [2, 3], [4, 0], [6, 3], [8, 2], [8, 0]];
pontos = numplib.array(pontos) # Vetor
t = range(len(pontos)) # Tamanho do vetor
vaux = numplib.linspace(0.0, len(pontos) - 1, 100)
x = pontos[:,0] # Ponto x
y = pontos[:,1] # Ponto y
tuplax = interpolatelib.splrep(t, x, k=3)
tuplay = interpolatelib.splrep(t, y, k=3)
listax = list(tuplax)
xl = x.tolist()
listax[1] = xl + [0.0, 0.0, 0.0, 0.0]
listay = list(tuplay)
yl = y.tolist()
listay[1] = yl + [0.0, 0.0, 0.0, 0.0]
x_i = interpolatelib.splev(vaux, listax)
y_i = interpolatelib.splev(vaux, listay)


figura = plotlib.figure()
faux = figura.add_subplot(233)
plotlib.plot(x, y)
plotlib.plot(x_i, y_i)
plotlib.xlim([min(x) - 0.2, max(x) + 0.2])
plotlib.ylim([min(y) - 0.2, max(y) + 0.2])
plotlib.title('B-Spline da funçao f(x(t), y(t))')
plotlib.show()
