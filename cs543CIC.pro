TEMPLATE = app
TARGET = cs543CIC
QT += core \
    xml
HEADERS += src/cr/cCR_TMP.h \
    src/map/cMap.h \
    src/cr/cCR.h \
    src/cNetwork.h \
    src/map/cPath.h \
    src/map/cLink.h
SOURCES += src/cr/cCR_TMP.cpp \
    src/map/cMap.cpp \
    main.cpp \
    src/cr/cCR.cpp \
    src/map/cPath.cpp
FORMS += 
RESOURCES += 
