TEMPLATE = app
TARGET = hub

# HUB TIER REPRESENTS AN OPTIONAL CENTRAL COMPONENT THAT TAKES CARE OF HEAVY LIFTING
# ON BEHALF OF THE AGENT (ROBOT) IN CASES WHERE IT IS NOT FEASIBLE TO RUN ALL THIS
# CODE DIRECTLY ON THE ROBOT.
# THIS IS USEFUL WHEN CODE IS HEAVY AND/OR YOU WANT MORE DIRECT DEBUGGING CAPABILITIES


include($$TOP_PWD/common.pri)
include($$TOP_PWD/lib/inc.pri)
include($$TOP_PWD/lib/ext.pri)

HEADERS += \
	HubMain.hpp

SOURCES += \
	HubMain.cpp

message("FROM hub.pro:")
include($$TOP_PWD/status.pri)
