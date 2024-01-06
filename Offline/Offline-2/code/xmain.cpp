#include<bits/stdc++.h>
#include<fstream>
using namespace std; 
#include "helper.hpp"
#include "matrix.hpp"
#include "triangle.hpp"
#include <iomanip>
#include "bitmap_image.hpp"
#define INF 100000

ifstream in ; 
ifstream in2; 
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


//code according to spec
static unsigned long int g_seed = 1;
inline int random_color()
{
g_seed = (214013 * g_seed + 2531011);
return (g_seed >> 16) & 0x7FFF;
}


int main()
{
    point eye , look , up ; 
    double fovy , aspect_ratio, near_plane , far_plane ; 
    int trianglecount = 0 ; 
    string inputfile="scene.txt" ; 
    string outputfile="stage1.txt" ; 
    in.open(inputfile) ; 
    out.open(outputfile) ; 
    eye=point_input() ; 
    look=point_input() ; 
    up=point_input() ; 

    in>>fovy>>aspect_ratio>>near_plane>>far_plane;

 


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
            trianglecount++ ; 
            
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
            mst.push(mst.top()) ; //! ....
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


    in.open("stage1.txt") ; 
    out.open("stage2.txt") ; 
    
    matrix lookat  ; 
    lookat=lookat.viewTransform(eye , look , up ) ;

    for(int i=0 ; i<trianglecount ; i++)
    {
        point p1,p2,p3 ; 
        p1=point_input() ;  p2=point_input() ;  p3=point_input() ;
        p1=lookat.transformation(p1) ; 
        p2=lookat.transformation(p2) ;
        p3=lookat.transformation(p3) ; 
        point_output(p1) ;
        point_output(p2) ;
        point_output(p3) ;  
        out<<endl ;
    }
    in.close() ; 
    out.close();

    in.open("stage2.txt") ;
    out.open("stage3.txt") ; 
    matrix perspective ; 
    perspective=perspective.projection(fovy , aspect_ratio , near_plane , far_plane) ; 
    for(int i=0 ; i<trianglecount ; i++)
    {
        point p1,p2,p3 ; 
        p1=point_input() ;  p2=point_input() ;  p3=point_input() ;
        p1=perspective.transformation(p1) ; 
        p2=perspective.transformation(p2) ;
        p3=perspective.transformation(p3) ; 
        point_output(p1) ;
        point_output(p2) ;
        point_output(p3) ;  
        out<<endl ;
    }
    in.close() ; 
    out.close();



    in.open("stage3.txt") ; 
    in2.open("config.txt") ; 
    out.open("z_buffer.txt")  ; 
    int Screen_Width , Screen_Height ; 
    in2>>Screen_Width>>Screen_Height ; 
    in2.close() ; 
    vector<triangle>tri_vec ; 
    //stage3 will contain exactly no_of_triangles*3(for each point) lines
    for(int i=0 ; i<trianglecount ; i++)
    {
        point a = point_input() ; 
        point b = point_input() ; 
        point c = point_input() ; 
        triangle t(a,b,c,random_color()%256,random_color()%256,random_color()%256) ; 
        tri_vec.push_back(t) ; 
    }

    double Xrightlimit = 1.0 ; 
    double Xleftlimit = -1.0 ; 
    double Ybottomlimit = -1.0 ; 
    double Ytoplimit = 1.0 ; 

    double dx = (Xrightlimit - Xleftlimit)  /Screen_Width ; 
    double dy = (Ytoplimit - Ybottomlimit)  /Screen_Height ; 
    double topY = Ytoplimit - dy/2; 
    double leftX = Xleftlimit + dx/2 ; 
    double bottomY = Ybottomlimit + dy/2; //needed for clipping
    double rightX = Xrightlimit - dx/2 ;  // ""
    double zmax = 1.0 ; 

    double** zbuffer = new double*[Screen_Height] ; 
    for(int i=0 ; i<Screen_Height ; i++)
    {
        zbuffer[i] = new double[Screen_Width] ; 
        for(int j=0 ; j<Screen_Width ; j++) zbuffer[i][j] = zmax ; 
    }    
    bitmap_image bimg = bitmap_image(Screen_Width , Screen_Height) ; 
    for(int i=0 ; i<Screen_Width ; i++)
    {
        for(int j=0 ; j<Screen_Height ; j++)
            bimg.set_pixel(i,j,0,0,0) ; 
    }
    for(int i=0 ; i<trianglecount ; i++)
    {

        triangle current_triangle = tri_vec[i]  ;

        
        double top_scanline = min(topY , current_triangle.getmaxY()); 
        double bottom_scanline = max( Ybottomlimit , current_triangle.getminY()); 

        int top_linecount = (int)round((topY-top_scanline)/dy) ; 
        int bottom_linecount = Screen_Height+(int)round((Ybottomlimit-bottom_scanline)/dy) ; 



        for(double row_no = top_linecount ; row_no<=bottom_linecount ; row_no++ )
        {
            double current_Ys = top_scanline-row_no*dy ; 
            double xa = 0 , xb = 0 ; 
            pair<double,double>pab ; 
            pab=current_triangle.getintersectioncolumn(current_Ys) ; 


            double left_scanline = min(pab.first , pab.second) ; 
            double right_scanline =max(pab.first , pab.second) ; 
            if(left_scanline<leftX)left_scanline=leftX ; 
            if(right_scanline>rightX)right_scanline=rightX ; 

            for(double column = left_scanline ; column<=right_scanline ; column+=dx)
            {
                double z_value = current_triangle.calculateZvalue(column,current_Ys,left_scanline,right_scanline) ; 
                zbuffer[row_no][(column-left_scanline)/dx] = 
            }
        }







    }

}
