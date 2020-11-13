# This file contains functions pertaining to the neural network model
# Created by:   Liam Graham
# Last Updated: June 2020

import torch
import torch.nn as nn
import numpy as np
import matplotlib.pyplot as plt
from Networks import * # The file containing the network class definition
import torchvision
from PIL import Image
import os

def train(model, train_loader, device, optimizer, criterion, printpct = 5, graphpct = 10, graphheight = 0.3):
    # This function can be used to train a neural network on multiple GPUs
    #
    # INPUTS:
    #     model:        the network object that will be trained
    #     train_loader: the object that loads data into batches
    #     device:       the device that the model will be trained on 
    #                   Note: this function is for multiple GPUs.
    #                         If training on the CPU or a single GPU,
    #                         change all model.module.* instances to 
    #                         model.*
    #     optimizer:    the optimizer that is used in conjunction with
    #                   gradient descent to update network parameters
    #     criterion:    the loss function of the network
    #     printpct:     the percentage frequency that a loss update is
    #                   printed. Eg., 5 prints every 5% completion of 
    #                   an epoch
    #     graphpct:     the percentage frequency that a loss update is
    #                   graphed. Eg., 10 graphs the loss every 10%
    #                   completion of an epoch
    #     graphheight:  the height of the graph of the loss printed  
    #                   whenever graphpct of an epoch is complete

    model.train()
    total_step = len(train_loader)
    print ("Beginning Training")
    model.module.total_epochs += 1
    for i, sample in enumerate(train_loader):
        # Move batch to the device
        images = sample['image'].to(device)
        labels = sample['data'].to(device)
        
        # Feeding the input forward 
        outputs = model(images)
        loss = criterion(outputs, labels)
        model.module.losses = np.append(model.module.losses, loss.item())
            
        # Backpropagation
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()
           
        # print an update every printpct percentage completion of an epoch
        if (i+1) % int(printpct*total_step/100) == 0:
            print ('Step [{}/{}], Loss: {:.4f}' 
                   .format(i+1, total_step, loss.item())) 
        
        # graph the loss every graphpct percentage completion of an epoch
        if (i+1) % int(graphpct*total_step/100) == 0:
            # Note: the division by 2500 is due to a dataset of size 250,000
            #       with a step size of 100
            xs = np.linspace(0, len(model.module.losses)/2500, len(model.module.losses))
            plt.plot(xs, model.module.losses)
            plt.ylim(0,graphheight)
            plt.grid(True)
            plt.show()
    print ("Epoch Complete!")
    
def test(model, test_loader, device, batch_size, test_items = 10000):
    #This function can be used to test a neural network on a dataset
    #
    # INPUTS:
    #     model:       the network object that will be tested
    #     test_loader: the object that loads into batches
    #     device:      the device that the model will be tested on 
    #                  Note: this function is for multiple GPUs.
    #                        If training on the CPU or a single GPU,
    #                        change all model.module.* instances to 
    #                        model.*
    #     batch_size:  the batch size of the data loader
    #     test_items:  the number of elements in the test set

    model.eval() # this is to switch batch normalization from moving
                 # average to total average
    test_total = test_items * model.module.num_classes
    total_test_step = len(test_loader)    
    with torch.no_grad():
        acc1, acc2, acc3, acc4, acc5 = 0, 0, 0, 0, 0
        tol1, tol2, tol3, tol4, tol5 = 0.1, 0.2, 0.3, 0.4, 0.5
        for i, sample in enumerate(test_loader):
            # move batch to device 
            image = sample['image'].to(device)
            data = sample['data'].to(device)
            
            # feed the data forward
            outputs = model(image)
            
            # calculate the difference between the true values and outputs
            difs = (abs(outputs[:,:] - data[:,:]))
            acc1 += (difs < tol1).sum().item()
            acc2 += (difs < tol2).sum().item()
            acc3 += (difs < tol3).sum().item()
            acc4 += (difs < tol4).sum().item()
            acc5 += (difs < tol5).sum().item()
            
            #print percentage completion
            if (i+1) % 1 == 0:
                print ('Step [{}/{}]'
                       .format(i+1, total_test_step), end = "\r")
    
    #append to model accuracy attribute
    model.module.Accuracies[0].append(100*acc1/test_total)
    model.module.Accuracies[1].append(100*acc2/test_total)
    model.module.Accuracies[2].append(100*acc3/test_total)
    model.module.Accuracies[3].append(100*acc4/test_total)
    model.module.Accuracies[4].append(100*acc5/test_total)
    print ("Within {}:\t{:.2f}%  [{}/{}]".format(tol1, 100*acc1/test_total, acc1, test_total))
    print ("Within {}:\t{:.2f}%  [{}/{}]".format(tol2, 100*acc2/test_total, acc2, test_total))
    print ("Within {}:\t{:.2f}%  [{}/{}]".format(tol3, 100*acc3/test_total, acc3, test_total))
    print ("Within {}:\t{:.2f}%  [{}/{}]".format(tol4, 100*acc4/test_total, acc4, test_total))
    print ("Within {}:\t{:.2f}%  [{}/{}]".format(tol5, 100*acc5/test_total, acc5, test_total))

