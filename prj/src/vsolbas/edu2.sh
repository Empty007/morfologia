#exemplo 1
echo "5 5
0 0 1 0 0
0 1 1 1 0
1 1 1 1 1
0 1 1 1 0
0 0 1 0 0" > linha1.ger


~/prj/bin/str_ger linha1.ger linha1.str

#~/prj/bin/vdspbas ~/prj/tmp/edusai01.txt  ~/prj/tmp/sai11.txt 200

#less ~/prj/tmp/sai11.txt

#exemplo 2
echo "5 5
1 1 1 1 1
1 1 1 1 1
1 1 1 1 1
1 1 1 1 1
1 1 1 1 1" > linha2.ger

#~/prj/bin/str_ger linha2.ger linha2.str
#~/prj/bin/vdilbas ~/prj/bas/baseid.txt ~/prj/tmp/edusai01.txt linha1.str
#~/prj/bin/vdilbas ~/prj/bas/edusai01.txt ~/prj/tmp/edusai02.txt linha2.str
~/prj/bin/vdspbas edusai03.txt  ~/prj/tmp/sai12.txt 200
#less ~/prj/tmp/sai12.txt

case $1 in
	-1) 
		time ./vgelbasIA edusai01.txt linha1.str
	;;
	-2) 
		time ./vgelbasIA edusai02.txt linha2.str
	;;
	-3) 
		time ./vgelbasIA edusai04.txt linha2.str
	;;
	-11) 
		time ./vgelbas edusai01.txt linha1.str
	;;
	-22) 
		time ./vgelbas edusai02.txt linha2.str
	;;
	-33) 
		time ./vgelbas edusai04.txt linha2.str
	;;
esac