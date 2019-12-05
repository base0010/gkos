#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := GatekeeperOS
#EXTRA_COMPONENT_DIRS +=/gk_pbrpc/nanopb_cpp/protos/

CFLAGS = -Igk_pbrpc/nanopb_cpp/protos/ -c -Wall
include $(IDF_PATH)/make/project.mk

