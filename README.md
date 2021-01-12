# Fractals And Neural Networks

This project was part of my masters work. The problem is: Given an image of a fractal created from an IFS of affine functions, can we predict the parameters of the IFS to recreate it. There are many additional functions in this pertaining to another part of my project, which was to develop theory for fractals made from IFSs of nonaffine functions. 

To run the code:
run compile.sh to generate the generatedata file. Then run generatedata and input specification to create a fractal database with your specifications (see rungeneratedata.txt for an example). One can then run trainmodel.py to train a neural network on the dataset.
