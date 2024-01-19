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


    //> READ DATA START 
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
        triangle t = triangle(a,b,c) ;
        t.setcolor() ; 
        tri_vec.push_back(t) ; 
    }
    //+ READ DATA END 


    //> Initialize z buffer and frame buffer start 
    double XRightLimit = 1.0 ; 
    double XLeftLimit = -1.0 ; 
    double YBottomLimit = -1.0 ; 
    double YTopLimit = 1.0 ; 
    double zmax = 1.0; 

    // find the length of each pixel in dx and dy 
    double dx = (XRightLimit - XLeftLimit)  /Screen_Width ; 
    double dy = (YTopLimit - YBottomLimit)  /Screen_Height ; 

    //specify Top_Y and Left_X values.
    double Top_Y = YTopLimit - dy/2 ; 
    double Left_X = XLeftLimit + dx/2 ;

    //also the bottom and right 
    double Bottom_Y = YBottomLimit + dy/2; 
    double Right_X = XRightLimit - dx/2 ;

    // initialize Z buffer 
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
    //+ Initialize z buffer and frame buffer end




    //> Z buffer Algorithm start 
    for(triangle t:tri_vec)
    {
        
         
        //? The value of top scan and bottom scan line 
        double top_scanlineDistance , bottom_scanelineDistance ;
        
        //? indicates the scaled version of topscanline and bottomscanline 
        int top_scanline , bottom_scanline ; 

        //Find top_scanline and bottom_scanline after necessary clipping
        //- TopScanLine 
        if(t.getmaxY() > Top_Y) //If max_y > Top_Y, clip (i.e. ignore) the portion above Top_Y and start scanning from Top_Y.
            top_scanlineDistance = Top_Y , top_scanline = 0 ; 
        else top_scanlineDistance = t.getmaxY() , top_scanline = (int)round((Top_Y-t.getmaxY())/dy) ; 

        //-BottomScanLine
        if(t.getminY() < Bottom_Y) 
            bottom_scanelineDistance = Bottom_Y , bottom_scanline = Screen_Height-1 ; 
        else bottom_scanelineDistance = t.getminY() , bottom_scanline = (Screen_Height-1)-(int)round(t.getminY()-Bottom_Y)/dy; 
        cout<<"-----------"<<top_scanline<<" "<<bottom_scanline<<endl ;

        

        

        //& iterating over Ys begins from here 
        for(int row_no = top_scanline ; row_no<=bottom_scanline ; row_no++)
        {
            //Find left_intersecting_column and right_intersecting_column after necessary clipping
            double Ys = Top_Y-row_no*dy ; 
            if(Ys>t.getmaxY())continue; 
            double leftIntersectiondistance , RightIntersectiondistance ; 
            int leftIntersectingColumn , rightIntersectingColumn ; 
            vector<double>pab ; 
            pab = t.getISPoints(Ys);
            leftIntersectiondistance = pab[0] ; 
            RightIntersectiondistance= pab[1] ;

            if(leftIntersectiondistance<=Left_X) leftIntersectingColumn=0 ; 
            else leftIntersectingColumn = (int)round((-Left_X+leftIntersectiondistance)/dx) ; 

            if(RightIntersectiondistance>=Right_X)rightIntersectingColumn = Screen_Width-1 ; 
            else rightIntersectingColumn = (Screen_Width-1)-(int)round((Right_X-RightIntersectiondistance)/dx) ; 

           



            //cout<<"j -- left -- right "<<row_no<<" "<<leftIntersectingColumn<<" "<<rightIntersectingColumn<<endl ; 

            
            
            
            double za = t.vertices[t.minXind].z + (Ys - t.vertices[t.minXind].y) * (t.vertices[(t.minXind+1)%3].z - t.vertices[t.minXind].z) / (t.vertices[(t.minXind+1)%3].y-t.vertices[t.minXind].y) ; 
            double zb = t.vertices[t.maxXind].z + (Ys - t.vertices[t.maxXind].y) * (t.vertices[(t.maxXind+1)%3].z - t.vertices[t.maxXind].z) / (t.vertices[(t.maxXind+1)%3].y-t.vertices[t.maxXind].y) ; 
            double zp=-1 ; 
            double constant = (zb-za)*dx/(RightIntersectiondistance-leftIntersectiondistance) ; 
            cout<<constant<<endl ;


            for(int xx = leftIntersectingColumn ; xx<=rightIntersectingColumn ; xx++)
            {
                if(zp==-1)
                    zp = za ; 
                else zp = zp+constant ; 
                
            
                if(zp>-1 && zp<zbuffer[row_no][xx])
                {
                    zbuffer[row_no][xx] = zp ; 
                    bimg.set_pixel(xx,row_no,t.R,t.G,t.B) ; 

                }
                

            }


            



        //- row inner loop 
        }
      



    //- triangle ends here 
    }

    bimg.save_image("out.bmp");
    for(int i = 0; i < Screen_Height; i++) 
    {
        for(int j = 0; j < Screen_Width; j++) 
        {
            if(zbuffer[i][j] < 1) {
                out << fixed<<setprecision(6) << zbuffer[i][j] << "\t";
            }
        }
        out<<endl;
    }
    out.close();


    for (int i = 0; i < Screen_Height; i++) 
    {
        delete[] zbuffer[i];
    }
    delete[] zbuffer;

    return 0;
}
