import sys
import subprocess
from test_utils import Point,Ell,Plot,createEllipsesFromFile

# python draw_ellipses.py ../ERROR_ELLS/test1A.ell ../ERROR_ELLS/test1B.ell


ell_file_input1 = sys.argv[1]
ell_file_input2 = sys.argv[2]


ells1 = Ell(ell_file_input1)
ells2 = Ell(ell_file_input2)

ells1.show()
ells2.show()

""" Graph """
plot = Plot(1)

max_axis = max(ells1.a, ells1.b, ells2.a, ells2.b)

plot.subplots[0].set_xlim(ells2.x-max_axis*3/2 ,ells2.x+max_axis*3/2)
plot.subplots[0].set_ylim(ells2.y-max_axis*3/2 ,ells2.y+max_axis*3/2)

plot.addEllToGraph(ells1, 0, 'white', 0.5)
plot.addEllToGraph(ells2, 0, 'red', 0.5)

plot.show()
