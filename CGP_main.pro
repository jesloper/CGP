TEMPLATE = subdirs
CONFIG += ordered debug
    
LANGUAGE = C++
RESOURCES = ./icons/sources.qrc
SUBDIRS = 	src \
            ErrorDetection \
           ErrorDetectionMultiClass
					
