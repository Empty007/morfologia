@echo off
md c:\prj\temp
cd c:\prj\temp
copy c:\prj\src\obj\pac_*.o
copy c:\prj\src\obj\desc_*.o
copy c:\prj\src\obj\negation.o
cd c:\prj\src\vnegbas
gcc -Ic:/prj/src/include vnegbas.c c:\prj\temp\*.o -o vnegbas
deltree c:\prj\temp

