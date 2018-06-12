# Ellipse Matching Repository
This repo holds 4 folders:
  * the Ellipse Matching C++ software
  * the Ellipse Matching Matlab2.0 software
  * the Ellipse Matching Matlab2.0 software with debugging outputs
  * the Ellipse Matching Testing software

The Ellipse Matching C++ is the C++ version of Ellipse Matching Matlab2.0

The document that describes the software validation is available at https://docs.google.com/document/d/1UFLx4ldzWHRycoPHMGS8Zzjf2K_VCNsebmEMK4Vym0w/edit?usp=sharing

# Ellipse Matching

This software describes the relative positions of two ellipses in a 2D space.

## Warning
Two ellipses that have the same rotation (less than a threshold value) could lead to a incorrect output. Hence, one of the two ellipses will be rotated by 1 degree in order to fix the problem.  

## Compilation (C++ version only)

For debugging purpose use:
   * export DEBUG=1
   * make

For production use:
   * make

## Usage:
### C++
    * ./bin/AG_ellipse_matching input1 input2 output
### Matlab
    * octave ELLIPSE_MATCHING.m input1 input2 output


## Inputs:

* input1 e input 2: files with extension .ell that describe one ellipse per row. The ellipses are described according to six parameters:
  * the name
  * center x
  * center y
  * a semi-axis
  * b semi-axis
  * phi, rotation (degrees) of the major axis respect to abscissa
  * output: il nome del file di output

Example of input1.ell file:

    S31 212.793 -45.3849 0.557728 0.642922 -17.5603
    S32 212.793 225.3849 0.557728 0.642922 0.0
    S33 212.793 -45.3849 0.557728 0.642922 -17.5603

Example of input2.ell file:

    S29 212.86 -45.4904 0.677285 0.481918 50.7225
    S30 212.86 -25.4904 0.677285 0.481918 50.7225



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





# Ellipse Matching Testing

Two types of test scripts are present.

## Slider Ellipse Test
The script takes in input two ellipses and translation parameters. The first ellipse is fixed, the second is translated by the translation parameters. At each step of the translation, the Matlab and C++ version of EllipseMatching software is run. At the end of the translation two plots are being shown, reporting the EllipseMatching output for each version.

### Usage:
python slider_ellipse_test.py <reference ellipse file> <slider ellipse file> <translation_magnitude> <direction> <step_size>

* reference ellipse file: must contain only one ellipse in the Ellipse Matching format.
* slider ellipse file: must contain only one ellipse in the Ellipse Matching format.
* translation_magnitude: the lenght of the translation
* direction: possible values: {N,S,O,E,NO,NE,SO,SE}
* step_size: at each step, the second ellipse is translated by step_size

### Example:
python slider_ellipse_test.py input_test/test_14_ref_ell.ell input_test/test_14_slider_ell.ell 1.5 O 0.1

## Ellipse Unit Test
The script takes in input three files. The first file must contains one ellipse. The second file must contains one or more ellipses. The third file must contains the correct labels describing the relative position of the ellipse in first file with the position of the ellipses in the second file. The Matlab and C++ version of EllipseMatching software are run. At the end of the programs two plots are being shown, reporting the EllipseMatching output for each version.

### Usage:
python slider_ellipse_test.py <reference ellipse file> <unit test ellipses> <unit test ellipses labels>

* reference ellipse file: must contain only one ellipse in the Ellipse Matching format.
* slider ellipse file: must contain one or more ellipses in the Ellipse Matching format.
* relative positions correct labels: must contain the correct relative positions labels.

### Example:
python unit_test.py input_test/test_unit_ref.ell input_test/test_unit_slider.ell input_test/test_unit_labels.ell
