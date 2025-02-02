GG = 'gcc'
DEST = 'exe'
SOURCE ='main.c'

all: $(DEST)

$(DEST):
	$(GG) -Wall -o exe $(SOURCE)
clear:
	rm -f $(DEST)
