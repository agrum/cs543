TEMPLATE = app
TARGET = cs543CIC
QT += core \
    xml
HEADERS += src/cr/cCR_ECIC.h \
    src/cr/cCR_CIC.h \
    src/cr/cCR.h \
    src/cNetwork.h \
    src/map/cPath.h \
    src/map/cLink.h
SOURCES += src/cr/cCR_ECIC.cpp \
    src/cr/cCR_CIC.cpp \
    src/cr/cCR.cpp \
    src/map/cPath.cpp \
    src/map/cLink.cpp \
    main.cpp
FORMS += 
RESOURCES += 
