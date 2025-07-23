gcc -o bin/pure_c-v1 pure_c.c

gcc -static -o bin/pure_c-v2 pure_c.c

gcc -nostdlib -o bin/pure_asm pure_asm.c

gcc -nostdlib -fno-stack-protector -no-pie -o bin/mixed-v1 mixed-v1.c 

gcc -nostdlib -fno-stack-protector -no-pie -o bin/mixed-v2 mixed-v2.c -lc

gcc -mavx2 -O2 -o bin/compare_avx2 compare_avx2.c