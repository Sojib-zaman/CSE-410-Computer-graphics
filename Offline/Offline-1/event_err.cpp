#include <bits/stdc++.h>
using namespace std ; 
#include <ctime>
#include <cstdlib>
#include <chrono>

#ifdef __linux__
    #include <GL/glut.h>
#elif 
    #include <windows.h>
    #include <glut.h>
#endif
#include<math.h>
#include <chrono>
#define pi (2*acos(0.0)) 
#define red 1
#define blue 2
#define green 3
#define yellow 4
#define cyan 5
#define magenta 6
#define black 7
#define white 8
double bar_x = 100 ; 
double bar_y = 100 ; 
double bar_width= 5.0;
double bar_len= 100;
int collision_count = 0 ; 
auto start_time = chrono::high_resolution_clock::now() ; 
double jl_angle = 20 ; 

class point
{
    double x ; 
    double y ; 
    double z ; 
    public : 
    point()
    {
        x=0 ; 
        y=0 ; 
        z=0;
    }
    point(double a ,double b , double c)
    {
        x=a ; 
        y=b; 
        z=c ; 

    }
    double getx(){return x;} 
    double gety(){return y;} 
    double getz(){return z;} 

    void setx(double a){x=a;}
    void sety(double a){y=a;}
    void setz(double a){z=a;}

    point addition(point p)
    {
        point newpoint(x+p.getx() , y+p.gety() , z+p.getz()) ; 
        return newpoint ; 
    }
    
    point subtraction(point p)
    {
        point newpoint(x-p.getx() , y-p.gety() , z-p.getz()) ; 
        return newpoint ; 
    }

   point scalarMul(double n)
    {
        point newpoint(x*n , y*n, z*n) ; 
        return newpoint ; 
    }

   point crossMul(point p)
    {
        point newpoint(y*p.getz()-z*p.gety() ,z*p.getx()-x*p.getz() , x*p.gety()-y*p.getx() ) ; 
        return newpoint ; 
    }
    void show(string str)
    {
        cout<<str<<" "<<x<<","<<y<<","<<z<<"---" ; 
    }


    


};





bool manual_ball_control = true ;
int ball_angle = 0  ;
int up_arrow_show = 0 ;  
point position_of_camera ; 
point camera_up ; 
point camera_look ; 
point camera_right ; 
int ball_rolling_angle = 0 ; 
int rolling_amount = 5 ; 
point ball_position ; 
double movement_amount = 5.0 ; 
point adv_point;
int square_length =15.0 ; 
point ball_look ; 
point ball_right ; 
point ball_up ; 
bool roll=false ; 
point bprev_right ; 
double ball_radius = 5.0 ; 


#define posX 100
#define negX 101 
#define posY 102
#define negY 103


class Event 
{
    public:
        double eventScheduledTime ;
        int collision_count ; 
        int wall_side ; 
    Event()
    {
        eventScheduledTime = 0 ; 
        collision_count = 0 ; 
        wall_side=-1 ;
    }
    Event(double a , int b , int w )
    {
        eventScheduledTime = a ; 
        collision_count = b ; 
        wall_side = w; 
    }
   

    bool operator<(const Event& other) const {
        return eventScheduledTime > other.eventScheduledTime; 
    }




};

