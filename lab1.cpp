#include <stdio.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
using namespace std;

int Rand_Partition(int* A,int p,int r){
    int i = rand()%(r-p+1)+p;
    int x,y;
    x=A[r];
    A[r]=A[i];
    A[i]=x;
    x=A[r];//将随机选择的元素x，交换到最末尾的坐标
    i=p-1;
    for(int j=p;j<=r-1;j++){
        if (A[j] <= x){
            i = i + 1;
            y=A[j];
            A[j]=A[i];
            A[i]=y;//小于x的元素都被交换到前部分
        }
    }
    y=A[r];
    A[r]=A[i+1];
    A[i+1]=y;
    return i+1;
}

void QuickSort(int* A,int p,int r){
    if(p<r){
        int q= Rand_Partition(A,p,r);//q是划分的界限
        QuickSort(A,p,q-1);
        QuickSort(A,q+1,r);//递归执行
    }
}

int main()
{
    srand(unsigned(time(0)));
    int n,plan,ca;
    cout<<"input plan:(0:input num;1:1000000 32 bit or 1;2:differ num)\n";
    cin>>plan;
    cout<<"call STL?(1:true;0:false)\n";
    cin>>ca;
    int* A;
    clock_t t0;
    clock_t t1;
    if(plan==0){
        cout<<"input num:\n";
        cin>>n;
        A = new int[n];
        for(int i=0;i<n;i++){
            A[i]=rand();
        }
        if(ca==0){
            QuickSort(A,0,n-1);
        }else{
            sort(A,A+n);
        }
        cout<<"result:\n";
        for(int i=0;i<n;i++){
            cout<<A[i]<<" ";
        }
        delete [] A;
    }else if(plan==1){
        cout<<"n=1000000\n";
        n=1000000;
        cout<<"random 32 bit integer\n";
        A = new int[n];
        for(int i=0;i<n;i++){
            A[i]=rand();
        }
        t0=clock();
        if(ca==0){
            QuickSort(A,0,n-1);
        }else{
            sort(A,A+n);
        }
        t1=clock();
        delete [] A;
        cout<<"TIME "<<(t1-t0)*1000/CLOCKS_PER_SEC<<"\n";
        cout<<"all is 1\n";
        A = new int[n];
        for(int i=0;i<n;i++){
            A[i]=1;
        }
        t0=clock();
        if(ca==0){
            QuickSort(A,0,n-1);
        }else{
            sort(A,A+n);
        }
        t1=clock();
        delete [] A;
        cout<<"TIME "<<(t1-t0)*1000/CLOCKS_PER_SEC<<"\n";
    }else if(plan==2){
        cout<<"n=10000\n";
        n=10000;
        for(int x=5;x<=10;x++){
            A = new int[n];
            for(int i=0;i<n;i++){
                if(i%10<x){
                    A[i]=1;
                }else{
                    A[i]=rand();
                }
            }
            t0=clock();
            if(ca==0){
                QuickSort(A,0,n-1);
            }else{
                sort(A,A+n);
            }
            t1=clock();
            delete [] A;
            cout<<"x="<<x*10<<" TIME "<<(t1-t0)*1000/CLOCKS_PER_SEC<<"\n";
        }
        cout<<"n=1000\n";
        n=1000;
        for(int x=5;x<=10;x++){
            A = new int[n];
            for(int i=0;i<n;i++){
                if(i%10<x){
                    A[i]=1;
                }else{
                    A[i]=rand();
                }
            }
            t0=clock();
            if(ca==0){
                QuickSort(A,0,n-1);
            }else{
                sort(A,A+n);
            }
            t1=clock();
            delete [] A;
            cout<<"x="<<x*10<<" TIME "<<(t1-t0)*1000/CLOCKS_PER_SEC<<"\n";
        }
        cout<<"n=5000\n";
        n=5000;
        for(int x=5;x<=10;x++){
            A = new int[n];
            for(int i=0;i<n;i++){
                if(i%10<x){
                    A[i]=1;
                }else{
                    A[i]=rand();
                }
            }
            t0=clock();
            if(ca==0){
                QuickSort(A,0,n-1);
            }else{
                sort(A,A+n);
            }
            t1=clock();
            delete [] A;
            cout<<"x="<<x*10<<" TIME "<<(t1-t0)*1000/CLOCKS_PER_SEC<<"\n";
        }
    }else{
        for(n=1000;n<=10000000;n*=2){
            A = new int[n];
            for(int i=0;i<n;i++){
                A[i]=rand();
            }
            t0=clock();
            if(ca==0){
                QuickSort(A,0,n-1);
            }else{
                sort(A,A+n);
            }
            t1=clock();
            delete [] A;
            cout<<"n="<<n<<" TIME "<<(t1-t0)*1000/CLOCKS_PER_SEC<<"\n";
        }
    }

	getchar();
}
