# run "python3 setup.py build_ext --inplace" to installing the module

from setuptools import Extension, setup

module = Extension("mysymnmfsp", sources=['symnmf.c','symnmfmodule.c'])
setup(name='mysymnmfsp',
     version='1.0',
     description='Python wrapper for custom C extension',
     ext_modules=[module])
