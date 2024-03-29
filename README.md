# Nonaffine-Fractals

This project was part of my MSc. Math

When creating fractals using and Iterated Function System (IFS), affine functions are often used.
In this work, I extended the theory to include functions whose derivative is bounded, such as sin,
cos, tan, and any other that you can think of, as well as piecewise contractive functions (ie. 
piecewise functions where each piecewise component is a contractive mapping). This code let's you
create databases of these types of fractals (for bounded derivative functions I simply used sin,
cos, and tan, though you can add others). In my thesis I made small databases of each type as well
as mixtures of each and with affine functions. I found that one can splice two fractals together
using the piecewise functions as well. For example, if one has two separate fractals containing
their respective visual features, one can create a single fractal with features of both. In my 
thesis I argue why this would work best when each IFS does not contain the same amount of
functions, and provide examples of how it does not preserve the features of both to the same 
extent when they contain the same amount.

To run the code, first compile it using the makefile. Then run ./generatedata and input 
specification to create a fractal database to your liking (see rungeneratedata.txt for an example).

Below are some examples of fractals made with:

IFSs consisting of sin, cos, and tan:

<img src = "https://user-images.githubusercontent.com/38572823/194648830-54beaf00-394b-40e4-a78f-c8ac1a8a4ade.png" width = "500" height = "500">

IFSs consisting of piecewise affine functions with a square (top) and circular (bottom) boundaries:

<img src = "https://user-images.githubusercontent.com/38572823/194649051-18f31a2c-dd97-47aa-a717-60da0f626a5a.png" width = "500" height = "340">

Fractal splicing (the two on the left were spliced to make the one on the right):

<img src = "https://user-images.githubusercontent.com/38572823/194649368-f0f15006-5ebf-444c-8c90-ce4ef9e36270.png" width = "500" height = "350">


