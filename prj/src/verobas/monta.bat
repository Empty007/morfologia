@echo off
md c:\prj\temp
cd c:\prj\temp
copy c:\prj\src\obj\pac_*.o
copy c:\prj\src\obj\tools.o
copy c:\prj\src\obj\desc_*.o
copy c:\prj\src\obj\fullpath.o
copy c:\prj\src\obj\erosion_basis.o
copy c:\prj\src\obj\utils.o
copy c:\prj\src\obj\strings.o
copy c:\prj\src\obj\intersection_basis.o
copy c:\prj\src\obj\cr_image.o
copy c:\prj\src\obj\machtype.o
copy c:\prj\src\obj\imagesize.o
copy c:\prj\src\obj\machorde.o
copy c:\prj\src\obj\keywords.o
copy c:\prj\src\obj\toolboxes.o
copy c:\prj\src\obj\putenv.o
cd c:\prj\src\verobas
gcc -Ic:/prj/src/include verobas.c c:\prj\temp\*.o -o verobas
deltree c:\prj\temp

