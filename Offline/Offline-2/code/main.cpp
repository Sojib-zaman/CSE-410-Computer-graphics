#include<bits/stdc++.h>
#include<fstream>
using namespace std; 
#include "matrix.h"
#include <iomanip>



ifstream in ; 
ofstream out ; 
point point_input()
{
    point np  ; 
    in>>np.x>>np.y>>np.z ; 
    return np ; 
}
void point_output(point p)
{
    out<<fixed<<setprecision(7)<<p.x<<" "<<p.y<<" "<<p.z<<endl ;
}



int main()
{
    point eye , look , up ; 
    double fov , aspect_ratio, near_plane , far_plane ; 

    string inputfile="scene.txt" ; 
    string outputfile="stage1.txt" ; 
    in.open(inputfile) ; 
    out.open(outputfile) ; 
    eye=point_input() ; 
    look=point_input() ; 
    up=point_input() ; 

    in>>fov>>aspect_ratio>>near_plane>>far_plane;

 


    stack<matrix> mst  ;
    // initially an identity matrix on the stack 
    matrix x ; 
    x.make_identity(); 
    mst.push(x) ; 


    bool end = false ; 
    string command = "" ; 
    while (!end)
    {
        in>>command ; 
        
        if(command=="triangle")
        {
            
            point ta , tb , tc ; 
            ta=point_input() ; 
            tb=point_input() ; 
            tc=point_input() ; 

            ta = mst.top().transformation(ta) ; 
            tb = mst.top().transformation(tb) ; 
            tc = mst.top().transformation(tc) ; 

            point_output(ta) ; 
            point_output(tb) ; 
            point_output(tc) ; 

            out<<" "<<endl ; 
            



        }
        else if(command=="translate")
        {
            point trans_point ; 
            trans_point=point_input() ; 
            matrix translationMatrix ; 
            translationMatrix.translation(trans_point); 
            translationMatrix=mst.top().matrix_multiplication(translationMatrix) ; 
            mst.pop() ; 
            mst.push(translationMatrix) ; 



        }
        else if(command=="scale")
        {
            point scale_amount ; 
            scale_amount=point_input() ; 
            matrix scaleMatrix ; 
            scaleMatrix.scaling(scale_amount) ; 
            scaleMatrix=mst.top().matrix_multiplication(scaleMatrix) ; 
            mst.pop() ; 
            mst.push(scaleMatrix) ; 
        }
        else if(command=="rotate")
        {
            point rotation_axis ; 
            double rotation_angle ; 
            in>>rotation_angle  ; 
            rotation_axis = point_input() ; 
            matrix rotationMatrix ; 
            rotationMatrix.rotation(rotation_angle , rotation_axis) ; 
            rotationMatrix=mst.top().matrix_multiplication(rotationMatrix) ; 
            mst.pop() ; 
            mst.push(rotationMatrix) ;  

        }
        else if(command=="push")
        {
            mst.push(mst.top()) ; //! why 
        }
        else if(command=="pop")
        {
            if(!mst.empty())mst.pop() ; 

        }
        else if(command=="end")
        {
            end=true ; 
        }
    }
    in.close() ; 
    out.close() ; 
    

}