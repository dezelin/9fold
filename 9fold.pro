TEMPLATE = subdirs

SUBDIRS += \
    9foldpatterns \
    9foldgeometry \
    9foldscene \
    9foldfformats \
    9foldmultimedia \
    9foldscripting \
    9foldpython \
    9foldv8 \
    9foldwidgets \
    9fold \
    9foldtests

9foldtests.depends = 9foldwidgets 9foldv8 9foldpython 9foldscripting 9foldmultimedia \
    9foldfformats 9foldscene 9foldgeometry 9foldpatterns

9fold.depends = 9foldwidgets 9foldv8 9foldpython 9foldscripting 9foldmultimedia \
    9foldfformats 9foldscene 9foldgeometry 9foldpatterns

9foldwidgets.depends = 9foldv8 9foldpython 9foldscripting 9foldmultimedia \
    9foldfformats 9foldscene 9foldgeometry 9foldpatterns

9foldv8.depends = 9foldscripting

9foldpython.depends = 9foldscripting

9foldscripting.depends = 9foldscene 9foldgeometry

9foldmultimedia.depends =

9foldfformats.depends = 9foldscene 9foldgeometry 9foldpatterns

9foldscene.depends = 9foldgeometry 9foldpatterns

9foldgeometry.depends =
