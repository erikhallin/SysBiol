#include <SOIL/SOIL.h>
#include <windows.h>
#include <gl/gl.h>
#include <fcntl.h> //for console
#include <stdio.h> //for console
#include <sstream>
#include <fstream>
#include <vector>
#include "data_display.h"

#define _version 0.1

using namespace std;

/*struct st_hit
{
    int hit_id;

    float start_time;
    float end_time;
    float step_size;

    float alpha;
    float alpha_0;
    float hill_coef;
    float beta;

    float mrna_lacI;
    float mrna_tetR;
    float mrna_cl;

    float prot_lacI;
    float prot_tetR;//reporter
    float prot_cl;
};*/

int g_window_width=800;
int g_window_height=600;

//data_display* pData_display;

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);


int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    string command_line=lpCmdLine;
    bool debug_mode=true;
    if(command_line=="debug") debug_mode=true;

    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "SysBiol";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;



    /* create main window */
    hwnd = CreateWindowEx(0,
                          "SysBiol",
                          "SysBiol",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          g_window_width,
                          g_window_height,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    //if debug mode start console
    if(debug_mode)
    {
        //Open a console window
        AllocConsole();
        //Connect console output
        HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
        int hCrt          = _open_osfhandle((long) handle_out, _O_TEXT);
        FILE* hf_out      = _fdopen(hCrt, "w");
        setvbuf(hf_out, NULL, _IONBF, 1);
        *stdout = *hf_out;
        //Connect console input
        HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
        hCrt             = _open_osfhandle((long) handle_in, _O_TEXT);
        FILE* hf_in      = _fdopen(hCrt, "r");
        setvbuf(hf_in, NULL, _IONBF, 128);
        *stdin = *hf_in;
        //Set console title
        SetConsoleTitle("Debug Console");

        cout<<"Software started\n";
        cout<<"Version: "<<_version<<endl;
    }

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

        //load hits
    vector<st_hit> vec_hits;
    ifstream input_file("values_of_interest.txt",ifstream::in);
    if(input_file==NULL)
    {
        cout<<"ERROR: No input file!\n";
        return 1;//NO file
    }
    else cout<<"Loading hits from input file...";
    string line;
    while ( getline(input_file,line) )
    {//read values
        if(line=="END")
        {
            break;//done
        }

        st_hit temp_hit;
        //time is default
        temp_hit.start_time=0;
        temp_hit.end_time=200;
        temp_hit.step_size=0.01;

        string word;
        stringstream ss0(line);
        ss0>>word;//skip title
        ss0>>word;//read value
        temp_hit.hit_id=atof( word.c_str() );
        getline(input_file,line);
        stringstream ss1(line);
        ss1>>word;//skip title
        ss1>>word;//read value
        temp_hit.alpha=atof( word.c_str() );
        getline(input_file,line);
        stringstream ss2(line);
        ss2>>word;//skip title
        ss2>>word;//read value
        temp_hit.alpha_0=atof( word.c_str() );
        getline(input_file,line);
        stringstream ss3(line);
        ss3>>word;//skip title
        ss3>>word;//read value
        temp_hit.hill_coef=atof( word.c_str() );
        getline(input_file,line);
        stringstream ss4(line);
        ss4>>word;//skip title
        ss4>>word;//read value
        temp_hit.beta=atof( word.c_str() );
        getline(input_file,line);
        stringstream ss5(line);
        ss5>>word;//skip title
        ss5>>word;//read value
        temp_hit.mrna_lacI=atof( word.c_str() );
        getline(input_file,line);
        stringstream ss6(line);
        ss6>>word;//skip title
        ss6>>word;//read value
        temp_hit.mrna_tetR=atof( word.c_str() );
        getline(input_file,line);
        stringstream ss7(line);
        ss7>>word;//skip title
        ss7>>word;//read value
        temp_hit.mrna_cl=atof( word.c_str() );
        getline(input_file,line);
        stringstream ss8(line);
        ss8>>word;//skip title
        ss8>>word;//read value
        temp_hit.prot_lacI=atof( word.c_str() );
        getline(input_file,line);
        stringstream ss9(line);
        ss9>>word;//skip title
        ss9>>word;//read value
        temp_hit.prot_tetR=atof( word.c_str() );
        getline(input_file,line);
        stringstream ss10(line);
        ss10>>word;//skip title
        ss10>>word;//read value
        temp_hit.prot_cl=atof( word.c_str() );
        getline(input_file,line);//blank

        vec_hits.push_back(temp_hit);
    }
    cout<<" DONE\n";


    /*//send hit vector to data_display
    float start_time=0.0;
    float end_time=20.0;
    float step_size=0.01;

    float alpha=0.0;
    float alpha_0=0.0;
    float hill_coef=1.2;
    float beta=10.000;

    float mrna_lacI=0.0;
    float mrna_tetR=0.0;
    float mrna_cl=0.0;

    float prot_lacI=0.0;
    float prot_tetR=0.0;//reporter
    float prot_cl=0.5;

    pData_display->set_start_values(start_time,end_time,step_size,
                                    alpha,alpha_0,
                                    hill_coef,beta,
                                    mrna_lacI,mrna_tetR,mrna_cl,
                                    prot_lacI,prot_tetR,prot_cl);*/

    /* program main loop */
    int hit_counter=0;
    bool soon_done=false;
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            //Clear screen buffer
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glLoadIdentity();

            if(soon_done) bQuit=true;
            //Init graph, new for every cycle
            data_display pData_display;
            pData_display.init(g_window_width,g_window_height);
            if(hit_counter>=(int)vec_hits.size())
            {
                //signal to turn off next cycle
                soon_done=true;
            }

            //Drawing and calculations
            if(!soon_done)
            if( pData_display.cycle(vec_hits[hit_counter]) )

            SwapBuffers(hDC);

            //output last cycle hit screen (first will be blank 0)
            string filename("hit_");
            stringstream ss;
            ss<<hit_counter;
            filename.append( ss.str() );
            filename.append(".bmp");
            int save_result = SOIL_save_screenshot
            (
                filename.c_str(),
                SOIL_SAVE_TYPE_BMP,
                0, 0, 800, 600
            );
            if(save_result) cout<<"Printed file for hit no: "<<hit_counter<<endl;
            else cout<<"ERROR: Could not print hit no: "<<hit_counter<<endl;

            hit_counter++;

            //SwapBuffers(hDC);
        }
    }

    cout<<"\nPrinting COMPLETE\n";
    system("pause");


    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);

    //set 2D mode

    glClearColor(0.0,0.0,0.0,0.0);  //Set the cleared screen colour to black
    glViewport(0,0,g_window_width,g_window_height);   //This sets up the viewport so that the coordinates (0, 0) are at the top left of the window

    //Set up the orthographic projection so that coordinates (0, 0) are in the top left
    //and the minimum and maximum depth is -10 and 10. To enable depth just put in
    //glEnable(GL_DEPTH_TEST)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,g_window_width,g_window_height,0,-1,1);

    //Back to the modelview so we can draw stuff
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //Enable antialiasing
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

