gcc -nostdlib -o pure_asm pure_asm.c

gcc -nostdlib -o mixed mixed.c

gcc -o pure_c pure_c.c

gcc -mavx2 -O2 -o compare_avx2 compare_avx2.c