priority_queue<Event> Event_PQ ;
void showPQ() {
    std::priority_queue<Event> tempQueue = Event_PQ;  // Create a copy of the original priority queue

    while (!tempQueue.empty()) {
        Event currentEvent = tempQueue.top();

        // Process the event (print for demonstration)
        std::cout << "Time: " << currentEvent.eventScheduledTime
                  << ", Collision Count: " << currentEvent.collision_count << std::endl;

        tempQueue.pop();  // Move to the next element in the temporary queue
    }
}
void check_reflection(double bar_x , double bar_y , double bar_len , double bar_width)
{
    // ball_position.show("ball position") ; 
    // cout<<endl ; 
    // cout<<ball_angle<<endl ; 

    if(ball_position.getx()+ball_radius+bar_width>=bar_x+bar_len || ball_position.getx() <= bar_x+ball_radius+bar_width)
    {
        ball_look.setx(-1*ball_look.getx()) ;
        ball_right.setx(-1*ball_right.getx()) ;
        ball_angle=180+ball_angle ; 
        collision_count++ ; 
    }
    if(ball_position.gety()>=bar_y+bar_len-ball_radius-bar_width || ball_position.gety() <= bar_y+ball_radius+bar_width)
    {
        ball_look.sety(-1*ball_look.gety()) ;
        ball_right.sety(-1*ball_right.gety()) ;
        ball_angle=180+ball_angle ; 
        collision_count++ ; 
    }

}

struct Sphere_point
{
	double x,y,z;
};

double angle_converter(double degree) {
    return degree * M_PI / 180.0;
}
void dis_calc()
{
    double initDisx1 = bar_x+bar_len-ball_position.getx()-ball_radius;
    double initDisx2 = -bar_x+ball_position.getx()-ball_radius;
    double initDisy1 = bar_y+bar_len-ball_position.gety()-ball_radius;
    double initDisy2 = -bar_y+ball_position.gety()-ball_radius;
    cout<<initDisx1<<" "<<initDisx2<<" "<<initDisy1<<" "<<initDisy2<<endl ;

    // time calculation 
    double tx1 = -1 , tx2 = -1 ; 
    double ty1 = -1 , ty2 = -1 ; 
    if(ball_look.getx()!=0)
    {
            tx1 = initDisx1/ball_look.getx() ; 
            tx2 = -initDisx2/ball_look.getx() ; 
    }
    if(ball_look.gety()!=0)
    {
            ty1 = initDisy1/ball_look.gety() ; 
            ty2 = -initDisy2/ball_look.gety() ; 
    }
    cout<<tx1<<" "<<tx2<<" "<<ty1<<" "<<ty2<<endl ; 
    auto xx = chrono::high_resolution_clock::now() ;
    chrono::duration<double> cc = xx - start_time ; 
    vector<double>colltime  ; 
    colltime.push_back(tx1) ; 
    colltime.push_back(tx2) ; 
    colltime.push_back(ty1) ; 
    colltime.push_back(ty2) ; 
    cout<<"cc time : "<<cc.count()<<endl ; 
    vector<int> wallside = {posX , negX , posY , negY} ; 
    for(int i=0 ; i<4 ; i++)
    {
        if(colltime[i]>0)
        {
            Event E= Event(colltime[i]+cc.count() , collision_count , wallside[i]) ; 
            Event_PQ.push(E) ; 
        }
    }




   
}

