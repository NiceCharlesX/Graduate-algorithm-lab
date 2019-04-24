#include <stdio.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <stack>
#include <set>

using namespace std;

void swap(double **A,int i,int j){
    double med0,med1;
    med0 = A[i][0];
    med1 = A[i][1];
    A[i][0]=A[j][0];
    A[i][1]=A[j][1];
    A[j][0]=med0;
    A[j][1]=med1;
}

double slope(double **A,int j){
    return (A[j][1]-A[0][1])/(A[j][0]-A[0][0]);
}

double returnX(double **A,int j){
    return A[j][0];
}

int Rand_Partition(double **A,int p,int r,double func(double**,int)){
    int i = rand()%(r-p+1)+p;
    swap(A,i,r);
    double x;
    x=func(A,r);
    i=p-1;
    for(int j=p;j<=r-1;j++){
        if ( func(A,j)<= x){
            i = i + 1;
            swap(A,i,j);
        }
    }
    swap(A,r,i+1);
    return i+1;
}

void QuickSort(double**A,int p,int r,double func(double**,int)){
    if(p<r){
        int q= Rand_Partition(A,p,r,func);
        QuickSort(A,p,q-1,func);
        QuickSort(A,q+1,r,func);
    }
}

void Intial_Partition(double**A,int n){
    int p=1,r=n-1;
    int i=p-1,k=n;
    for(int j=p;j<=r;j++){
        if(A[j][0]-A[0][0]==0){
            k=k-1;
            swap(A,k,j);
        }
        else if(slope(A,j)>=0){
            i = i + 1;
            swap(A,i,j);
        }
    }
    QuickSort(A,1,i,slope);
    for(int j=k;j<n;j++){
        i=i+1;
        swap(A,i,j);
    }
    QuickSort(A,i+1,n-1,slope);
}

bool isLeft(double**A,int i,int j,int k){
    if((A[k][0]-A[j][0])*(A[j][1]-A[i][1])-(A[k][1]-A[j][1])*(A[j][0]-A[i][0])<0){
        return true;
    }
    return false;
}

stack<int> Scan(double **A,int n,bool func(double **,int,int,int)){//扫描所有已经按极角排好序点，形成凸包
    stack<int> convex;
    int top=2,ntop=1;
    convex.push(0);
    while(slope(A,top)==slope(A,ntop)){
        if(A[top][1]>A[ntop][1]){
            ntop=top;
        }
        top+=1;
    }
    convex.push(ntop);
    for(int i=top+1;i<n;i++){
        if(slope(A,top)==slope(A,i)){
            if(A[top][1]>=A[i][1]){
                continue;
            }else{
                top=ntop;
                convex.pop();
            }
        }
        ntop=convex.top();
        while(!func(A,ntop,top,i)){//考虑是否是向左
            top=ntop;
            convex.pop();
            ntop=convex.top();
        }
        convex.push(top);
        top=i;
        ntop=convex.top();
    }
    convex.push(n-1);
    return convex;
}

stack<double*> Graham_Scan(double**A,int n){
    double miny=100;
    int j=0;
    for(int i=0;i<n;i++){
        if(A[i][1]<miny){
            j=i;
            miny = A[i][1];
        }
    }
    swap(A,j,0);//选择y坐标最小的点，并将其交换到数组头部
    Intial_Partition(A,n);//初始按照极角排序

    stack<int> convex= Scan(A,n,isLeft);
    stack<double*> result;
    while (!convex.empty()){
        double* point = new double[2];
        point[0]=A[convex.top()][0];
        point[1]=A[convex.top()][1];
        result.push(point);
		convex.pop();
	}
    return result;
}

set<int> Enumerate(double** A,int n){
    bool *f = new bool[n];
    for(int i=0;i<n;i++){
        f[i]=true;
    }
    set<int> result;
    for(int i=0;i<n;i++){
        if(f[i]==false){
            continue;
        }
        for(int j=i+1;j<n;j++)
        {
            if(f[j]==false){
                continue;
            }
            int k;
            double flag = 0;
            for(k=0;k<n;k++){
                if(k==j||k==i||f[k]==false){
                    continue;
                }
                double s = (A[j][0]-A[i][0])*(A[k][1]-A[i][1])-(A[j][1]-A[i][1])*(A[k][0]-A[i][0]);
                if(s==0){
                    if((A[i][0]-A[j][0])*(A[i][0]-A[k][0])<=0){
                        f[i]=false;
                        j=n;
                        break;//i节点处于中间位置，需要跳出两层循环
                    }
                    else if((A[j][0]-A[i][0])*(A[j][0]-A[k][0])<=0){
                        f[j]=false;
                        break;//j节点处于中间位置，跳出一层循环
                    }
                    f[k]=false;//k节点处于中间位置
                }
                else{
                    if(s*flag<0){//与之前的值不同侧
                        break;
                    }
                    flag = s;
                }
            }
            if(k==n){
                result.insert(i);
                result.insert(j);
            }
        }
    }
    return result;
}

