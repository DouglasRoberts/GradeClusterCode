# GradeClusterCode
This repository contains code for looking at grading clusters.  It requires ROOT to be installed on you system in order to run.

To get started, you will first need to create a symbolic link called 'data' pointing to the location of the raw data files.

Then, in ROOT, load and run RawToRoot.C

root [0] .L RawToRoot.C+
root [1] RawToRoot()

This will translate the raw data into ROOT TTrees, and auto-generate header files for these trees.

Next, use make to build the shared object library.

