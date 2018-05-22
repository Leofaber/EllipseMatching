"""
python slider_ellipse_test.py ../test_9_ref_ell.ell ../test_9_slider_ell.ell 25 NO 1

python slider_ellipse_test.py
    ../test_9_ref_ell.ell
    ../test_9_slider_ell.ell
    25
    NO
    1
"""


import os
import subprocess
import sys
import matplotlib.pyplot as plt
from matplotlib.patches import Ellipse
from matplotlib.patches import Patch
import numpy.random as rnd
import numpy.linalg
from pprint import pprint

class Point:
    def __init__(self,x,y):
        self.x = x
        self.y = y
    def copy_from(self,point):
        self.x = point.x
        self.y = point.y
    def show(self):
        print "(",self.x,",",self.y,")"

class GeomEngine:

    def __init__(self):
        def N(point,ts):
            point.y += ts
        def S(point,ts):
            point.y -= ts
        def O(point,ts):
            point.x -= ts
        def E(point,ts):
            point.x += ts

        def NE(point,ts):
            N(point,ts)
            E(point,ts)
        def NO(point,ts):
            N(point,ts)
            O(point,ts)
        def SE(point,ts):
            S(point,ts)
            E(point,ts)
        def SO(point,ts):
            S(point,ts)
            O(point,ts)

        self.options = {   'N' : N,
                           'S' : S,
                           'O' : O,
                           'E' : E,
                           'NE' : NE,
                           'NO' : NO,
                           'SE' : SE,
                           'SO' : SO,
                       }

    def applyTranslationTo(self, point, translation_direction, translation_step):
        self.options[translation_direction](point, translation_step)

    def computeTranslation(self, start_point, translation_size, translation_direction, translation_step=1):
        assert translation_direction == 'N' or 'S' or 'O' or 'E' or 'NE' or 'NO' or 'NE' or 'SO' or 'SE'

        points = [start_point]

        old_point = Point(0,0)
        new_point = Point(0,0)

        new_point.copy_from(start_point)

        dist = 0
        while translation_size - dist > 0:
            old_point.copy_from(new_point)
            self.applyTranslationTo(new_point, translation_direction, translation_step)
            points.append(Point(new_point.x, new_point.y))
            a = numpy.array((old_point.x ,old_point.y))
            b = numpy.array((new_point.x, new_point.y))
            dist += numpy.linalg.norm(a-b)
            #print "\n",dist

        return points




class Plot:

    def __init__(self):
        self.fig = plt.figure(0)
        self.ax = self.fig.add_subplot(111, aspect='equal')
        self.overlap = self.fig.add_subplot(111, aspect='equal')

        self.colors = {
             ' is contained but tangent to ' : 'blue',
             ' overlap with ' : 'yellow',
             ' is external to ' : 'magenta',
             ' is external but tangent to ' : 'black',
             ' is contained in ' : 'brown'
            }
        self.p1 = Patch(color='blue', label='is contained but tangent to')
        self.p2 = Patch(color='yellow', label='overlap with')
        self.p3 = Patch(color='magenta', label='is external to')
        self.p4 = Patch(color='black', label='is external but tangent to')
        self.p5 = Patch(color='brown', label='is contained in')
        plt.legend(handles=[self.p1,self.p2,self.p3,self.p4,self.p5], prop={'size': 10})

    def setAxisLimits(self, ref_ell, sli_ell):
        a = numpy.array((ref_ell.x ,ref_ell.y))
        b = numpy.array((sli_ell.x, sli_ell.y))
        dist = numpy.linalg.norm(a-b)
        self.ax.set_xlim(ref_ell.x-dist ,ref_ell.x+dist)
        self.ax.set_ylim(ref_ell.y-dist ,ref_ell.y+dist)

        # It accepts a List<Ell>
    def addEllToGraph(self, ell, color, alpha):
        ell_matplotlib = Ellipse((ell.x, ell.y), ell.a*2, ell.b*2, ell.r)
        self.ax.add_artist(ell_matplotlib)
        self.overlap.add_artist(ell_matplotlib)
        ell_matplotlib.set_clip_box(self.ax.bbox)
        ell_matplotlib.set_alpha(alpha)
        ell_matplotlib.set_facecolor(color)

    def show(self):
        plt.show()

    # start = [x,y]  stop = [x,y]
    def addLine(self, start, stop):
        plt.plot( start, stop, color='k', linestyle='-', linewidth=1)


