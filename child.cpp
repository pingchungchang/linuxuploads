#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <iostream>
#include <cstdio>
#include <string.h>

int main(int argc,char* argv[]){
    int j = atoi(argv[1]);
    int ans = 0;
    for(int i = 0;i<=j;i++){
        ans +=i;
    }
    printf("%d\n",ans);
    return 0;
}
