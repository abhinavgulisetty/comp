#include <stdio.h>
#include <string.h>

#define MAX_STATES 20
#define MAX_TRANS 50

typedef struct{
    int from,to;
    char symbol;
}Transition;

Transition trans[MAX_TRANS];
int num_states,num_trans;

void epsilon_closure(int s,int closure[],int *size){
    int visited[MAX_STATES]={0},stack[MAX_STATES],top=-1;
    stack[++top]=s; visited[s]=1;

    while(top>=0){
        int cur=stack[top--];
        closure[(*size)++]=cur;

        for(int i=0;i<num_trans;i++)
            if(trans[i].from==cur && trans[i].symbol=='e' && !visited[trans[i].to]){
                visited[trans[i].to]=1;
                stack[++top]=trans[i].to;
            }
    }
}

void sort(int a[],int n){
    for(int i=0;i<n-1;i++)
        for(int j=i+1;j<n;j++)
            if(a[i]>a[j]){int t=a[i];a[i]=a[j];a[j]=t;}
}

int main(){
    printf("Epsilon Closure\n");

    printf("Enter number of states: ");
    scanf("%d",&num_states);

    printf("Enter number of transitions: ");
    scanf("%d",&num_trans);

    printf("Enter transitions (from to symbol)\n");
    for(int i=0;i<num_trans;i++)
        scanf("%d %d %c",&trans[i].from,&trans[i].to,&trans[i].symbol);

    printf("\nClosures:\n");
    for(int s=0;s<num_states;s++){
        int closure[MAX_STATES],size=0;
        epsilon_closure(s,closure,&size);
        sort(closure,size);

        printf("e-closure(q%d) = { ",s);
        for(int i=0;i<size;i++){
            printf("q%d",closure[i]);
            if(i<size-1) printf(", ");
        }
        printf(" }\n");
    }
    return 0;
}