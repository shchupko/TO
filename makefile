


LINK_TARGET = top

OBJS =  top.o \
	MockPetsc.o \
	Optimization.o \
	LoadCases.o  \
	Log.o \
	tinyxml.o \
	ParseXML.o \
	GUIXml.o

REBUILDABLES = $(OBJS) $(LINK_TARGET)

clean : 
	rm -f $(REBUILDABLES)
	echo Clean done

all : $(LINK_TARGET)
	echo All done

$(LINK_TARGET) : $(OBJS)
	rm -f $(LINK_TARGET)
	g++ -g -o $@ $^
	rm -f $(OBJS)