point RodriGeneral(point rotatingVector , point withRespectTo , double angle)
{
    point perpendicular ; 
    perpendicular = rotatingVector.crossMul(withRespectTo) ;
    //perpendicular = ball_up ; 
    //perpendicular.show("perpendicular") ;  
    point newpoint ; 
    newpoint = rotatingVector.scalarMul(cos(angle_converter(angle))).addition(perpendicular.scalarMul(sin(angle_converter(angle)))) ; 
    return newpoint ; 

}
void keyboard(unsigned char key , int a , int b)
{
    // position_of_camera.show(); 
    // camera_look.show() ; 
    // camera_up.show() ; 
    // camera_right.show() ; 
    switch (key)
    {
    case '1':
        camera_look = RodriGeneral(camera_look , camera_up , 0.1) ; 
        camera_right = RodriGeneral(camera_right , camera_up , 0.1) ; 
        // to make sure rodr. works , the changed vector's axis needs to be changed at that degree to make sure they are still unit and perpendicular 
        break;
    case '2':
        camera_look = RodriGeneral(camera_look , camera_up , -0.1) ; 
        camera_right = RodriGeneral(camera_right , camera_up , -0.1) ; 
        break ; 
    case '3':
        camera_look = RodriGeneral(camera_look , camera_right , 0.1) ; 
        camera_up = RodriGeneral(camera_up , camera_right , 0.1) ; 
        break;
    case '4':
        camera_look = RodriGeneral(camera_look , camera_right , -0.1) ; 
        camera_up = RodriGeneral(camera_up , camera_right , -0.1) ; 

        break;
    case '5':
        camera_right = RodriGeneral(camera_right , camera_look , 0.1) ; 
        camera_up = RodriGeneral(camera_up , camera_look , 0.1) ; 
        break;
    case '6':
        camera_right = RodriGeneral(camera_right , camera_look , -0.1) ; 
        camera_up = RodriGeneral(camera_up , camera_look , -0.1) ; 
        break;
    case ' ':
        manual_ball_control=!manual_ball_control ;
        break ; 
    case 'j':
        roll=false ;
        ball_angle+=jl_angle ; 
        ball_look = RodriGeneral(ball_look , ball_up , -jl_angle)  ; 
        ball_right = RodriGeneral(ball_right , ball_up , -jl_angle)  ;
        dis_calc();
        cout<<"AFTER j , PQ "<< Event_PQ.size()<<endl ; 
        showPQ() ;
        break ; 
    case 'l':
        roll=false ;
        ball_angle-=jl_angle ; 
        ball_look = RodriGeneral(ball_look , ball_up ,jl_angle)  ; 
        ball_right = RodriGeneral(ball_right , ball_up ,jl_angle)  ;
        dis_calc();
        cout<<"AFTER l , PQ "<< Event_PQ.size()<<endl ;
        showPQ() ;
        break ;  
    case 'i':
        bprev_right=ball_right ; 
        roll=true ; 
        ball_rolling_angle+=rolling_amount ; 
        ball_position=ball_position.addition(ball_look) ;
        break;
    case 'k':
         
        roll=true ; 
        bprev_right=ball_right ; 
        ball_rolling_angle-=rolling_amount ; 
        ball_position=ball_position.subtraction(ball_look) ;
        break;
    default:
        break;
    }
}


//! Changes the camera position only but not the look up or right vectors
void keystrokehandler(int key , int x  , int y)
{
    // position_of_camera.show(); 
    // camera_look.show() ; 
    // camera_up.show() ; 
    // camera_right.show() ; 
    switch (key)
    {
    case GLUT_KEY_UP:
        position_of_camera=position_of_camera.addition(camera_look) ; 
        break;
    case GLUT_KEY_DOWN:
        position_of_camera=position_of_camera.subtraction(camera_look) ; 
        break;
    case GLUT_KEY_RIGHT:
        position_of_camera=position_of_camera.addition(camera_right) ; 
        break;
    case GLUT_KEY_LEFT:
        position_of_camera=position_of_camera.subtraction(camera_right) ; 
        break;
    case GLUT_KEY_PAGE_UP:
        position_of_camera=position_of_camera.addition(camera_up) ; 
        break;
    case GLUT_KEY_PAGE_DOWN:
        position_of_camera=position_of_camera.subtraction(camera_up) ; 
        break;
    default:
        printf("aeh?\n") ; 
        break;
    }
}
void init()
{
    
    glEnable(GL_DEPTH_TEST) ; 
    glMatrixMode(GL_PROJECTION) ; 
    glLoadIdentity();
    gluPerspective(70,1,1,1000); 
    
    //! DATA TAKEN BY MANUAL CALCULATION 
    position_of_camera=point(50,50,50) ; 
    camera_up=point(3.0/sqrt(18.0) ,0 , -3.0/sqrt(18.0) ) ; 
    camera_right=point(-1.0/sqrt(6.0) , 2.0/sqrt(6.0) , -1.0/sqrt(6.0) );
    camera_look=point(-1.0/sqrt(3.0) , -1.0/sqrt(3.0) , -1.0/sqrt(3.0));

    //! DATA TAKEN BY PRINTING VALUES , LOOKS BETTER 
    //position_of_camera=point(102.292,127.909,94.8459) ; 
    position_of_camera=point(149.26,253.596,103.069) ; 
    camera_up=point(-0.0293367,-0.834602,0.550071) ; 
    camera_right=point(0.997508,0.0108813,0.0697086);
    camera_look=point(0.0641645,-0.550745,-0.832204);

    ball_position = point(189,150,5) ; 
    ball_look = point(1,0,0) ; 
    ball_right = point(0,1,0) ; 
    ball_up = point(0,0,1) ;

    dis_calc() ; 
    cout<<"AFTER INIT , PQ "<< Event_PQ.size()<<endl ; 



}

