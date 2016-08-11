parser: cassandra-lexer.cpp cassandra-parser.cpp cassandra-driver.h \
	cassandra-driver.cpp main.cpp pomdp.h pomdp.cpp cassandra-parser.h \
	dsgame.h dsgame.cpp
	c++ -O3 -std=c++11 cassandra-lexer.cpp cassandra-parser.cpp \
		cassandra-driver.cpp pomdp.cpp dsgame.cpp main.cpp \
		-I/usr/local/include \
		-I/usr/local/include/eigen3 \
		-I/Users/gaperez/Dropbox/ULB/AI-Toolbox/include \
		-L/usr/local/lib \
		-L/Users/gaperez/Dropbox/ULB/AI-Toolbox/build \
		-lz3 -lAIToolboxMdp -lAIToolboxPOMDP -o parser

cassandra-lexer.cpp: cassandra-parser.h cassandra.l
	flex --outfile=cassandra-lexer.cpp cassandra.l

cassandra-parser.h: cassandra.y
	bison --defines=cassandra-parser.h \
		--output=cassandra-parser.cpp cassandra.y

cassandra-parser.cpp: cassandra.y
	bison --defines=cassandra-parser.h \
		--output=cassandra-parser.cpp cassandra.y

clean:
	rm cassandra-parser.h cassandra-parser.cpp \
		cassandra-lexer.cpp location.hh position.hh \
		stack.hh parser
