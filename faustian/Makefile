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
PKGNAME ?= faustian
PKGVERSION ?= 0.0.1
LIBNAME ?= libfaustian
PROFILE ?= testing
SDKPATH ?= ../er-301

units = DattorroPlusPlus

# Describe your files to this makefile:
headers = $(patsubst %,dsp/%.h,$(units))
sources = $(patsubst %,dsp/%.cpp,$(units)) dsp/faustian.cpp.swig
assets = $(patsubst %,%.lua,$(units)) toc.lua
includes = . ./dsp/

# Do you need any additional preprocess symbols?
symbols = 

#######################################################
# Edits are generally not needed beyond this point.

include $(SDKPATH)/scripts/tutorial.mk

dsp/%.cpp dsp/%.h: dsp/%.dsp
	faust2er301 $<

# Prevent make from deleting these intermidiate files in
# case we'd like to inspect them.
.PRECIOUS: dsp/%.cpp dsp/%.h
