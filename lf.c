#include <stdio.h>
#include <string.h>

#define MAXP 20
#define MAXA 20
#define MAXL 100

int prefix_len(char *a,char *b){
    int i=0;
    while(a[i] && b[i] && a[i]==b[i]) i++;
    return i;
}

void print_prime(char base,int level){
    printf("%c",base);
    for(int i=0;i<level;i++) printf("'");
}

void factor(char lhs,char alt[MAXA][MAXL],int n,int level){

    if(n<=1){
        printf("%c -> %s\n",lhs,alt[0]);
        return;
    }

    char prefix[MAXL];
    strcpy(prefix,alt[0]);
    int plen=strlen(prefix);

    for(int i=1;i<n;i++){
        int c=prefix_len(prefix,alt[i]);
        if(c<plen){
            plen=c;
            prefix[plen]='\0';
        }
    }

    if(plen==0){
        printf("%c -> ",lhs);
        for(int i=0;i<n;i++){
            printf("%s",alt[i]);
            if(i<n-1) printf(" | ");
        }
        printf("\n");
        return;
    }

    printf("%c -> %s",lhs,prefix);
    print_prime(lhs,level);
    printf("\n");

    char suffix[MAXA][MAXL];
    int k=0;

    for(int i=0;i<n;i++){
        char *s=alt[i]+plen;
        if(strlen(s)==0)
            strcpy(suffix[k++],"#");
        else
            strcpy(suffix[k++],s);
    }

    print_prime(lhs,level);
    printf(" -> ");

    int need_recursion=0;

    for(int i=1;i<k;i++){
        if(prefix_len(suffix[0],suffix[i])>0)
            need_recursion=1;
    }

    if(!need_recursion){
        for(int i=0;i<k;i++){
            printf("%s",suffix[i]);
            if(i<k-1) printf(" | ");
        }
        printf("\n");
        return;
    }

    printf("\n");
    factor(lhs,suffix,k,level+1);
}

int main(){

    char prod[MAXP][MAXL];
    int n=0;

    printf("Enter productions:\n");

    while(1){
        printf("P[%d]: ",n+1);
        fgets(prod[n],MAXL,stdin);
        prod[n][strcspn(prod[n],"\n")]=0;

        if(strcmp(prod[n],"end")==0) break;

        n++;
    }

    printf("\nAfter Left Factoring:\n");

    for(int i=0;i<n;i++){

        char lhs=prod[i][0];
        char *rhs=strchr(prod[i],'=');
        if(!rhs) continue;
        rhs++;

        char temp[MAXL];
        strcpy(temp,rhs);

        char alt[MAXA][MAXL];
        int k=0;

        char *t=strtok(temp,"|");
        while(t){
            strcpy(alt[k++],t);
            t=strtok(NULL,"|");
        }

        factor(lhs,alt,k,1);
    }

    return 0;
}