# This file contains the class definition of the dataset 
# Created by:   Liam Graham
# Last Updated: June 2020

import os
import numpy as np
import torch
import torch.utils.data as data_utils
from PIL import Image

class FractalDataset(data_utils.Dataset):
    # The class definition of the dataset - this is not to be
    # confused with the data loader which segments the data into
    # batches
    #
    # NOTE: All images of an N-function IFS were stored in one 
    #       directory along with a datafile containing their
    #       properties. Each fractal was stored as frac{}.png
    #       where {} is its creation number and corresponds to
    #       the line number in the data file. In the data file 
    #       the data was stored in the order of:
    #       - creation number
    #       - number of functions in the IFS
    #       - number of points
    #       - number of pixels corresponding to the fractal
    #       - x coordinate of centroid
    #       - y coordinate of centroid
    #       - standard deviation in the x direction
    #       - standard deviation in the y direction
    #       - fractal dimension estime
    #       - IFS parameters
    #
    # INITIALIZATIONS:
    #     filename:  the name of the datafile
    #     root_dir:  the directory containing the images and datafile
    #     invert:    if 0, load images normally, else, invert the images
    #     transform: a transformation that can be applied to the data as
    #                it is loaded
    
    def __init__(self, filename, root_dir, invert = 0, transform=None):
        fracdata = np.loadtxt(filename)
        numfuncs = int((len(fracdata[0,:])-9)/7)
        self.outputs = fracdata[:, 9:9+6*numfuncs]         
        self.len = len(self.outputs)
        self.root_dir = root_dir
        self.transform = transform
        self.invert = invert
    
    # returns the amount of elements in the dataset
    def __len__(self):
        return self.len
    
    # returns an item linenum of the dataset as a python dictionary
    # containing the image of an attractor and its IFS parameters
    def __getitem__(self, linenum):
        img_name = os.path.join(self.root_dir,
                                "frac{}.png".format(linenum))
        image = (Image.open(img_name)).getdata()
        
        data = self.outputs[linenum, :]
        sample = {'image': image, 'data': data}
        
        if self.transform:
            sample = self.transform(sample, self.invert)
        
        return sample

class ToTensor(object):
    # The class definition of the transform the converts
    # the data to a pytorch tensor object
    
    def __call__(self, sample, invert):
        data = torch.Tensor(sample['data'])
        image = sample['image']
        image = (torch.Tensor(image)).reshape(-1, 640)
        image.unsqueeze_(0)
        
        if (invert != 0):
            image = -1*(image - 255)
        
        return {'data': data,
                'image': image}
