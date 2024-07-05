from setuptools import setup, Extension
from pybind11.setup_helpers import Pybind11Extension, build_ext

ext_modules = [
    Pybind11Extension("segment_tree",
        ["segmenttree2d.cpp"],
    ),
]

setup(
    name="segment_tree",
    version="0.0.1",
    author="Author Name",
    author_email="author@example.com",
    description="A 2D Segment Tree implementation",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
)
