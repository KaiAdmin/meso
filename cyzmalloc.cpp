#include<cmath>

class Heap{
public:
    int brk,chk;
    int head=0,tot=0,nexts[3300]={0},last[3300]={0};
    void * initptr[3300]={nullptr};
    bool vis[3300][128]={0};
    int viscnt[3300]={0};
}h[16];

void * cyzmalloc(size_t size){
    int mlcsiz=ceil(log2(size));
    //if size is too large
    if(mlcsiz>=17)return malloc(size);

    //allocate
    if(h[mlcsiz].tot!=0){
        for(int i=h[mlcsiz].head;i;i=h[mlcsiz].nexts[i]){
            for(int j=0;j<128;j++){
                if(h[mlcsiz].vis[i][j]==0){
                    h[mlcsiz].vis[i][j]=1;h[mlcsiz].viscnt[i]++;
                    return (void*)h[mlcsiz].initptr[i]+h[mlcsiz].chk*j;
                }   
            }
        }
    }

    //if used up
    if(h[mlcsiz].tot<=3200){
        h[mlcsiz].initptr[++h[mlcsiz].tot]=malloc((1<<mlcsiz)*128);

        h[mlcsiz].nexts[h[mlcsiz].tot]=h[mlcsiz].head;h[mlcsiz].head=h[mlcsiz].tot;

        h[mlcsiz].last[h[mlcsiz].nexts[h[mlcsiz].tot]]=h[mlcsiz].tot;

        h[mlcsiz].vis[h[mlcsiz].tot][0]=1;h[mlcsiz].viscnt[h[mlcsiz].tot]++;

        return (void *)h[mlcsiz].initptr[h[mlcsiz].tot];
    }
    //then fail
    return nullptr;
}

void cyzfree(void * ptr){
    for(int i=1;i<=16;i++){//which h
        for(int j=h[i].head;j;j=h[i].nexts[j]){
            if(h[i].initptr[j]<=ptr&&ptr<h[i].initptr[j]+h[i].brk){//find
                if(((ptrdiff_t)ptr-(ptrdiff_t)h[i].initptr[j])%h[i].chk!=0)return;//not available
                if(h[i].vis[j][((ptrdiff_t)ptr-(ptrdiff_t)h[i].initptr[j])/h[i].chk]==0)return;//it has been freed

                h[i].vis[j][((ptrdiff_t)ptr-(ptrdiff_t)h[i].initptr[j])/h[i].chk]=0;//free
                h[i].viscnt[j]--;

                if(h[i].viscnt[j]==0){
                    free(h[i].initptr[j]);//free this space;
                    h[i].last[h[i].nexts[j]]=h[i].last[j];
                    h[i].nexts[h[i].last[j]]=h[i].nexts[j];
                }

                return;
            }
        }
    }
}