class Ell:
    # The class "constructor" - It's actually an initializer
    def __init__(self, filename, name='test', x=0, y=0, a=0, b=0, r=0):
        if(filename=='None'):
            self.name = name
            self.x = float(x)
            self.y = float(y)
            self.a = float(a)
            self.b = float(b)
            self.r = float(r)
        else:
            with open(filename) as input_file:
                content = input_file.readlines()
            ell_params = content[0].split()
            assert len(ell_params) == 6
            self.name = ell_params[0]
            self.x = float(ell_params[1])
            self.y = float(ell_params[2])
            self.a = float(ell_params[3])
            self.b = float(ell_params[4])
            self.r = float(ell_params[5])

    def show(self):
        pprint(vars(self))

    def toString(self):
        return self.name+' '+str(self.x)+' '+str(self.y)+' '+str(self.a)+' '+str(self.b)+' '+str(self.r)+'\n';






""" Input """
ref_ell_file_input = sys.argv[1]
slider_ell_file_input = sys.argv[2]
translation_size = float(sys.argv[3])      # magnitude
translation_direction = sys.argv[4] # N S O E, NE, NO, SO, SE
translation_step = float(sys.argv[5])      # magnitude of the i-step

print  sys.argv[1:]


""" Ells """
ref_ell = Ell(ref_ell_file_input)
ref_ell.show()
slider_ell = Ell(slider_ell_file_input)
slider_ell.show()
#test_ell = Ell('None','TEST',1,2,3,4,5)
#test_ell.show()




""" Geom Engine """
ge = GeomEngine()
start = Point(slider_ell.x, slider_ell.y)
points = ge.computeTranslation(start, translation_size,  translation_direction, translation_step)


""" Generating translated ellipses """
translated_ellipses = []
i = 0
for p in points:
    translated_ellipses.append(Ell('None', 'SLI',p.x, p.y, slider_ell.a/2, slider_ell.b/2, slider_ell.r))
    i+=1

translated_ell = translated_ellipses[-1]

print "\nGenerated ellipses:"
for e in translated_ellipses:
    e.show()



""" Graph """
plot = Plot()
plot.setAxisLimits(ref_ell, translated_ellipses[0])
plot.addEllToGraph(ref_ell, 'red', 0.5)
plot.addLine([slider_ell.x, translated_ell.x], [slider_ell.y, translated_ell.y])
#plot.show()





""" Generating slide ellipses input file """
try:
    os.remove('tmp.ell')
except OSError:
    pass


print "\nWritten ellipses:"
with open('tmp.ell', 'a') as the_file:
    for e in translated_ellipses:
        the_file.write(e.toString());




""" Execution of the program """
bashCommand = 'octave ELLIPSE_MATCHING.m '+ref_ell_file_input+' tmp.ell ../python_slider_test.ell'
process = subprocess.Popen(bashCommand.split(), stdout=subprocess.PIPE)
output, error = process.communicate()


print output
print error
print '-- END OF MATLAB PROGRAM'



""" Results analysis """
try:
    os.remove('temp_out.txt')
except OSError:
    pass

# storing matlab output in a file
with open('temp_out.txt', 'a') as output_file:
    output_file.write(output)

# processing output file
with open('temp_out.txt') as output_file:
    content = output_file.readlines()

results = []
ellipses = []

[ results.append(x.split('SLI')[1].split('[')[0]) for x in content if x[0]=='[' and len(x) > 20]
[ ellipses.append(x.split('--')[1][2:].strip()) for x in content if x[0]=='[' and len(x) > 20]

for i in range(len(results)):
    print ellipses[i].split()[0]+str(i+1)+results[i]+ref_ell.toString().split()[0]


assert len(ellipses) == len(results)


# Slider ellipses
i = 0
for i in range(len(translated_ellipses)):
    plot.addEllToGraph(translated_ellipses[i], plot.colors[results[i]], 0.3)
    i+=1



plot.show()

sys.exit()
