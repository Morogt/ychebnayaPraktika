QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Global.cpp \
    addclientform.cpp \
    addneworder.cpp \
    clientviewtours.cpp \
    editDBformorders.cpp \
    main.cpp \
    authorization.cpp \
    managerform.cpp \
    tourlist.cpp \
    updclientsform.cpp

HEADERS += \
    Global.h \
    addclientform.h \
    addneworder.h \
    authorization.h \
    clientviewtours.h \
    editDBformorders.h \
    managerform.h \
    tourlist.h \
    updclientsform.h

FORMS += \
    addclientform.ui \
    addneworder.ui \
    authorization.ui \
    clientviewtours.ui \
    editDBformorders.ui \
    managerform.ui \
    tourlist.ui \
    updclientsform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
