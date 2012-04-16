case $1 in
	-0) 
		time ./vgelbas tests/test_key.txt linha3.str
	;;
	-1) 
		time ./vgelbas tests/test1.txt linha3.str
	;;
	-2) 
		time ./vgelbas tests/test2.txt linha3.str
	;;
	-3) 
		time ./vgelbas tests/test3.txt linha3.str
	;;
	-4) 
		time ./vgelbas tests/test4.txt linha3.str
	;;
	-5) 
		time ./vgelbas tests/failarvore.txt linha3.str
	;;
	-6) 
		time ./vgelbas tests/failarvore2.txt linha3.str
	;;
	-7) 
		time ./vgelbas tests/failarvore3.txt linha3.str
	;;
	-8) 
		time ./vgelbas tests/failarvore4.txt linha3.str
	;;
	-9) 
		time ./vgelbas tests/failarvore5.txt linha3.str
	;;	
	-10) 
		time ./vgelbas tests/failarvore6.txt linha3.str
	;;
	-11) 
		time ./vgelbas tests/failarvore7.txt linha3.str
	;;
	-12) 
		time ./vgelbas tests/failarvore8.txt linha3.str
	;;
	-13) 
		time ./vgelbas tests/failarvore9.txt linha3.str
	;;
	-14) 
		time ./vgelbas tests/reallyhard.txt linha3.str
	;;
	-15) 
		time ./vgelbas tests/medium.txt linha3.str
	;;
	-16) 
		time ./vgelbas tests/filter.txt linha3.str
	;;
	-17) 
		time ./vgelbas tests/almosthard.txt linha3.str
	;;
	-22) 
		time ./vgelbas_sin_copia tests/test2.txt linha3.str
	;;
	-33) 
		time ./vgelbas_sin_copia tests/test3.txt linha3.str
	;;
	-44) 
		time ./vgelbas_upper tests/test4.txt linha3.str
	;;
esac