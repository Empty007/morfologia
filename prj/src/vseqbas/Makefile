HOME = /Users/macbookpro/Documents
vsolbas : vsolbas.c
	gcc vsolbas.c -I$(HOME)/prj/src/include $(HOME)/prj/src/obj/libprj.a -o vsolbas
	cp vsolbas.ori $(HOME)/prj/bin/vsolbas
	chmod u+x $(HOME)/prj/bin/vsolbas
clean :
	rm -f *~
str_ger : str_ger.c
	gcc str_ger.c -I$(HOME)/prj/src/include $(HOME)/prj/src/obj/libprj.a -o str_ger
vlosbas : vlosbas.c
	gcc vlosbas.c -I$(HOME)/prj/src/include $(HOME)/prj/src/obj/libprj.a -o vlosbas

comb : comb.c
	gcc comb.c $(HOME)/prj/src/obj/libprj.a -o comb

vgelbas : vgelbas.c
	gcc vgelbas.c -I$(HOME)/prj/src/include $(HOME)/prj/src/obj/libprj.a -o vgelbas
	cp vgelbas vlosbas

vseqbas : vseqbas.c
	gcc vseqbas.c -I$(HOME)/prj/src/include $(HOME)/prj/src/obj/libprj.a -o vseqbas

velebas : elle.c vgelbas.c vseqbas.c
	gcc vseqbas.c elle.c -I$(HOME)/prj/src/include $(HOME)/prj/src/obj/libprj.a -o velebas
	cp velebas vseqbas

base : base.c
	gcc gelbas.c base.c -I$(HOME)/prj/src/include $(HOME)/prj/src/obj/libprj.a -o base

crescente_str : crescente_str.c
	gcc gelbas.c elle.c crescente_str.c -I$(HOME)/prj/src/include $(HOME)/prj/src/obj/libprj.a -o vlosbas

base-ori : base-ori.c
	gcc gelbas.c base-ori.c -I$(HOME)/prj/src/include $(HOME)/prj/src/obj/libprj.a -o base

########################################################

cria: cria.c gelbas.c
	gcc gelbas.c cria.c -I$(HOME)/prj/src/include $(HOME)/prj/src/obj/libprj.a -o cria

upper: upper.c gelbas.c
	gcc gelbas.c upper.c -I$(HOME)/prj/src/include $(HOME)/prj/src/obj/libprj.a -o upper

lower: lower.c elle.c gelbas.c
	gcc gelbas.c elle.c lower.c -I$(HOME)/prj/src/include $(HOME)/prj/src/obj/libprj.a -o lower

lraha: lraha.c elle.c gelbas.c
	gcc gelbas.c elle.c lraha.c -I$(HOME)/prj/src/include $(HOME)/prj/src/obj/libprj.a -o lraha

sol : sol.c elle.c
	gcc gelbas.c elle.c sol.c -I$(HOME)/prj/src/include $(HOME)/prj/src/obj/libprj.a -o sol

lowerc: lowerc.c gelbas.c
	gcc gelbas.c lowerc.c -I$(HOME)/prj/src/include $(HOME)/prj/src/obj/libprj.a -o lowerc

solc : solc.c elle.c gelbas.c
	gcc gelbas.c elle.c solc.c -I$(HOME)/prj/src/include $(HOME)/prj/src/obj/libprj.a -o solc

crescente : crescente.c gelbas.c elle.c
	gcc gelbas.c elle.c crescente.c -I$(HOME)/prj/src/include $(HOME)/prj/src/obj/libprj.a -o crescente

cresc : cresc.c gelbas.c elle.c
	gcc gelbas.c elle.c cresc.c -I$(HOME)/prj/src/include $(HOME)/prj/src/obj/libprj.a -o cresc

dual: dual.c gelbas.c
	gcc gelbas.c dual.c -I$(HOME)/prj/src/include $(HOME)/prj/src/obj/libprj.a -o dual

cbase: cbase.c gelbas.c
	gcc gelbas.c cbase.c -I$(HOME)/prj/src/include $(HOME)/prj/src/obj/libprj.a -o cbase
