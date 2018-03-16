GCC:=gcc
#c++编译器
CXX:=gcc
#c编译器
CC:=gcc

#取得操作系统名称#OS_NAME="Linux:SunOS:HP-UX:AIX"
OS_NAME:= $(shell uname -s)

#######c编译选项###############
#c编译器参数,建议-W -Wall必须使用
CFLAGS:= -c -g -W -Wall
#CPP c预编译器名字,默认$(CC) -E
CPP:=
#CPPFLAGS C预编译器选项
CPPFLAGS:=
######c++编译选项#############
#c++编译选项 c++编译器参数
CXXFLAGS:= -c -g -W -Wall
#c++预编译器参数
CXXPPFLAGS:=

#c链接器选项
CLD_FLAGS:= 
#c++链接器选项
CXXLD_FLAGS:= 

############################################################
##指定源文件的路径/支持的源文件的扩展名/源文件搜索路径
##自动搜索源文件名
## 注意:
##   文件名不能含特殊字符,如空格/通配符等
#指定支持的源代码扩增名
SFIX:=.c .C .cpp .cc .CPP .c++ .cp .cxx 
#从指定的路径目录下获取源文件
#$(wildcard PATTERN) 列出当前目录下所有符合模式“PATTERN”格式的文件名
SRC_DIR:=
AUTO_SOURCES:= $(foreach x,${SRC_DIR},\
                     $(wildcard  \
                        $(addprefix  ${x}/*,${SFIX})) )
#生成依赖文件名集合) 
#AUTO_OBJS:=$(addsuffix .o ,$(basename $(notdir ${SOURCES}) ) ) #notdir拿掉路径部分; basename取出文件名中最后一个点号之前的部分
AUTO_OBJS:=$(addsuffix .o ,$(basename  ${AUTO_SOURCES})  ) #含路径信息
###########################################################
#检查源码中，除了C源码外是否有C++源码 ,并定义变量LDCXX存储检查结果
LDCXX:= $(strip $(filter-out  %.c , ${AUTO_SOURCES} ) )
ifdef LDCXX   #有C++源码时,所有源码都使用g++编译，包括C源码，将CC、CFLAGS 的值设置为对应的${CXX}、 ${CXXFLAGS}的值
  CC := ${CXX}                    #重置C编译器为C++编译器
  CFLAGS :=  ${CXXFLAGS}          #重置C编译选现为C++编译选现
  CPP :=  ${CXXPP}                #重置C预编译器为C++预编译器
  CPPFLAGS := ${CXXPPFLAGS}       #重置C预编译的选项为C++预编译的选项
endif

#宏定义
DCPPFLAGS:=-D${OS_NAME}

###############################以上是公共定义部分####################################
#头文件路径
INCLUDES:= -I./ -I./include/
#动态库路径
LIB_DIR:=
SO_LIBS:=-lstdc++ -lpthread -lrt

#常用文件
COMMON_SOURCES:=
COMMON_OBJS:= $(COMMON_SOURCES:.cpp=.o)

PRJ_SOURCES=src/lex_analyzer.cpp src/express_parser.cpp

SOURCES:=$(COMMON_SOURCES) $(AUTO_SOURCES) $(PRJ_SOURCES)
OBJS:= $(AUTO_OBJS) $(COMMON_OBJS) $(PRJ_SOURCES:.cpp=.o)

#单元测试
unittest_FLAG:=1
unittest_TARGETS=unittest_lexer 
unittest_SOURCES:= src/unittest_lexer.cpp

unittest_OBJS=$(unittest_SOURCES:.cpp=.o)
ifdef unittest_FLAG
#SOURCES +=$(unit_test_SOURCES)
#OBJS +=$(unit_test_OBJS)
#INCLUDES += -I./unit_test -I./unit_teset/pub_lib -I./unit_test/pub_lib/cppunit_tool
#LIB_DIR += -Lunit_test/pub_lib/cppunit_tool/linux64
#SO_LIBS +=-lcppunit
unittest_INCLUDES:=-I./unit_test -I./unit_test/pub_lib -I./unit_test/pub_lib/cppunit_tool -I./unit_test/pub_lib/gtest/include 
unittest_LIB_DIR:=-Lunit_test/pub_lib/cppunit_tool/linux64 -L./unit_test/pub_lib/gtest/linux64/
unittest_LIBS:= #-lgtest #-lcppunit 
CFLAGS += -DSINGLE_UNITTEST  
CPPFLAGS += -DSINGLE_UNITTEST 
#使用googletest
LINK_FLAGS =  #-isystem
endif

#汇总所有的源文件
TARGET:=mxx_lexer
#输出路径
OUTPUT_DIR:=./bin
all:$(TARGET)

#单元测试构建规则
$(TARGET):$(OBJS)
	$(GCC) -o $@ $(LIB_DIR) $(SO_LIBS)  $(OBJS)

#$@表示目标; $^表示所有依赖文件; $<表示第一个依赖文件
#模式规则: 
# 模式规则中，至少在规则的目标定义中要包含"%"，否则，就是一般的规则。
# 目标中的"%"定义表示对文件名的匹配，"%"表示长度任意的非空字符串。
# 例如："%.c"表示以".c"结尾的文件名（文件名的长度至少为3），而"s.%.c"则表示以"s."开头，".c"结尾的文件名（文件名的长度至少为5）
# 模式规则语法:  %.o: %.c ; COMMAND...
# 老式模式规则语法: .c.o: <=> %.o: %.c
#静态模式:
#  规则存在多个目标;
#  并且不同的目标可以根据目标文件的名字来自动构造出依赖文件
#  静态模式规则比多目标规则更通用
#静态规则:
#  静态规则语法: 
#     TARGETS ...: TARGET-PATTERN: PREREQ-PATTERNS ...
#        
$(COMMON_OBJS): %.o:%.cpp
	$(GCC) -o $@ $(DCPPFLAGS) $(CFLAGS) $(INCLUDES) $<

$(PRJ_SOURCES:.cpp=.o): %.o:%.cpp
	$(GCC) -o $@ $(DCPPFLAGS) $(CFLAGS) $(INCLUDES) $<
    
%.o: %.c
	$(CC) ${DCPPFLAGS}  ${CFLAGS}   ${INCLUDES} $<
%.o: %.C
	$(CXX) ${DCPPFLAGS} ${CXXFLAGS} ${INCLUDES) $<
%.o: %.cc
	${CXX} ${DCPPFLAGS} ${CXXFLAGS} ${INCLUDES}   $<
%.o: %.cpp
	${CXX}     ${DCPPFLAGS}    ${CXXFLAGS}    ${INCLUDES}   $<
%.o : %.CPP
	${CXX}     ${DCPPFLAGS}    ${CXXFLAGS}    ${INCLUDES}   $<
%.o: %.c++
	${CXX}     ${DCPPFLAGS}    ${CXXFLAGS}    ${INCLUDES}   $<
%.o: %.cp
	${CXX}     ${DCPPFLAGS}    ${CXXFLAGS}    ${INCLUDES}   $<
%.o: %.cxx
	${CXX}     ${DCPPFLAGS}    ${CXXFLAGS}    ${INCLUDES}   $<



#单元测试构建规则
#unittest:$(OBJS) $(unittest_OBJS)
unittest:$(OBJS) $(unittest_TARGETS)
#	$(GCC) -o $@ $(LIB_DIR) $(SO_LIBS) $(unittest_LIB_DIR) $(unit_test_LIB_DIR) $(LINK_FLAGS)  $(OBJS) $(unittest_OBJS) unit_test/pub_lib/gtest/linux64/libgtest.a
	@echo "build unit test finished!"

./unit_test/unittest_mem_buff:./unit_test/unittest_mem_buff.o 
	$(GCC) -o $@ $(LIB_DIR) $(SO_LIBS) $(unittest_LIB_DIR) $(unit_test_LIB_DIR) $(unittest_LIBS) $(LINK_FLAGS) $< unit_test/pub_lib/gtest/linux64/libgtest.a  
#unit_test/pub_lib/gtest/linux64/libgtest.a

./unit_test/unittest_dataMemberDescriber: ./unit_test/unittest_dataMemberDescriber.o dataMemberDescriber.o
	$(GCC) -o $@ $(LIB_DIR) $(SO_LIBS) $(unittest_LIB_DIR) $(unit_test_LIB_DIR) $(unittest_LIBS) $(LINK_FLAGS) $^ unit_test/pub_lib/gtest/linux64/libgtest.a 

$(unittest_OBJS): %.o:%.cpp
	$(GCC) -o $@ $(DCPPFLAGS) $(CFLAGS) $(INCLUDES) $(unittest_INCLUDES) $<


clean:
	rm -f $(OBJS)
	rm -f $(unittest_OBJS) unittest #清理单元测试相关内容

test:
	@echo "AutoSources=[${AUTO_SOURCES}]"
	@echo "AutoObjs=[${AUTO_OBJS}]"
	@echo "LDCXX=[${LDCXX}]"
	@echo "OBJS=[${OBJS}]"
