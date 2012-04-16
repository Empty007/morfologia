@echo off
md c:\prj\temp
cd c:\prj\temp
copy c:\prj\src\obj\pac_*.o
copy c:\prj\src\obj\desc_*.o
copy c:\prj\src\obj\union_basis.o
copy c:\prj\src\obj\negation.o
cd c:\prj\src\vunibas
gcc -Ic:/prj/src/include vunibas.c c:\prj\temp\*.o -o vunibas
deltree c:\prj\temp

