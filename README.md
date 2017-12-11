# Ellipse Matching C++ Routine

This software describes the relative positions of two ellipses in a 2D space.

## Utilizzo: 

    ./bin/A./bin/AG_ellipse_matching input1 input2 output

## Inputs:

* input1 e input 2: .ell files that describe one ellipse per row. The ellipses are described according to five parameters:
  * center x
  * center y
  * a axis
  * b axis
  * phi, rotation of the major axis respect to abscissa
* output: il nome del file di output 

## Outputs:
Write on file, for each ellipses position in input1 and for each ellipses position in input2, a description of relative positions. There are different outcomes:
* "is contained in"
* "is contained but tangent to"
* "is equal to"
* "overlap with"
* "contains but tangent to"
* "contains"
* "is external but tangent to"
* "is external to"
* "error"

Example of output file:

    [00001] S31
    [00001] S29 overlap with [00001] S31--3
    [00002] S30 is external to [00001] S31--7
    [00002] S32
    [00001] S29 is external to [00002] S32--7
    [00002] S30 is external to [00002] S32--7
    [00003] S33
    [00001] S29 overlap with [00003] S33--3
    [00002] S30 is external to [00003] S33--7
