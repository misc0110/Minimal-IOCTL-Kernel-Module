# Minimal IOCTL Kernel Module

A minimal kernel module with one simple `ioctl` call. 

## Usage
Build the sample application by running `make`.
Build the kernel module by running `make` in the `module/` directory and load it into the kernel using `sudo insmod sample.ko`. 
Run the sample application:

    % ./example 
    Val (before): 3
    Val (after):  4