void setcolor(int color)
{
    switch (color)
    {
    case 1:
        glColor3f(1.0,0.0,0.0) ; 
        break;
    case 2:
        glColor3f(0.0,0.0,1.0) ; 
        break;
    case 3:
        glColor3f(0.0,1.0,0.0) ; 
        break;
    case 4:
        glColor3f(1.0,0.0,1.0) ; 
        break;
    case 5:
        glColor3f(0.0f, 1.0f, 1.0f); 
        break;
    case 6:
        glColor3f(1.0f, 0.0f, 1.0f);
        break;
    case 7:
        glColor3f(0.0f, 0.0f, 0.0f);
        break;
    case 8:
        glColor3f(1.0f, 1.0f, 1.0f);
        break;
    default:
        printf("aeh?\n") ; 
        break;
    }

}


// This code is taken from main.cpp 
void drawBall(double radius,int slices,int stacks)
{
	struct Sphere_point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
        
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}

	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glRotatef(ball_rolling_angle , 0, 1 , 0) ; 
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
   
			glBegin(GL_QUADS);{ 
                   
            if(j%2==0)setcolor(red) ; 
            else setcolor(green) ; 
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
              
            if(j%2==0)setcolor(green) ; 
            else setcolor(red) ; 
           
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}

}

void drawFloor()
{
	
    double x=-100.0,y=-100.0;
    for(int i=0 ; i<30 ; i++)
    {
        x=i*square_length ; 
        for(int j=0 ; j<30 ; j++)
        {
            y=j*square_length ; 

            if((i+j)%2==1)setcolor(black) ; 
            else setcolor(white) ; 

            glBegin(GL_QUADS) ; 
            {
                glVertex2f(x,y) ;
                glVertex2f(x+square_length,y) ;
                glVertex2f(x+square_length,y+square_length) ;
                glVertex2f(x,y+square_length) ;

            }
            
            glEnd() ; 
        }
        
    }

	
}


void draw_tb_layers(double bx , double by , double bl , double bw, double height )
{
    glBegin(GL_QUADS) ; 
    glVertex3f(bx, by, height);
    glVertex3f(bx, by - bw, height);
    glVertex3f(bx + bl, by - bw, height);
    glVertex3f(bx + bl, by, height);
    glEnd();


}

void draw_fb_layers(double bx , double by , double bl , double bw, double height )
{
    glBegin(GL_QUADS) ; 
    glVertex3f(bx, by, 0.0);
    glVertex3f(bx, by, height);
    glVertex3f(bx + bl, by, height);
    glVertex3f(bx + bl, by, 0.0);
    glEnd();


}

void draw_lr_layers(double bx , double by , double bl , double bw, double height )
{
    glBegin(GL_QUADS) ; 
    glVertex3f(bx, by - bw, 0.0);
    glVertex3f(bx, by - bw, height);
    glVertex3f(bx, by - bw, height);
    glVertex3f(bx, by - bw, 0.0);
    glEnd();


}

void draw_barrier(double bx , double by , double bl , double bw)
{
    double height = 4.0 ; 
    //setcolor(red) ;

    draw_tb_layers(bx,by,bl,bw,0.0);
    draw_tb_layers(bx,by,bl,bw,height);



    draw_fb_layers(bx,by,bl,bw,height);
    draw_fb_layers(bx,by - bw,bl,bw,height);
    draw_lr_layers(bx,by,bl,bw,height) ;
    draw_lr_layers(bx+bl,by,bl,bw,height) ;
}

