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

crescente : crescente.c
	gcc gelbas.c elle.c crescente.c -I$(HOME)/prj/src/include $(HOME)/prj/src/obj/libprj.a -o vlosbas

base-ori : base-ori.c
	gcc gelbas.c base-ori.c -I$(HOME)/prj/src/include $(HOME)/prj/src/obj/libprj.a -o base

vgelbasIA : vgelbasIA.c
	gcc vgelbasIA.c -I$(HOME)/prj/src/include $(HOME)/prj/src/obj/libprj.a -o vgelbasIA

vgelbasIA1 : vgelbasIA.c
	gcc vgelbasIA.c -I$(HOME)/prj/src/include $(HOME)/prj/src/obj/libprj.a -g -o vgelbasIA