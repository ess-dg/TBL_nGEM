
PROG = read_nGEM_data

all: $(PROG) 

clean:
	rm -f $(PROG) a.out 

$(PROG): $(PROG).cpp
#	g++ -std=c++14  -Wall -Werror $< -o $@ 
	g++ -std=c++14 $< -o  $@ 
