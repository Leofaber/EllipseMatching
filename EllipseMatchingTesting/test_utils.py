from pprint import pprint
import matplotlib.pyplot as plt
from matplotlib.patches import Ellipse
from matplotlib.patches import Patch
import numpy.random as rnd
import numpy.linalg
import os
import subprocess
import sys

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

    def __init__(self, numberOfPlot):
        self.colors = {
             'is contained but tangent to' : 'blue',
             'overlap with' : 'yellow',
             'is external to' : 'magenta',
             'is external but tangent to' : 'black',
             'is contained in' : 'brown',
             'is equal to' : 'grey'
        }
        self.p1 = Patch(color='blue', label='is contained but tangent to')
        self.p2 = Patch(color='yellow', label='overlap with')
        self.p3 = Patch(color='magenta', label='is external to')
        self.p4 = Patch(color='black', label='is external but tangent to')
        self.p5 = Patch(color='brown', label='is contained in')
        self.p6 = Patch(color='grey', label='is equal to')

        self.figures = []
        self.subplots = []
        self.numberOfPlot = numberOfPlot
        for i in range(self.numberOfPlot):
            self.figures.append(plt.figure(i))
            self.subplots.append(plt.subplot(111, aspect='equal'))
            self.subplots[-1].legend(handles=[self.p1,self.p2,self.p3,self.p4,self.p5,self.p6], prop={'size': 10})#, loc='upper right', bbox_to_anchor=(0.5, 0.5))

    def setFigureTitles(self,titles):
        for i in range(self.numberOfPlot):
            plt.figure(i)
            plt.title(titles[i])


    def setAxisLimits(self, ref_ell):#, sli_ell):
        max_axis = max(ref_ell.a, ref_ell.b)
        print "max axis: "+str(max_axis)
        for i in range(self.numberOfPlot):
            self.subplots[i].set_xlim(ref_ell.x-max_axis*3/2 ,ref_ell.x+max_axis*3/2)
            self.subplots[i].set_ylim(ref_ell.x-max_axis*3/2 ,ref_ell.x+max_axis*3/2)



        # It accepts a List<Ell>
    def addEllToGraph(self, ell, figure, color, alpha):
        if(self.checkPlotIndex(figure)):
            ell_matplotlib = Ellipse((ell.x, ell.y), ell.a*2, ell.b*2, ell.r)# it accepts the total length (diameter) of horizontal axis and the   total length (diameter) of vertical axis
            #ell_matplotlib.set_clip_box(self.first_plot.bbox)
            #ell_matplotlib.set_clip_box(self.second_plot.bbox)
            ell_matplotlib.set_alpha(alpha)
            ell_matplotlib.set_facecolor(color)
            plt.figure(figure)
            self.subplots[figure].add_artist(ell_matplotlib)
        else:
            print "Cant add Ellipse to figure "+str(figure)



    def show(self):
        plt.show()

    def checkPlotIndex(self,figure):
        if figure > self.numberOfPlot:
            return False
        else:
            return True

    # start = [x,y]  stop = [x,y]
    def addLine(self, figure, start, stop):
        if(self.checkPlotIndex(figure)):
            plt.figure(figure)
            self.subplots[figure].plot( start, stop, color='k', linestyle='-', linewidth=1)
        else:
            print "Cant add line to figure "+str(figure)



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


def createEllipsesFromFile(filename):
    ellipses = []
    with open(filename) as input_file:
        content = input_file.readlines()

    for line in content:
        ell_params = line.split()
        assert len(ell_params) == 6
        ellipses.append(Ell('None',ell_params[0],ell_params[1],ell_params[2],ell_params[3],ell_params[4],ell_params[5]))

    return ellipses


def parseOutput(language, ellipseMatchingOutput, ellipseNameExtraCharsNum):

    """ Results analysis """
    try:
        os.remove('temp_out.txt')
    except OSError:
        pass

    # storing matlab output in a file
    with open('temp_out.txt', 'a') as output_file:
        output_file.write(ellipseMatchingOutput);

    # processing output file
    with open('temp_out.txt') as output_file:
        content = output_file.readlines();



    if language == "matlab" and "END OF THE PROGRAM\n" not in content:
        print content;
        print '----> Error from Matlab Ellipse Matching :(';
        sys.exit();
    elif language == "c++" and ("END OF THE PROGRAM\n" not in content or any("Error" in c for c in content)):
        print content;
        print '----> Error from C++ Ellipse Matching :(';
        sys.exit();
    else:
        print '----> No errors from '+language+' Ellipse Matching :)';



    results = []
    ellipses = []


    for x in content:
        if x[0]=='[' and len(x) > 20:
            temp = x.split('--'); # --> ['[00036] SLI is external to [00001] REF', '7 SLI 0.000 15.00 2.500000 1.500000 1.570796\n']
            res_temp = temp[0];
            ell_temp = temp[1];
            results.append(res_temp.split('SLI')[1].split('[')[0][ellipseNameExtraCharsNum+1:-1]);
            ellipses.append(ell_temp[2:].strip());

    #[ results.append(((x.split('SLI')[1].split('[')[0])[ellipseNameExtraCharsNum+1:])[:-1]) for x in content if x[0]=='[' and len(x) > 20]
    #[ ellipses.append(x.split('--')[1][2:].strip()) for x in content if x[0]=='[' and len(x) > 20]
    #print results
    #print ellipses

    couple = results, ellipses;

    return couple


def runEllipseMatching(language, input1, input2):
    output="";
    error="";
    path="";
    bashCommandStart="";

    if language == "matlab":
        path = os.environ['HOME']+"/EllipseMatching/EllipseMatchingMatlab_DEBUG";
    elif language == "c++":
        path = os.environ['HOME']+"/EllipseMatching/EllipseMatchingC++";

    print '\n\nMoving to '+path+' program folder'
    os.chdir(path);

    if language == "matlab":
        bashCommandStart = 'octave ELLIPSE_MATCHING.m ../EllipseMatchingTesting/'+input1+' ../EllipseMatchingTesting/'+input2+' ../python_test.ell';
    elif language == "c++":
        bashCommandStart = './bin/AG_ellipse_matching ../EllipseMatchingTesting/'+input1+' ../EllipseMatchingTesting/'+input2+' ../python_test.ell';
    else:
        print 'Error. '+language+' not supported.';

    print 'Starting '+language+' Ellipse Matching with bash command: '+bashCommandStart;
    process = subprocess.Popen(bashCommandStart.split(), stdout=subprocess.PIPE)
    output, error = process.communicate()


    #print output
    #print error
    return (output,error)
