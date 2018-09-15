import setuptools

with open("README", "r") as fh:
    long_description = fh.read()

setuptools.setup(
    name="parse-edid2",
    version="0.1",
    author="paulguy",
    author_email="paulguy119@gmail.com",
    description="EDID parser",
    long_description=long_description,
    long_description_content_type="text/plain",
    url="https://github.com/paulguy/parse-edid2/parse-edid2",
    packages=setuptools.find_packages(),
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: GNU General Public License v3 (GPLv3)",
        "Operating System :: OS Independent",
    ],
)
