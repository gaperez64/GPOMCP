parser: cassandra-lexer.cpp cassandra-parser.cpp cassandra-driver.h \
	cassandra-driver.cpp
	c++ cassandra-lexer.cpp cassandra-parser.cpp \
		cassandra-driver.cpp main.cpp -o parser

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