void draw_head( double arrow_width)
{
    double head_len = .8 ; 
    double arrow_head = 0.1*ball_look.getx()*10 ; 
    double translate_arrow_head = 0.5 ; 
    glTranslatef(0,translate_arrow_head,0);
    glBegin(GL_TRIANGLES); 
    {
       glVertex3f(ball_look.getx()*10,ball_look.gety()*10+arrow_width,0) ; 
       glVertex3f(ball_look.getx()*10,ball_look.gety()*10-arrow_width,0) ; 
       glVertex3f(ball_look.getx()*10+arrow_head,ball_look.gety()*10,0) ; 
       
    }
    glEnd() ; 
}
void draw_arrow(point ballpos , double arrow_length , double arrow_width , double angle , int color)
{
    
    setcolor(color) ;
   // cout<<"angle : "<<angle<<endl ; 
    glTranslatef(ballpos.getx(), ballpos.gety(), ballpos.getz());
    if(color==2)
        {
        glBegin(GL_QUADS);
        {
            
            glVertex3f(0.0, 0.0, 0.0);
            glVertex3f(ball_look.getx()*10, ball_look.gety()*10, 0.0);
            glVertex3f(ball_look.getx()*10, ball_look.gety()*10+arrow_width, 0.0);
            glVertex3f(0.0, arrow_width, 0.0);
        }
        glEnd() ; 
        draw_head(arrow_width) ; 
        }
    else
    {
        glRotatef(-90.0 ,0,1,0);
        glBegin(GL_QUADS);
        {
            glVertex3f(0.0, 0.0, 0.0);
            glVertex3f(arrow_length, 0.0, 0.0);
            glVertex3f(arrow_length, arrow_width, 0.0);
            glVertex3f(0.0, arrow_width, 0.0);
        }
        glEnd() ; 
        double head_len = .8 ; 
        double arrow_head = 0.1*arrow_length ; 
        double translate_arrow_head = 0.5 ; 
        glTranslatef(0,translate_arrow_head,0);
        glBegin(GL_TRIANGLES); 
        {
            glVertex3f(arrow_length, arrow_width , 0.0);
            glVertex3f(arrow_length + arrow_head, 0.0, 0.0);
            glVertex3f(arrow_length, -arrow_width , 0.0);
        }
        glEnd() ; 
    } 
    
    

   
}

void EventScheduler(double elapsed_time)
{
    
    if(!Event_PQ.empty())
    {
        Event currentEvent = Event_PQ.top();
        cout << "Time: " << currentEvent.eventScheduledTime<< ", Collision Count: " << currentEvent.collision_count <<"elapsed time "<<elapsed_time<<endl ;
        
        
        double precision_time1 = elapsed_time+.008 ; 
        double precision_time2 = elapsed_time-.008 ; 
        if(currentEvent.eventScheduledTime>=precision_time2 && currentEvent.eventScheduledTime<=precision_time1 && currentEvent.collision_count==collision_count)
        {
            // so it's time 
            if(currentEvent.wall_side==posX || currentEvent.wall_side==negX)
            {
                cout<<"it's time"<<endl ; 
                ball_look.setx(-1*ball_look.getx()) ;
                ball_right.setx(-1*ball_right.getx()) ;
                ball_angle=180+ball_angle ; 
                collision_count++ ; 
                
            }
            else 
            {
                ball_look.sety(-1*ball_look.gety()) ;
                ball_right.sety(-1*ball_right.gety()) ;
                ball_angle=180+ball_angle ; 
                collision_count++ ;                 
            }
            Event_PQ.pop() ; 
             cout<<"AFTER bari 1 , PQ "<< Event_PQ.size()<<endl ; 
            showPQ();
            dis_calc() ;
            cout<<"AFTER bari 2 , PQ "<< Event_PQ.size()<<endl ; 
            showPQ();
            
        }
        else if(currentEvent.collision_count!=collision_count) 
        {
            Event_PQ.pop();
            EventScheduler(elapsed_time);

        }

    }
    
}

