#-------------------------------------------------
#
# Project created by QtCreator 2017-07-18T14:35:51
#
#-------------------------------------------------

QT += core gui widgets opengl network
TEMPLATE = app
CONFIG += c++11

TARGET = perception

INCLUDEPATH += managers/vision-manager \
               managers/feedback-manager \
               managers/ai-manager \
               entities \
               utils \
	       config

INCLUDEPATH += ../furgbol-core/include \
               ext/json

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp\
           mainwindow.cpp \
           entities/robot.cpp \
           entities/ball.cpp \
           managers/feedback-manager/feedbackManager.cpp \
           managers/vision-manager/visionManager.cpp \
           entities/teamRobot.cpp \
           entities/enemyRobot.cpp \
           utils/clock.cpp \
	   config/config.cpp \
    	   system.cpp \
           managers/ai-manager/aiManager.cpp \
           networkconfigdialog.cpp \
           geometryconfigdialog.cpp \
           glmonitorwidget.cpp \
           controlconfigdialog.cpp \
           managers/feedback-manager/serialFeedback.cpp

HEADERS += mainwindow.h \
           entities/robot.h \
           entities/ball.h \
           managers/feedback-manager/feedbackManager.h \
           managers/vision-manager/visionManager.h \
           entities/teamRobot.h \
           entities/enemyRobot.h \
	   utils/definitions.h \	
           utils/clock.h \
	   config/config.h \
    	   system.h \
           managers/ai-manager/aiManager.h \
           networkconfigdialog.h \
           geometryconfigdialog.h \
           glmonitorwidget.h \
           controlconfigdialog.h \
           managers/feedback-manager/serialFeedback.h

FORMS    += mainwindow.ui \
    networkconfigdialog.ui \
    geometryconfigdialog.ui \
    controlconfigdialog.ui

RESOURCES += \
    resources.qrc

LIBS += -lGLU -lprotobuf -lboost_system -pthread `pkg-config --libs opencv`
#LIBS += -L../proto-lib -lproto
LIBS += -L../furgbol-core/lib -lssl-vision-proto -lssl-refbox-proto -lfurgbol-proto
