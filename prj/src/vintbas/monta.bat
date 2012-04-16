@echo off
md c:\prj\temp
cd c:\prj\temp
copy c:\prj\src\obj\pac_*.o
copy c:\prj\src\obj\desc_*.o
copy c:\prj\src\obj\intersec.o
cd c:\prj\src\vintbas
gcc -Ic:/prj/src/include vintbas.c c:\prj\temp\*.o -o vintbas
deltree c:\prj\temp

