
#!/bin/bash

for i in 0 1 2 3 4
    do
        echo $i>ab
        echo $i+1 | bc >> ab
        ./a.out<ab
    done

