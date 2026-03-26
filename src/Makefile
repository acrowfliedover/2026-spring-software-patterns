# Patterns project Makefile
CXX      ?= g++
CXXFLAGS := -std=c++20 -Wall -I.
TARGET   := Patterns

SRCS := Attr.C Builder.C Director.C Document.C DOMDocumentAdapter.C \
        Element.C NamedNodeMap.C Node.C Test.C Text.C \
        ValidChildren.C XMLSerializer.C XMLTokenizer.C XMLValidator.C

OBJS := $(SRCS:.C=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.C
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
