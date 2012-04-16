BEGIN {cont=0; abre=0}
/\/\*=\*\// {print $0; cont++; abre=1}
END{print cont}
