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
        triangle t(a,b,c,random_color()%256,random_color()%256,random_color()%256) ; 
        tri_vec.push_back(t) ; 
    }
    //+ READ DATA END 


    //> Initialize z buffer and frame buffer start 
    double XRightLimit = 1.0 ; 
    double XLeftLimit = -1.0 ; 
    double YBottomLimit = -1.0 ; 
    double YTopLimit = 1.0 ; 

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
    double zmax = 1.0; 
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

    int tc = 0 ; 
    //> Z buffer Algorithm start 
    for(triangle t:tri_vec)
    {
        tc++ ;
        cout<<"tc 246 "<<tc<<endl ; 
         
        //? The value of top scan and bottom scan line 
        double top_scanlineDistance , bottom_scanelineDistance ;
        
        //? indicates the scaled version of topscanline and bottomscanline 
        //int top_scanline , bottom_scanline ; 



        //- TopScanLine 
        if(t.getmaxY() > Top_Y) //If max_y > Top_Y, clip (i.e. ignore) the portion above Top_Y and start scanning from Top_Y.
            top_scanlineDistance = Top_Y ; 
        else top_scanlineDistance = t.getmaxY() ; 

        //-BottomScanLine
        if(t.getminY() < Bottom_Y) 
            bottom_scanelineDistance = Bottom_Y ; 
        else bottom_scanelineDistance = t.getminY() ; 


        cout<<"------------------------"<<top_scanlineDistance<<" "<<bottom_scanelineDistance<<endl ; 

        int no_of_scanlines = ceil((top_scanlineDistance - bottom_scanelineDistance) /dy) ; 
        
        //Find top_scanline and bottom_scanline after necessary clipping
        //. Skipped , will be doing by accurate values of topscanline and bottomscanline distances 

        for(int row_no = 0 ; row_no<no_of_scanlines ; row_no++)
        {
            //Find left_intersecting_column and right_intersecting_column after necessary clipping
            double Ys = top_scanlineDistance-row_no*dy ; 
            double leftInterSectingPoint , RightIntersectingPoint ; 
            vector<double>pab ; 
            pab = t.getISPoints(Ys);
            leftInterSectingPoint = pab[0] ; 
            RightIntersectingPoint= pab[1]; 
            cout<<"left : "<<leftInterSectingPoint<<" right : "<<RightIntersectingPoint<<endl ; 

            // If min_x < Left_X, clip (i.e. ignore) the portion to the left of Left_X.

            // if(t.getmaxX() > Right_X) 
            //     RightIntersectingPoint = Right_X; 

            // if(t.getminX() < Left_X) 
            //     leftInterSectingPoint = Left_X ; 

            double za = t.vertices[t.minXind].z + (Ys - t.vertices[t.minXind].y) * (t.vertices[(t.minXind+1)%3].z - t.vertices[t.minXind].z) / (t.vertices[(t.minXind+1)%3].y-t.vertices[t.minXind].y) ; 
            double zb = t.vertices[t.maxXind].z + (Ys - t.vertices[t.maxXind].y) * (t.vertices[(t.maxXind+1)%3].z - t.vertices[t.maxXind].z) / (t.vertices[(t.maxXind+1)%3].y-t.vertices[t.maxXind].y) ; 
            double zp=-1 ; 
            double constant = (zb-za)*dx/(pab[1]-pab[0]) ; 
            cout<<"tc 297 "<<tc<<endl ; 

            cout<<dx<<endl;
            
            //! may need to do something like top and bottom 
            for(double xx = leftInterSectingPoint ; xx<=RightIntersectingPoint ; xx+=dx)
            {
                if(zp==-1)
                    zp = za ; 
                else zp = zp+dx*constant ; 
                
                
                int rc = (Top_Y-Ys)/dy ; 
                int cr = (xx-Left_X)/dx;
                if(zp>-1 && zp<zbuffer[cr][rc])
                {
                    zbuffer[cr][rc] = zp ; 
                    bimg.set_pixel(cr,rc,t.R,t.G,t.B) ; 

                }
                

            }
             cout<<"tc 316 "<<tc<<endl ; 

            



        //- row inner loop 
        }
         cout<<"tc 324 "<<tc<<endl ; 



    //- triangle ends here 
    }

    bimg.save_image("out.bmp");
    for(int i = 0; i < Screen_Height; i++) 
    {
        for(int j = 0; j < Screen_Width; j++) 
        {
            if(zbuffer[i][j] < 1) {
                out << fixed<<setprecision(7) << zbuffer[i][j] << "\t";
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
