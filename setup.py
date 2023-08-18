from setuptools import setup, Extension
import glob
import numpy

superpositeur = Extension('superpositeur',
                    language='c++',
                    include_dirs = ['include/', numpy.get_include()],
                    sources = glob.glob("src/**/*.cpp") + ["python/PythonAPI.cpp"],
                    extra_compile_args=['-std=c++23', '/std:c++latest'],
                )

setup(name = 'superpositeur',
       version = '0.1.0',
       description = 'Superpositeur is a density matrix simulator for quantum operations',
       author = 'Pablo Le Henaff',
       author_email = 'p.lehenaff@tudelft.nl',
       ext_modules = [superpositeur])
