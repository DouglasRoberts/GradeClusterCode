# GradeClusterCode
This repository contains code for looking at grading clusters.  It requires ROOT to be installed on you system in order to run.

To get started, you will first need to load the IRPA-provided data into an SQLite database.
Then, run 'make' to build the shared object library.
Then, in ROOT, load and run DoGradingClusters.C

root [0] .L DoGradingClusters.C+
root [1] CreateSectionsFile()

This will create a root file, Sections.root, that contains the individual section information items.  This is the input to the clustering algorithm.


