QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

#CONFIG += sanitizer
#CONFIG += sanitize_address
#CONFIG += sanitize_memory

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = mylocker
TEMPLATE = app

# No debug output in release mode
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

# Print if this is a debug or release build
CONFIG(debug, debug|release) {
    message("This is a debug build")
} else {
    message("This is a release build")
}


APPLICATION_FULLNAME = MyLocker
DEFINES += APPLICATION_FULLNAME=\\\"$${APPLICATION_FULLNAME}\\\"

# Set program version
VERSION = 3.0
DEFINES += VERSIONSTR=\\\"$${VERSION}\\\"


# Define git info
GIT_HASH="\\\"$$system(git -C \""$$_PRO_FILE_PWD_"\" rev-parse --short HEAD)\\\""
GIT_BRANCH="\\\"$$system(git -C \""$$_PRO_FILE_PWD_"\" rev-parse --abbrev-ref HEAD)\\\""
BUILD_TIMESTAMP="\\\"$$system(date -u +\""%Y-%m-%dT%H:%M:%SUTC\"")\\\""
DEFINES += GIT_HASH=$$GIT_HASH GIT_BRANCH=$$GIT_BRANCH BUILD_TIMESTAMP=$$BUILD_TIMESTAMP

# Link OpenSSL
LIBS += -lssl -lcrypto

# Include OpenSSL headers
INCLUDEPATH += /usr/include
LIBS += -L/usr/lib

SOURCES += \
    categories/categorymanager.cpp \
    login/autolockmanager.cpp \
    password/addpassworddialog.cpp \
    password/passwordcard.cpp \
    settings/settingsmanager.cpp \
    dashboard/dashboard.cpp \
    encryptor/encryptor.cpp \
    login/loginscreen.cpp \
    main.cpp \
    mainwindow.cpp \
    notes/notesmanager.cpp \
    password/passwordmanager.cpp \
    password/passwordworker.cpp \
    password/storage/passwordstorage.cpp \
    security-manager/securitymanager.cpp \
    setup/setupdialog.cpp

HEADERS += \
    categories/categorymanager.h \
    login/autolockmanager.h \
    password/addpassworddialog.h \
    password/passwordcard.h \
    settings/settingsmanager.h \
    dashboard/dashboard.h \
    encryptor/encryptor.h \
    login/loginscreen.h \
    mainwindow.h \
    notes/notesmanager.h \
    password/passwordmanager.h \
    password/passwordworker.h \
    password/storage/passwordstorage.h \
    security-manager/securitymanager.h \
    setup/setupdialog.h

FORMS += \
    dashboard/dashboard.ui \
    login/loginscreen.ui \
    mainwindow.ui \
    notes/notesmanager.ui \
    password/passwordmanager.ui \
    setup/setupdialog.ui

# Default rules for deployment
isEmpty(PREFIX){
 PREFIX = /usr
}

message("Installation prefix: $$PREFIX")

BINDIR  = $$PREFIX/bin
DATADIR = $$PREFIX/share

target.path = $$BINDIR

unix:INSTALLS += target
