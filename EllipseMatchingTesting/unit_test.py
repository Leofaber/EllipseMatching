import sys
import subprocess
from test_utils import Point,Ell,GeomEngine,Plot,createEllipsesFromFile,parseOutput,runEllipseMatching

# python unit_test.py input_test/test_unit_ref.ell  input_test/test_unit_slider.ell input_test/test_unit_labels.ell

def checkAndVerify(unit_test_ells,results,ellipses):
    print '\n -> number of comparisons: '+str(len(unit_test_ells))
    print ' -> number of results found: '+str(len(results))
    assert len(ellipses) == len(results) == len(unit_test_ells)
    print '\n\n ####### UNIT TESTs ########'

    """ VERIFY TEST """
    count_passed = 0
    tot =  len(unit_test_ells)
    for i in range(len(results)):
        if results[i] == labels[i]:
            print ellipses[i].split()[0]+' '+results[i]+' '+ref_ell.toString().split()[0]+' [OK]';
            count_passed += 1
        else:
            print ellipses[i].split()[0]+' '+results[i]+' '+ref_ell.toString().split()[0]+' * WRONG * ('+labels[i]+')';
    print "Passed -> "+str(count_passed)+' / '+str(tot)



ref_ell_file_input = sys.argv[1]
slider_ell_file_input = sys.argv[2]
label_ell_file_input = sys.argv[3]

print  sys.argv[1:]

ref_ell = Ell(ref_ell_file_input)
ref_ell2 = Ell(ref_ell_file_input)

ref_ell.show()
unit_test_ells = createEllipsesFromFile(slider_ell_file_input)

for el in unit_test_ells:
    el.show()


""" Processing label file """
with open(label_ell_file_input) as label_file:
    labels = label_file.readlines()

labels = [x.strip() for x in labels]
#print labels


""" Execution of the Matlab program """
out_and_error_matlab = runEllipseMatching("matlab",ref_ell_file_input,slider_ell_file_input);
results_and_ellipses_matlab = parseOutput("matlab",out_and_error_matlab[0],3)

results_matlab = results_and_ellipses_matlab[0]
ellipses_matlab = results_and_ellipses_matlab[1]

checkAndVerify(unit_test_ells, results_matlab, ellipses_matlab)



""" Execution of the C++ program """
out_and_error_cplusplus = runEllipseMatching("c++",ref_ell_file_input,slider_ell_file_input);
results_and_ellipses_cplusplus = parseOutput("c++",out_and_error_cplusplus[0],3)

results_cpp = results_and_ellipses_cplusplus[0]
ellipses_cpp = results_and_ellipses_cplusplus[1]

checkAndVerify(unit_test_ells, results_cpp, ellipses_cpp)







""" Graph """
plot = Plot(2)
plot.setAxisLimits(ref_ell)
plot.setFigureTitles(['Ellipse Matching Matlab','Ellipse Matching C++'])

plot.addEllToGraph(ref_ell, 0, 'white', 0.5)
plot.addEllToGraph(ref_ell, 1, 'white', 0.5)


for i in range(len(unit_test_ells)):
    plot.addEllToGraph(unit_test_ells[i], 0, plot.colors[results_matlab[i]], 0.3)
    plot.addEllToGraph(unit_test_ells[i], 1, plot.colors[results_cpp[i]], 0.3)



plot.show()
