TEMPLATE = subdirs

 SUBDIRS = \
    Examples \
    libQt6Rockchip


 # where to find the sub projects - give the folders
 libQt6Rockchip.subdir = libQt6Rockchip
 Examples.subdir  = examples/test

 # what subproject depends on others
 Examples.depends = libQt6Rockchip
