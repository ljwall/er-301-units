# How to use this makefile
# 
# To compile and create the pkg using the default PROFILE and ARCH.
# > make 
#
# Remove build outputs for the default PROFILE and ARCH.
# > make clean
#
# Copy the pkg file to the packages folder
# > make install
#
# To cross-compile for the ER-301 hardware, add ARCH=am335x the above examples.
#
# To create a release version for the am335x:
# make PROFILE=release ARCH=am335x
#
# Remove build outputs for all profiles and architectures.
# > make dist-clean
#
# WARNING: In order to keep this makefile simple, dependencies are hard-coded 
# rather than auto-generated.

# Override these defaults on the commandline:
PKGNAME ?= bloscillators
PKGVERSION ?= 0.0.1
LIBNAME ?= libbloscillators
PROFILE ?= testing
SDKPATH ?= ../er-301

# Describe your files to this makefile:
headers = $(wildcard lib/*.h) $(wildcard objects/*.h)
sources = $(wildcard lib/*.cpp) $(wildcard objects/*.cpp) $(wildcard objects/*.cpp.swig)
assets = $(wildcard *.lua)
includes = . ./objects/

# Do you need any additional preprocess symbols?
symbols = 

#######################################################
# Edits are generally not needed beyond this point.

include $(SDKPATH)/scripts/tutorial.mk
