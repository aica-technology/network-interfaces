import setuptools

setuptools.setup(
    name="network_interfaces",
    version="0.2.0",
    description="This package implements network interfaces of AICA",
    maintainer="Dominic Reber",
    maintainer_email="dominic@aica.tech",
    url="https://github.com/aica-technology/network-interfaces",
    packages=setuptools.find_packages(),
    install_requires=[
        "control-libraries>=5.0.0",
    ],
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: GNU General Public License (GPL)",
        "Operating System :: Unix",
    ],
    python_requires='>=3.8',
)