// display function 
void rolling_ball()
{
    // time::start_frame() ;
    // if(!manual_ball_control)
    // {
    //     ball_position=ball_position.addition(ball_look.scalarMul(time::delta_time_s())) ;
    // }
        

    auto end_time = chrono::high_resolution_clock::now() ; 
    chrono::duration<double> simulation_duration = end_time - start_time ;
   
    
    if(manual_ball_control)
        check_reflection(bar_x, bar_y,bar_len , bar_width);  
    else 
    {
        EventScheduler(simulation_duration.count());
    }

    
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW); // Now working on model view matrix 
    glLoadIdentity(); // all previous matrix cleared 

    gluLookAt(position_of_camera.getx() , position_of_camera.gety() , position_of_camera.getz() , 
    position_of_camera.getx()+camera_look.getx() , position_of_camera.gety()+camera_look.gety() , position_of_camera.getz()+camera_look.getz() , 
    camera_up.getx() , camera_up.gety() , camera_up.getz()
    ) ; 

 
   
    
    

    glPushMatrix() ; 
    drawFloor() ; 
    glPopMatrix() ;

    //ball_angle=acos(ball_look.getx());
   
    glPushMatrix();
    draw_arrow(ball_position, 10.0 , 1.0 , ball_angle , blue);
    glPopMatrix(); 

    if(up_arrow_show<100)
    {
        glPushMatrix();
        draw_arrow(ball_position, 15.0 , 1.0 , ball_angle , cyan);
        glPopMatrix();
      
    }
    up_arrow_show++ ; 
    up_arrow_show=up_arrow_show%200; 


    glPushMatrix() ;
    glTranslatef(ball_position.getx() , ball_position.gety() , ball_position.getz()) ; 
    if(roll)
        glRotatef(ball_rolling_angle ,  ball_right.getx() , ball_right.gety() , ball_right.getz() ) ;
    else glRotatef(ball_rolling_angle ,  bprev_right.getx() , bprev_right.gety() , bprev_right.getz() ) ;
    drawBall(ball_radius,15,12) ;  // radius , slice and stack 
    glPopMatrix() ; 



    glPushMatrix() ; 
    setcolor(red);
    draw_barrier(bar_x , bar_y , bar_len,bar_width) ; 
    glPopMatrix() ; 

    glPushMatrix() ; 
    setcolor(blue);
    glTranslatef(2*bar_len , 0 , 0) ;
    glRotatef(90,0,0,1) ; 
    draw_barrier(bar_x , bar_y , bar_len,bar_width) ; 
    glPopMatrix() ; 

    glPushMatrix() ; 
    setcolor(green);
    glTranslatef(3*bar_len , -5 , 0) ;
    glRotatef(90,0,0,1) ; 
    draw_barrier(bar_x , bar_y , bar_len,bar_width) ; 
    glPopMatrix() ; 

    glPushMatrix() ;
    setcolor(magenta);
    glTranslatef(5, bar_len , 0) ;
    draw_barrier(bar_x , bar_y , bar_len,bar_width) ; 
    glPopMatrix() ; 

    glutSwapBuffers() ; 
   // time::end_frame() ;
}
void idle()
{    
    
    glutPostRedisplay(); 
}


 
int main(int argc , char** argv)
{
    glutInit(&argc , argv) ; 
    glutInitWindowPosition(1000,200) ; 
    glutInitWindowSize(1000,1000) ; 
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH) ; //RGB - Double buffer - depth buffer 
    glutCreateWindow("Rolling Ball") ; 
    
    init() ; 
    glutSpecialFunc(keystrokehandler);
    glutKeyboardFunc(keyboard); 
    glutDisplayFunc(rolling_ball) ;
    glutIdleFunc(idle) ;
    glutMainLoop() ; 
    return 0 ; 
}