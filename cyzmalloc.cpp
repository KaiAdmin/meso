#include<cmath>
class HEAP{
public:
    int brk,chk;
    int head=0,tot=0,nexts[33]={0};
    void * initptr[33]={nullptr};
    bool vis[33][128]={0};
}h[16];
void * cyzmalloc(size_t size){
    int g=ceil(log2(size));
    //if size is too large
    if(g>=17)return malloc(size);
    //allocate
    if(h[g].tot!=0){
        for(int i=h[g].head;i;i=h[g].nexts[i]){
            for(int j=0;j<128;j++){
                if(h[g].vis[i][j]==0){
                    h[g].vis[i][j]=1;
                    return (void*)h[g].initptr[i]+h[g].chk*j;
                }   
            }
        }
    }
    //if used up
    if(h[g].tot<=32){
        h[g].initptr[++h[g].tot]=malloc((1<<g)*128);h[g].nexts[h[g].tot]=h[g].head;h[g].head=h[g].tot;
        h[g].vis[h[g].tot][0]=1;
        return (void *)h[g].initptr[h[g].tot];
    }
    //then fail
    return nullptr;
}