def save(model, learning_rate, optimizer, device, filename):
    #This function can be used to save a neural network
    #
    # INPUTS:
    #     model:         the network object that will be tested
    #     learning_rate: the current learning rate used
    #     optimizer:     the optimizer that is used in conjunction with
    #                    gradient descent to update network parameters
    #     device:        the device the model is currently on
    #     filename:      the name of the file the model will be saved as
    #                    Note: a common file extension type is .tar
    
    cpudevice = torch.device('cpu')
    model.to(cpudevice)
    state = {
        'losses': model.losses,
        'num_classes': model.num_classes,
        'Accuracies': model.Accuracies,
        'learning rate': learning_rate,
        'total epochs': model.total_epochs,
        'dropout': model.dropout,
        'state_dict': model.state_dict(),
        'optimizer': optimizer.state_dict(),
    }
    torch.save(state, filename)
    model.to(device)
    
def load(filename device, chunks, ksizes, psizes, scale_lr = 1):
    # This function can be used to load a neural network
    #
    # INPUTS:
    #     filename: the name of the file the model will be saved as
    #     device:   the device the model will be loaded to
    #               Note: this function is for multiple GPUs.
    #                     If training on the CPU or a single GPU,
    #                     change all model.module.* instances to 
    #                     model.*
    #     The following must match that of the saved model:
    #     chunks:   a list of size 7 representing the chunk sizes
    #     ksize:    a list of size 7 representing the kernel size 
    #               for the layer in each chunk
    #               note: ksize should only be odd numbers
    #     psize:    a list of size 7 representing the kernel size 
    #               in each pooling layer
    #               note: psize should only be even numbers
    #     
    #     scale_lr: the factor by which the learning rate will be
    #               scaled
     
    checkpoint = torch.load(filename, map_location = 'cpu')
    num_classes = checkpoint['num_classes']
    dropout = checkpoint['dropout']
    model = FracNet(num_classes, dropout, chunks, ksizes, psizes)
    if torch.cuda.device_count() > 1:
        model = nn.DataParallel(model)
    model.to(device)
    learning_rate = checkpoint['learning rate']*scale_lr
    optimizer = torch.optim.Adam(model.parameters(), lr = learning_rate)
    model.module.losses = checkpoint['losses']
    model.module.Accuracies = checkpoint['Accuracies']
    model.module.total_epochs = checkpoint['total epochs']
    model.module.load_state_dict(checkpoint['state_dict'])
    optimizer.load_state_dict(checkpoint['optimizer'])
    optimizer.param_groups[0]['lr']*= scale_lr
    print ("Model loaded succesfully:\tEpoch:",model.module.total_epochs,"loaded onto", device)
    return model, optimizer, learning_rate
    
