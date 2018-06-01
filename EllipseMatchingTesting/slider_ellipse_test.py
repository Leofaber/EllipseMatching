"""
python slider_ellipse_test.py input_test/test_9_ref_ell.ell input_test/test_9_slider_ell.ell 25 NO 1

python slider_ellipse_test.py
    input_test/test_9_ref_ell.ell
    input_test/test_9_slider_ell.ell
    25
    NO
    1
"""


import os
import sys


from test_utils import Point,Ell,GeomEngine,Plot,parseOutput,runEllipseMatching





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
    translated_ellipses.append(Ell('None', 'SLI',p.x, p.y, slider_ell.a, slider_ell.b, slider_ell.r))
    i+=1

translated_ell = translated_ellipses[-1]

print "\nGenerated ellipses:"
for e in translated_ellipses:
    e.show()




""" Generating slide ellipses input file """
sliderInput ='tmp.ell';
try:
    os.remove(sliderInput)
except OSError:
    pass
with open(sliderInput, 'a') as the_file:
    for e in translated_ellipses:
        the_file.write(e.toString());



out_and_error_matlab = runEllipseMatching("matlab",ref_ell_file_input,sliderInput);
results_and_ellipses_matlab = parseOutput("matlab",out_and_error_matlab[0],0)


out_and_error_cplusplus = runEllipseMatching("c++",ref_ell_file_input,sliderInput);
results_and_ellipses_cplusplus = parseOutput("c++",out_and_error_cplusplus[0],0)



results_matlab = results_and_ellipses_matlab[0]
results_cplusplus = results_and_ellipses_cplusplus[0]

ellipses_matlab = results_and_ellipses_matlab[1]
ellipse_cplusplus = results_and_ellipses_cplusplus[1]

# CHECK
print ' \n\n -> number of comparisons: '+str(len(translated_ellipses))
print ' -> number of results found from matlab: '+str(len(results_matlab))
print ' -> number of results found from cplusplus: '+str(len(results_matlab))
print ' -> number of ellipses used from matlab: '+str(len(ellipses_matlab))
print ' -> number of ellipses used from cplusplus: '+str(len(ellipse_cplusplus))
assert len(results_matlab) == len(results_cplusplus) == len(ellipses_matlab) == len(ellipse_cplusplus) == len(translated_ellipses)



for i in range(len(results_matlab)):
    print '\nMatlab: '+results_matlab[i]+' '+ellipses_matlab[i]+'\nC++:    '+results_cplusplus[i]+' '+ellipse_cplusplus[i];



""" Graph """
plot = Plot(2)
plot.setAxisLimits(ref_ell)

""" For Matlab """
plot.addEllToGraph(ref_ell, 0, 'white', 0.5)
plot.addLine(0, [slider_ell.x, translated_ell.x], [slider_ell.y, translated_ell.y])


""" For C++ """
plot.addEllToGraph(ref_ell, 1, 'white', 0.5)
plot.addLine(1, [slider_ell.x, translated_ell.x], [slider_ell.y, translated_ell.y])

alpha = 0.3
if translation_step <= 0.1:
    alpha = 0.3

for i in range(len(translated_ellipses)):
    plot.addEllToGraph(translated_ellipses[i], 0, plot.colors[results_matlab[i]], alpha)
    plot.addEllToGraph(translated_ellipses[i], 1, plot.colors[results_cplusplus[i]], alpha)


plot.show()
