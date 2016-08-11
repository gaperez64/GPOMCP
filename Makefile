parser: cassandra-lexer.cpp cassandra-parser.cpp cassandra-driver.h \
	cassandra-driver.cpp main.cpp pomdp.h pomdp.cpp cassandra-parser.h
	c++ -O3 -std=c++11 cassandra-lexer.cpp cassandra-parser.cpp \
		cassandra-driver.cpp pomdp.cpp main.cpp -o parser

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
		stack.hh
