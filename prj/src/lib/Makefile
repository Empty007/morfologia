HOME = /Users/macbookpro/Documents
OBJS = utils.o lvrot.o negation.o fullpath.o tools.o cr_image.o pac_win.o pac_bas.o desc_bas.o desc_win.o \
           dilate_b.o keywords.o machtype.o putenv.o machorde.o strings.o imagesize.o pac_xpl.o pac_tool.o erosion_.o \
           intersec.o fat.o dsp.o union_ba.o toolboxes.o 

libprj.a : $(OBJS)
	ar r libprj.a $(OBJS)
	mv libprj.a $(HOME)/prj/src/obj
%.o : %.c
	gcc -c -I$(HOME)/prj/src/include $<

clean :
	-rm -f *.o

