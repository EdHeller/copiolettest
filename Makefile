.SUFFIXES: # Suppress default legacy rules

.PHONY: 
all: fix

#Add rgblink, rgbasm, and rgbfix to your path

# Search for .c files in "src" directory; .h files in "include" directory
# The pattern matching character '%' matches filename without the extension
vpath %.s src
vpath %.inc src/inc
vpath %.png src/gfx

#rgbgfx -u helloworld.png -o helloworld.2bpp -t helloworld.tilemap
#rgbgfx -u -c "#3cabe0,#b1d36a,#ffffff,#000000#;" helloworld_inv.png -o helloworld.2bpp -t helloworld.tilemap

game.gb: helloWorld.o
	rgblink -o ./bin/$@ $<

helloWorld.o: helloWorld.s
	rgbasm -o $@ $<


#adds the right header info so the game will be bootable. 
#Consider -c for gameboy color compatiblible marking
.PHONY:
fix: game.gb
	rgbfix -v -p 0xFF ./bin/game.gb


.PHONY: 
clean:
	rm helloWorld.o ./bin/game.gb


.PHONY: 
run:
	vba ./bin/game.gb
