all: compile link

compile:ques1.cpp ques3.cpp ques4.cpp ques4_2.cpp 
	g++ -c ques1.cpp
	g++ -c ques3.cpp
	g++ -c ques4.cpp
	g++ -c ques4_2.cpp
	
link: 
	g++ -o ques1 ques1.o
	g++ -o ques3 ques3.o
	g++ -o ques4 ques4.o
	g++ -o ques4_2 ques4_2.o

run:
	./ques1
	./ques3
	./ques4
	./ques4_2
	
clean: 
	rm -f ques1.o
	rm -f ques3.o
	rm -f ques4.o
	rm -f ques4_2.o
