include($$TOP_PWD/test/test.pri)

TARGET = test_ardumy

HEADERS += \
	TestArduMY.hpp \
	../../libs/arduino/MagicDetector.hpp \
	../../libs/arduino/CommandParser.hpp \
	../../libs/arduino/Actuator.hpp \
    ../../libs/arduino/ActuatorConfigParser.hpp \
    ../../libs/arduino/ActuatorValueParser.hpp


SOURCES += \
	TestArduMY.cpp \
	../../libs/arduino/MagicDetector.cpp \
	../../libs/arduino/CommandParser.cpp \
	../../libs/arduino/Actuator.cpp \
    ../../libs/arduino/ActuatorConfigParser.cpp \
    ../../libs/arduino/ActuatorValueParser.cpp

include($$TOP_PWD/status.pri)
include($$TOP_PWD/test/test_autorun.pri)

