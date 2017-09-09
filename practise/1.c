#include<stdio.h>
#include<iostream>
using namespace std;     

void InsertSort(int a[], int n)    
{    
    for(int i= 1; i<n; i++)
    {    
        if(a[i] < a[i-1]){               //若第i个元素大于i-1元素，直接插入。小于的话，移动有序表后插入    
            int j= i-1;                  //表中最后一个数据  
            int x = a[i];               //复制为哨兵，即存储待排序元素    
            a[i] = a[i-1];               //先后移一个元素 (因为a[i]就是X，所以不怕丢失)   
            while(j>=0 && x < a[j])
            {        //查找在有序表的插入位置  (遍历表)  
                a[j+1] = a[j];    
                j--;                      //元素后移    
            }    
            a[j+1] = x;                //插入到正确位置    
        }    
    }    
        
}   
int main()  
{  
    int n;  
    cin>>n;  
    int *a=new int[n];  
    for(int j=0;j<n;j++)  
        cin>>a[j];  
    InsertSort(a,n);  
    for(int i=0;i<n;i++)  
        cout<<a[i];  
    delete []a;  
}  
