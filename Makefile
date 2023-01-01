# Projekt: Analyza evoluce kooperace v opakovanych hrach s nenulovym souctem
# Autor:   Jaroslav Bendl (xbendl00)
# Datum:   18.11.2010
# 
# Pouziti:
#   - preklad:      make
#   - zabalit:      make pack
#   - vycistit:     make clean

all:
	$(MAKE) -C src

clean:
	rm -f *.o *.tar *.tar.gz ipd

pack:
	tar cvf xbendl00.tar *
	gzip -9 xbendl00.tar