stack<double*> dcConvex(double**A,int s,int e){
    stack<double*> CH;
    if(e-s==2){//逆时针输出三点
        double ss = (A[e][0]-A[s][0])*(A[s+1][1]-A[s][1])-(A[e][1]-A[s][1])*(A[s+1][0]-A[s][0]);
        if(ss>0){
            CH.push(A[s]);
            CH.push(A[e]);
            CH.push(A[s+1]);
        }else if(ss<0){
            CH.push(A[s]);
            CH.push(A[s+1]);
            CH.push(A[e]);
        }else{//考虑三点共线
            if((A[s][0]-A[e][0])*(A[s][0]-A[s+1][0])<=0){
                CH.push(A[e]);
                CH.push(A[s+1]);
            }
            else if((A[e][0]-A[s][0])*(A[e][0]-A[s+1][0])<=0){
                CH.push(A[s]);
                CH.push(A[s+1]);
            }
            else{
                CH.push(A[s]);
                CH.push(A[e]);
            }
        }
        return CH;
    }else if(e-s==1){
        CH.push(A[s]);
        CH.push(A[e]);
        return CH;
    }
    else if(s==e){
        CH.push(A[e]);
        return CH;
    }
    stack<double*> CHL = dcConvex(A,s,(e-s)/2+s);
    stack<double*> CHR = dcConvex(A,s+(e-s)/2+1,e);

    int m;
    m = CHL.size()+CHR.size();
    double **M = new double*[m];
    for(int i=0;i<m;i++){
        if(!CHL.empty()){
            double* p = CHL.top();
            M[i]=new double[2];
            M[i][0]=p[0];
            M[i][1]=p[1];
            CHL.pop();
        }else{
            double* p = CHR.top();
            M[i]=new double[2];
            M[i][0]=p[0];
            M[i][1]=p[1];
            CHR.pop();
        }
    }
    return Graham_Scan(M,m);
}


int main()
{
    srand(unsigned(time(0)));
    int n,plan;
    cout<<"plan:\n";
    cin>>plan;
    double **A;
    clock_t t0,t1;
    if(plan==0){
        cout<<"input num:\n";
        cin>>n;
        A = new double*[n];
        for(int i=0;i<n;i++){
            A[i]=new double[2];
            A[i][0]=(double)rand()/(double)RAND_MAX*100;
            A[i][1]=(double)rand()/(double)RAND_MAX*100;
        }
        QuickSort(A,0,n-1,returnX);
        stack<double*> dresult= dcConvex(A,0,n-1);
        cout<<"\nDivide&conquer output:\n";
        while(!dresult.empty()){
            double* point;
            point = dresult.top();
            cout<<"("<<point[0]<<",";
            cout<<point[1]<<") ";
            dresult.pop();
        }

        stack<double*> gresult= Graham_Scan(A,n);
        cout<<"\nGraham-Scan output:\n";
        while(!gresult.empty()){
            double* point;
            point = gresult.top();
            cout<<"("<<point[0]<<",";
            cout<<point[1]<<") ";
            gresult.pop();
        }

        cout<<"\nEnumerate:\n";
        set<int> eresult=Enumerate(A,n);
        set<int>::iterator ite1 = eresult.begin();
        set<int>::iterator ite2 = eresult.end();
        for(;ite1!=ite2;ite1++)
        {
            cout<<"("<<A[*ite1][0]<<",";
            cout<<A[*ite1][1]<<") ";
        }
        delete[] A;
    }else if(plan==1){
        for(n=250;n<=3000;n+=250){
            A = new double*[n];
            for(int i=0;i<n;i++){
                A[i]=new double[2];
                A[i][0]=(double)rand()/(double)RAND_MAX*100;
                A[i][1]=(double)rand()/(double)RAND_MAX*100;
            }
            t0=clock();
            QuickSort(A,0,n-1,returnX);
            dcConvex(A,0,n-1);
            t1=clock();
            cout<<"n="<<n<<" time D="<<(t1-t0)*1000/CLOCKS_PER_SEC;
            t0=clock();
            Graham_Scan(A,n);
            t1=clock();
            cout<<" G="<<(t1-t0)*1000/CLOCKS_PER_SEC;
            t0=clock();
            Enumerate(A,n);
            t1=clock();
            cout<<" E="<<(t1-t0)*1000/CLOCKS_PER_SEC;
            cout<<"\n";
            delete[] A;
        }
    }else{
        n=8;
        A = new double*[n];
        double B[8][2]= {{0,0},{1,1},{1.5,1.5},{3,3},{1,2},{1,1},{3,6},{-3,6}};
        for(int i=0;i<n;i++){
            A[i]=new double[2];
            A[i][0]=B[i][0];
            A[i][1]=B[i][1];
        }
        QuickSort(A,0,n-1,returnX);
        stack<double*> dresult= dcConvex(A,0,n-1);
        cout<<"\nDivide&conquer output:\n";
        while(!dresult.empty()){
            double* point;
            point = dresult.top();
            cout<<"("<<point[0]<<",";
            cout<<point[1]<<") ";
            dresult.pop();
        }

        cout<<"\nGraham-Scan output:\n";
        stack<double*> gresult= Graham_Scan(A,n);
        while(!gresult.empty()){
            double* point;
            point = gresult.top();
            cout<<"("<<point[0]<<",";
            cout<<point[1]<<") ";
            gresult.pop();
        }

        cout<<"\nEnumerate:\n";
        set<int> eresult=Enumerate(A,n);
        set<int>::iterator ite1 = eresult.begin();
        set<int>::iterator ite2 = eresult.end();
        for(;ite1!=ite2;ite1++)
        {
            cout<<"("<<A[*ite1][0]<<",";
            cout<<A[*ite1][1]<<") ";
        }
        delete[] A;
    }


	getchar();
}
