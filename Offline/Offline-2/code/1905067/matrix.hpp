#include<bits/stdc++.h>
using namespace std ; 

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



        matrix viewTransform(point eye , point look , point up)
        {
            point l = look.subtraction(eye) ; 
            l=l.normalize() ; 
            point r=l.crossproduct(up) ; 
            r=r.normalize() ; 
            point u = r.crossproduct(l) ; 
            translation(eye.scalarMul(-1)) ; // now our arr is the matrix T 
            // create R and multiply with T
            matrix R ; 
            R.make_identity()  ; 
            R.arr[0][0] = r.x  ; R.arr[0][1] = r.y ; R.arr[0][2] = r.z; 
            R.arr[1][0] = u.x  ; R.arr[1][1] = u.y ; R.arr[1][2] = u.z; 
            R.arr[2][0] = -l.x  ; R.arr[2][1] = -l.y ; R.arr[2][2] = -l.z; 

            matrix retval = matrix_multiplication(R) ; 
            return retval ; 

        }
        matrix projection(double fovy , double aspect_ratio , double near_plane , double far_plane)
        {
            double fovx = fovy*aspect_ratio ; 
            double t = near_plane*tan(fovy*0.5*acos(-1)/180)  ; 
            double r = near_plane*tan(fovx*0.5*acos(-1)/180)  ;
            matrix P ; 
            P.make_identity() ; 
            P.arr[0][0] = near_plane/r ; 
            P.arr[1][1] = near_plane/t ; 
            P.arr[2][2] = -1*(far_plane+near_plane)/(far_plane-near_plane) ; 
            P.arr[2][3] = -(2*far_plane*near_plane)/(far_plane-near_plane) ; 
            P.arr[3][2] = -1 ; 
            P.arr[3][3] = 0 ; 
            return P ; 
        }
};