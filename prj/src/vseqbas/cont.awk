BEGIN { set = 0;     cont = 0; }

{
  if (/\-/) {
    for (i=1; i<10; i++) {
      # printf ("%d = %d %d\n", i, $i, $i==1);
      if ($i == 1) {
        cont++; 
      }
    }
  }
  else {
    set++;
    printf ("set = %02d cont = %d\n", set, cont);
    cont = 0;
  }
}
