import sys
import warnings
from glob import glob

import pkgconfig
from pybind11.setup_helpers import ParallelCompile, Pybind11Extension, build_ext, naive_recompile
from setuptools import setup

__version__ = "0.0.3"

try:
    status = pkgconfig.installed('communication_interfaces', f'>= {__version__}')
    if not status:
        raise Exception('Could not find library communication_interfaces')
except Exception as e:
    warnings.warn(f'Error with dependencies: {e.args[0]}')
    sys.exit(1)

ParallelCompile('NPY_NUM_BUILD_JOBS', needs_recompile=naive_recompile).install()

ext_modules = [
    Pybind11Extension('communication_interfaces',
        glob('src/*.cpp'),
        cxx_std=17,
        include_dirs=['include'],
        libraries=['communication_interfaces'],
        define_macros = [('MODULE_VERSION_INFO', __version__)],
        ),
]

setup(
    name='communication-interfaces',
    version=__version__,
    author='Dominic Reber',
    author_email='dominic@aica.tech',
    url='https://github.com/aica-technology/network-interfaces',
    description='Python bindings for the C++ communication interfaces',
    ext_modules=ext_modules,
    extras_require={"test": "pytest"},
    cmdclass={"build_ext": build_ext},
    python_requires='>=3',
    install_requires=[
        'pyzmq>=22.3'
    ],
    license='GNU GPL v3',
    zip_safe=False,
)