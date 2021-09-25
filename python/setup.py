import setuptools

setuptools.setup(
    name="network_interfaces",
    version="0.1.0",
    description="This package implements network interfaces of AICA",
    url="https://github.com/aica-technology/network-interfaces",
    packages=setuptools.find_packages(),
    install_requires=[
        "control-libraries>=4.0.0",
    ],
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: GNU General Public License (GPL)",
        "Operating System :: Unix",
    ],
    python_requires='>=3.8',
)
