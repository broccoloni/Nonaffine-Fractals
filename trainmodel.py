# This file contains can be used to load data, create a network, train the model, and save it
# Created by:   Liam Graham
# Last Updated: June 2020

import os
import torch
import torch.nn as nn
import torchvision
import torchvision.transforms as transforms
from PIL import Image
import torch.utils.data as data_utils
import matplotlib.pyplot as plt
import numpy as np
from FracDataset import FractalDataset, ToTensor # The file containing the Fractal dataset class definition
from Networks import *       # The file containing the network class definition
from Networkfuncs import *   # The file containing the network function such as train, test, etc.

if __name__ == "__main__":
    batch_size  = 100
    num_workers = 6    # The number of cpus used to load data

    # Loading the Fractal dataset
    # This file will load the training set from the directory 
    # /home/lgraha07/scratch/Fraclibnf2/
    #
    # The validation set from
    # /home/lgraha07/scratch/ValidFraclibnf2/
    #
    # and the test set from 
    # /home/lgraha07/scratch/TestFraclibnf2/
    
    dirname = "Fraclibnf2"
    testdirname = "TestFraclibnf2"
    validdirname = "ValidFraclibnf2"

    path = "/home/lgraha07/scratch/"

    dirpath = path+dirname
    testdirpath = path+testdirname
    validdirpath = path+validdirname

    filename = "/fracdata.dat"
    filepath = dirpath+filename
    testfilepath = testdirpath+filename
    validfilepath = validdirpath+filename

    train_set = FractalDataset(filename = filepath,
                                root_dir = dirpath,   
                                transform = ToTensor())
    test_set = FractalDataset(filename = testfilepath,
                               root_dir = testdirpath,
                               transform = ToTensor())
    valid_set = FractalDataset(filename = validfilepath,
                                 root_dir = validdirpath,
                                 transform = ToTensor())

    # Put each dataset into a pytorch data loader which divides it into batches
    frac_train_loader = data_utils.DataLoader(train_set,
                                              batch_size = batch_size,
                                              shuffle = True,
                                              num_workers = num_workers)
    frac_test_loader = data_utils.DataLoader(test_set,
                                             batch_size = batch_size,
                                             shuffle = False,
                                             num_workers = num_workers)
    frac_valid_loader = data_utils.DataLoader(valid_set,
                                             batch_size = batch_size,
                                             shuffle = False,
                                             num_workers = num_workers)

    # Initializations and Hyperparameters
    device = torch.device('cuda:0' if torch.cuda.is_available() else 'cpu')
    num_classes = train_set[0]['data'].size(0)
    dropout = 0
    learning_rate = 0.0000075
    chunks = [3,5,7,11,29,23,17]
    ksizes = [3,3,3,3,3,3,3]
    psizes = [4,4,4,4,4,4,4]
    betas = (0.9,0.999)
    num_epochs = 10
    
    # Create the model
    model = FracNet(num_classes, dropout, chunks, ksizes, psizes)
    if torch.cuda.device_count() > 1:
        model = nn.DataParallel(model)
    model.to(device)
    optimizer = torch.optim.Adam(model.parameters(), lr = learning_rate, betas = betas, weight_decay = 0.01)
    
    # Train, test and save the model. Note: during training it is tested on the validation set
    # Once all training is complete is when it is tested on the test set
    for i in range(num_epochs):
        train(model, frac_train_loader, device, optimizer, criterion = nn.MSELoss())
        test(model, frac_val_loader, device, batch_size)
        modelname = "modelnf2_"+str(i)+".tar"
        save(model, learning_rate, optimizer, device, modelname)
        
    # To go back and load epoch 8 and scale the learning rate by 1/10, we could then run
    model, optimizer, learning_rate = load("modelnf2_8.tar", device, chunks, ksizes, psizes, scale_lr = 1/10.)
    
        