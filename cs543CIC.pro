TEMPLATE = app
TARGET = cs543CIC
QT += core \
    xml
HEADERS += src/traffic/cRequest.h \
    src/traffic/cTraffic.h \
    src/cr/cCR_CIC.h \
    src/map/cMap.h \
    src/cr/cCR.h \
    src/cNetwork.h \
    src/map/cPath.h \
    src/map/cLink.h
SOURCES += src/traffic/cRequest.cpp \
    src/traffic/cTraffic.cpp \
    src/cr/cCR_CIC.cpp \
    src/map/cMap.cpp \
    main.cpp \
    src/cr/cCR.cpp \
    src/map/cPath.cpp
FORMS += 
RESOURCES += 
