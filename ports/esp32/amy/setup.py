from distutils.core import setup, Extension
import glob
import os
# the c++ extension module
sources = glob.glob("*.c")
os.environ["CC"] = "gcc"
os.environ["CXX"] = "g++"

extension_mod = Extension("libamy", sources=sources, extra_link_args=["-lsoundio", "-lpthread"])

setup(name = "libamy", 
	ext_modules=[extension_mod])