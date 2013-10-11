# Chobie storage engine plugin
cmake_minimum_required(VERSION 2.8)

SET(CHOBIE_PLUGIN_STATIC "chobie")
SET(CHOBIE_PLUGIN_DYNAMIC "chobie")

SET(CHOBIE_SOURCES ha_chobie.cc ha_chobie.h)
MYSQL_ADD_PLUGIN(chobie ${CHOBIE_SOURCES} STORAGE_ENGINE MODULE_ONLY)

TARGET_LINK_LIBRARIES(chobie mysys)
