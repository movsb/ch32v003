!/bin/bash

cat build/ch32v00x.lst | grep '.text' | grep ' F ' | awk '{printf("%d\t%s\n", strtonum("0x"$5), $0)}' | sort -k1,1nr | awk '{print substr($0, index($0, "\t") + 1)}'

