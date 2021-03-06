# Include helper for external libs

# TOP_PWD and TOP_BUILD are defined in .qmake.config in project root

# EXAMPLE NON-OS EXTERNAL DEPENDENCY
contains(DEFINES, EXTERNAL_LIB_QRENCODE){
EXT = $$TOP_PWD/ext
LIBS +=         -L$$EXT/qrencode-3.4.3/.libs -lqrencode_ww
INCLUDEPATH +=    $$EXT/qrencode-3.4.3
DEPENDPATH +=     $$EXT/qrencode-3.4.3/.libs
PRE_TARGETDEPS += $$EXT/qrencode-3.4.3/.libs/libqrencode_ww.a
}

#This is used for speech output
contains(DEFINES, EXTERNAL_LIB_ESPEAK){
LIBS +=         -lespeak
INCLUDEPATH +=    /usr/include/espeak
}


# This is used for OpenCV
contains(DEFINES, EXTERNAL_LIB_OPENCV){
LIBS +=         -lopencv_core -lopencv_imgproc -lopencv_objdetect -lopencv_highgui
INCLUDEPATH +=    /usr/include/opencv
}



# This is used for OpenCL
contains(DEFINES, EXTERNAL_LIB_OPENCL){
#INCLUDEPATH+="./OpenCL-ICD-Loader/inc"
#LIBS+= -L./OpenCL-ICD-Loader/build/bin
INCLUDEPATH+="/usr/local/include/CL"
LIBS+= -L$$TOP_PWD/libs/libclt/OpenCL-ICD-Loader/build/bin
LIBS+= -lOpenCL
PRE_TARGETDEPS += $$TOP_PWD/libs/libclt/OpenCL-ICD-Loader/build/bin/libOpenCL.so
}


#LIBS += -lusb

