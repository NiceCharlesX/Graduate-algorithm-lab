#include <stdio.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <set>
#include <glpk.h>
#include <algorithm>
using namespace std;

int getRandom(set<int> Set){
    set<int>::iterator ite1 = Set.begin();
    set<int>::iterator ite2 = Set.end();
    int i=0,p;
    p=rand()%Set.size();
    for(;ite1!=ite2;ite1++)
    {
        if(i==p){
            return *ite1;
        }
        i++;
    }
}
void outputSet(set<int> Set){
    set<int>::iterator ite1 = Set.begin();
    set<int>::iterator ite2 = Set.end();
    for(;ite1!=ite2;ite1++)
    {
        cout<<" "<<*ite1;
    }
}
/*
int numIntersect(set<int> S1,set<int> S2){
    set<int> U;
    U.insert(S1.begin(),S1.end());
    U.insert(S2.begin(),S2.end());
    return S1.size()+S2.size()-U.size();
}*/

void greedySetCover1(set<int>* S,int num){
    set<int>* CS =new set<int>[num];
    int msize=0,mid;
    for(int i=0;i<num;i++){
        if(msize<S[i].size()){
            msize=S[i].size();
            mid=i;
        }
    }//初始找到包含最多元素的集合
    for(int i=0;i<num;i++){
        set_difference(S[i].begin(),S[i].end(),S[mid].begin(),S[mid].end(),inserter(CS[i],CS[i].begin()));
    }//拷贝S到CS
    do{
	if(num<=500){
	    cout<<"S"<<mid;
            outputSet(S[mid]);
            cout<<"\n";
	}
        msize=0;
        for(int i=0;i<num;i++){
            if(msize<CS[i].size()){
                msize=CS[i].size();
                mid=i;
            }
        }
        if(msize>0){
            for(int i=0;i<num;i++){
                set<int> mset;
                set_difference(CS[i].begin(),CS[i].end(),CS[mid].begin(),CS[mid].end(),inserter(mset,mset.begin()));
                CS[i]=mset;
            }//将CS中所有已被覆盖的元素删除
        }
    }
    while(msize!=0);
    delete[] CS;
    cout<<"\nend";
}

/*
void greedySetCover(set<int>* S,int num){
    bool* cs = new bool[num];
    for(int i=0;i<num;i++){
        cs[i] = false;
    }
    set<int> U;
    for(int i=0;i<num;i++){
        U.insert(i);
    }
    while(!U.empty()){
        int ncover=0,id;
        for(int i=0;i<num;i++){
            if(!cs[i]){
                int icover=numIntersect(U,S[i]);
                if(icover>ncover){
                    ncover=icover;
                    id=i;
                }
            }
        }
        set<int>::iterator ite1 = S[id].begin();
        set<int>::iterator ite2 = S[id].end();
        for(;ite1!=ite2;ite1++)
        {
            U.erase(*ite1);
        }
	if(num<=500){
	    cout<<"S"<<id;
	    outputSet(S[id]);
	    cout<<"\n";
	}
    }
    delete[] cs;
}*/

int* ia;
int* ja;//ia代表行坐标，ja代表列坐标
double* ar;

int main()
{
    srand(unsigned(time(0)));
    int num;
    cout<<"input num:\n";
    cin>>num;
    set<int>* S=new set<int>[num];

    int n=0,e;
    set<int> NU;
    set<int> US;
    for(int i=0;i<num;i++){
        NU.insert(i);
    }
    while(n<20){
        e=rand()%num;
        if(S[0].count(e)==0){
            S[0].insert(e);
            NU.erase(e);
            n++;
        }
    }
    US.insert(S[0].begin(),S[0].end());
    int m=1;
    while(NU.size()>=20)
    {
        n=rand()%20+1;
        int x = rand()%n+1;
        for(int i=0;i<x;i++){
            e=getRandom(NU);
            S[m].insert(e);
            NU.erase(e);
        }
        US.insert(S[m].begin(),S[m].end());
        for(int i=0;i<n-x;i++){
            e = getRandom(US);
            while(S[m].count(e)==1){
                e = getRandom(US);
            }
            S[m].insert(e);
        }
        m++;
    }
    S[m]=NU;
    for(int i=m+1;i<num;i++){
        n=rand()%30+1;
        for(int j=0;j<n;j++){
            S[i].insert(rand()%num);
        }
    }
    clock_t start=clock();
    greedySetCover1(S,num);
    /*
    for(int i=0;i<num;i++){
        cout<<"\nS"<<i;
        outputSet(S[i]);
    }*/
    clock_t end1=clock();
    set<int>* EC=new set<int>[num];//表示每个元素被那些表包含
    for(int i=0;i<num;i++){
        for(int j=0;j<num;j++){
            if(S[j].count(i)>0){
                EC[i].insert(j);
            }
        }
    }
    //delete[] S;

    glp_prob *lp;
    ia=new int[num*num];
    ja=new int[num*num];//ia代表行坐标，ja代表列坐标
    ar=new double[num*num];
    double* x=new double[num];//代表集合是否被选择
    double z;//线性规划优化值
    lp = glp_create_prob();
    glp_set_prob_name(lp,"set cover");
    glp_set_obj_dir(lp,GLP_MIN);//maximization
    glp_add_rows(lp,num);
    for(int i=0;i<num;i++){
	//cout<<"\n"<<"r_"+i<<" "<<i;
        //glp_set_row_name(lp, i+1, "r_"+i);
        glp_set_row_bnds(lp, i+1, GLP_LO, 1.0,0.0);//GLP_LO means it has a upper bound
    }
    glp_add_cols(lp, num);
    for(int i=0;i<num;i++){
        //glp_set_col_name(lp, i+1,"x"+(i+1));
        glp_set_col_bnds(lp, i+1, GLP_LO, 0.0,0.0);//GLP_LO means it has a lower bound
        glp_set_obj_coef(lp, i+1, 1.0);//setobjective coefficient目标函数系数
    }
    int np=0;
    int row=0;
    int f=0;//所有e的频率不超过f
    for(int i=0;i<num;i++){
        row+=1;
        if(f<EC[i].size()){
            f=EC[i].size();
        }
        set<int>::iterator ite1 = EC[i].begin();
        set<int>::iterator ite2 = EC[i].end();
        for(;ite1!=ite2;ite1++)
        {
            np+=1;
            ia[np]=row,ja[np]=(*ite1)+1,ar[np]=1;
        }
    }

    glp_load_matrix(lp, np, ia, ja, ar); //将约束系数矩阵导入
    glp_simplex(lp, NULL); //simplex method使用单纯形法求解
    z = glp_get_obj_val(lp); //获取目标函数最大值
    cout<<"\nLinear programming";
    for(int i=0;i<num;i++){
        x[i]=glp_get_col_prim(lp, i+1);//获取目标函数最大值时相应结构变量的值
        if(x[i] > 1.0/f && num<=500){
            cout<<"\nS"<<i;
            outputSet(S[i]);
        }
    }
    cout<<"\nz = "<<z;
    glp_delete_prob(lp);
    clock_t end2=clock();
    cout<<"\nTIME "<<(end1-start)*1000/CLOCKS_PER_SEC;
    cout<<"\nTIME "<<(end2-end1)*1000/CLOCKS_PER_SEC;
    cout<<"\n";
    getchar();
}
