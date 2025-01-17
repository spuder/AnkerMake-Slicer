# File for compiling just meshlab mini (without the external libraries!)
#
# MeshLab qmake config uses the following variables:
#
# MESHLAB_SOURCE_DIRECTORY: the directory where is placed the main meshlab.pro
# MESHLAB_BUILD_DIRECTORY: the directory where the meshlab build files are placed
# MESHLAB_DISTRIB_DIRECTORY: the directory that will contain all the files necessary
#                            for a portable version (after build and deploy)
#                            (if shadow build, will be MESHLAB_BUILD_DIRECTORY/distrib)
# MESHLAB_EXTERNAL_DIRECTORY: the directory where external libraries are placed
#

TEMPLATE = subdirs

message("Anker Make")
message("DISTRIB_DIRECTORY: "$$MESHLAB_DISTRIB_DIRECTORY)

SUBDIRS += common
common.subdir = common



#SUBDIRS += anker_plugin_demo
#anker_plugin_demo.subdir = ankerplugins/anker_plugin_demo
#anker_plugin_demo.depends = common
SUBDIRS += anker_edit_model
anker_edit_model.subdir = ankerplugins/anker_edit_model
anker_edit_model.depends = common

SUBDIRS += fdm_setting
fdm_setting.subdir = ankerplugins/fdm_setting
fdm_setting.depends = common

SUBDIRS += fdm_slicer
fdm_slicer.subdir = ankerplugins/fdm_slicer
fdm_slicer.depends = common

# fdm_gcode_parser plugin
SUBDIRS += fdm_gcode_parser
fdm_gcode_parser.subdir = ankerplugins/fdm_gcode_parser
fdm_gcode_parser.depends = common

SUBDIRS += AnkerMake
AnkerMake.subdir = meshlab
AnkerMake.depends = common anker_edit_model fdm_gcode_parser fdm_setting fdm_slicer

# if distrib folder is not in $$PWD/../distrib (shadow build case),
# we need to copy all the files inside $$PWD/../distrib in the actual
# distrib folder ($$OUT_PWD/distrib or $$MESHLAB_DISTRIB_DIRECTORY)




!equals(PWD, $$OUT_PWD) : !equals(PWD, $$OUT_PWD/src) {
    #copying the "lib" folder inside the $$OUT_PWD/distrib

    win32:copydir.commands = $(COPY_DIR) \"$$shell_path($$PWD/../distrib)\" \"$$shell_path($$OUT_PWD/distrib)\"
    !win32:copydir.commands = $(COPY_DIR) \"$$shell_path($$PWD/../distrib)\" \"$$shell_path($$OUT_PWD)\"
    first.depends += $(first) copydir
    export(first.depends)
    export(copydir.commands)

    QMAKE_EXTRA_TARGETS += first copydir
}


TRANSLATIONS += meshlab/language/tr_zh.ts\
                meshlab/language/tr_en.ts\
                meshlab/language/tr_ja.ts
