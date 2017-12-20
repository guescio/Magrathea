#-------------------------------------------------DERS
#
# Project created by QtCreator 2017-10-07T07:29:37
#
#-------------------------------------------------

QT += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Magrathea
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        magrathea.cpp \
        MotionHandler.cpp \
        Gantry_Vancouver.cpp \
    PetalAlgManager.cpp \
    gluehandler.cpp

HEADERS += \
        magrathea.h \
        MotionHandler.h \
        Gantry_Vancouver.h \
    PetalAlgManager.h \
    gluehandler.h \
    gantry.h

FORMS += \
        magrathea.ui


#DEFINES -= UNICODE

INCLUDEPATH += "C:\Program Files (x86)\Aerotech\A3200\CLibrary\Include"
INCLUDEPATH += "C:\Program Files (x86)\Aerotech\A3200\CLibrary\Bin"
INCLUDEPATH += "C:\Program Files (x86)\Aerotech\A3200\CLibrary\Bin64"

##win32:
LIBS += -L'C:/Program Files (x86)/Aerotech/A3200/CLibrary/Lib/' -lA3200C

#INCLUDEPATH += 'C:/Program Files (x86)/Aerotech/A3200/CLibrary/Lib'
#DEPENDPATH += 'C:/Program Files (x86)/Aerotech/A3200/CLibrary/Lib'

#PRE_TARGETDEPS += $$PWD/'../../../../../Program Files (x86)/Aerotech/A3200/CLibrary/Lib/A3200C.lib'

##win32:!win32-g++: PRE_TARGETDEPS += $$PWD/'../../../../../Program Files (x86)/Aerotech/A3200/CLibrary/Lib/A3200C.lib'
##else:win32-g++: PRE_TARGETDEPS += $$PWD/'../../../../../Program Files (x86)/Aerotech/A3200/CLibrary/Lib/libA3200C.a'

##LIBS += 'C:/Program Files (x86)/Aerotech/A3200/CLibrary/Bin/A3200C.dll'
##LIBS += 'C:/Program Files (x86)/Aerotech/A3200/CLibrary/Bin/A32Cmplr.dll'
##LIBS += 'C:/Program Files (x86)/Aerotech/A3200/CLibrary/Bin/A32Sys.dll'
##LIBS += 'C:/Program Files (x86)/Aerotech/A3200/CLibrary/Bin/AerUtilities.dll'
##LIBS += 'C:/Program Files (x86)/Aerotech/A3200/CLibrary/Bin/LicenseDecoder.dll'

#HEADERS += 'C:/Program Files (x86)/Aerotech/A3200/CLibrary/Include/A3200.h'
