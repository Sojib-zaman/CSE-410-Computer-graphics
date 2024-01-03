#include<bits/stdc++.h>
using namespace std ; 
#include "helper.h"
class matrix
{
    public:
        double arr[4][4]  ; 
        matrix()
        {
            for(int i=0 ; i<4 ; i++)
                for(int j=0 ; j<4 ; j++)
                    arr[i][j] = 0 ; 
        }
        void make_identity()
        {
            for(int i=0 ; i<4 ; i++)
                    arr[i][i] = 1 ; 
        }

        matrix matrix_multiplication(matrix x)
        {
            matrix retval ; 
            for(int i=0 ; i<4 ; i++)
                for(int j=0 ; j<4 ; j++)
                    for(int k=0 ; k<4 ; k++)
                        retval.arr[i][j]+=arr[i][k]*x.arr[k][j] ; 
            return retval;
            
        }




        void translation(point p)
        {
            double val[]={p.getx() , p.gety() , p.getz()} ; 
            make_identity() ; 
            for(int i=0 ; i<3 ; i++)
                arr[i][3]=val[i] ;     
        }
        void rotation(double angle , point axis)
        {
            point i(1,0,0) , j(0,1,0) , k(0,0,1)  ; 
            axis=axis.normalize() ; 
            point c1 = rodrigues(i,axis,angle) ; 
            point c2 = rodrigues(j,axis,angle) ; 
            point c3 = rodrigues(k,axis,angle) ; 

            make_identity() ; 
            
            arr[0][0] = c1.x  ; 
            arr[1][0] = c1.y  ; 
            arr[2][0] = c1.z  ; 
            arr[0][1] = c2.x  ; 
            arr[1][1] = c2.y  ; 
            arr[2][1] = c2.z  ; 
            arr[0][2] = c3.x  ; 
            arr[1][2] = c3.y  ; 
            arr[2][2] = c3.z  ; 
            
        }
        void scaling(point p)
        {
            double val[]={p.getx() , p.gety() , p.getz()} ; 
            for(int i=0 ; i<3 ; i++)
                    arr[i][i] = val[i] ;
            arr[3][3]=1 ;
             
        }
        void show(string matrixname)
        {
            cout<<matrixname<<endl ; 
            for(int i=0 ; i<4 ; i++)
            {
                for(int j=0 ; j<4 ; j++)
                    {
                        cout<<arr[i][j]<<" ";
                    }
            cout<<endl ;     
            }    
            cout<<"------------------"<<endl ; 

        }

        // basically doing a 4*4 * 4*1 multiplication 
        point transformation(point p)
        {
            show("transformation") ; 
            cout<<p.x<<" "<<p.y<<" "<<p.z<<endl ; 

            double retval[4]={0.0} ; 
            double pv[4] = {p.x , p.y , p.z , p.w} ; 
            for (int i = 0; i< 4; i++) {
                for (int j = 0; j < 4; ++j) {
                    retval[i] += arr[i][j] * pv[j];
                }
            
            }
        point newpoint(retval[0]/retval[3] ,retval[1]/retval[3] , retval[2]/retval[3] , 1 ) ; 
        return newpoint ;
        }
};