# Ellipse Matching C++ Routine

Il software verifica la posizione di due ellissi 

## Utilizzo: 

    ./bin/A./bin/AG_ellipse_matching input1 input2 output

## Inputs:

* input1 e input 2: due files con estensione .ell che riportano un'ellisse per ogni riga descritta secondo 5 parametri:
  * center x
  * center y
  * a axis
  * b axis
  * phi, rotation of the major axis respect to abscissa
* output: il nome del file di output 

## Outputs:
For each ellipses position in input1 and for each ellipses position in input2, there are different outcomes:
* "is contained in"
* "is contained but tangent to"
* "is equal to"
* "overlap with"
* "contains but tangent to"
* "contains"
* "is external but tangent to"
* "is external to"
* "error"
