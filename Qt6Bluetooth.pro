TEMPLATE = subdirs

 SUBDIRS = \
    EXAMPLES \
    SRC


 # where to find the sub projects - give the folders
 SRC.subdir = src
 EXAMPLES.subdir  = examples/test

 # what subproject depends on others
 EXAMPLES.depends = SRC
