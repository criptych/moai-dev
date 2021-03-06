#================================================================#
# Copyright (c) 2010-2017 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

include $(CLEAR_VARS)

LOCAL_MODULE 		:= expat
LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
LOCAL_CFLAGS		:= -DHAVE_MEMMOVE=1 -DXML_STATIC -DXML_DEV_URANDOM
LOCAL_CFLAGS		+= -include $(MOAI_SDK_HOME)/src/zl-vfs/zl_replace.h
LOCAL_CFLAGS		+= -fvisibility=hidden

LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/expat-2.2.6/lib/xmlparse.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/expat-2.2.6/lib/xmlrole.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/expat-2.2.6/lib/xmltok.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/expat-2.2.6/lib/xmltok_impl.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/expat-2.2.6/lib/xmltok_ns.c

include $(BUILD_STATIC_LIBRARY)
