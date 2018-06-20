cd EllipseMatchingMatlab
mkdir -p $AGILE/scripts/ellipsedet/new
cp -rf * $AGILE/scripts/ellipsedet/new
cd ..
cd EllipseMatchingC++
make 
make install
cd ..
 
