
//////////////////////////////////////////
//                                      //
// OpenGL in a PROPER Windows APP       //
// ( NO GLUT !! )                       //
//                                      //
// You found this at bobobobo's weblog, //
// https://bobobobo.wordpress.com        //
//                                      //
// Creation date:  Feb 9/08             //
// Last modified:  Feb 10/08            //
//                                      //
//////////////////////////////////////////

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL\glew.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

//////////////////////////
// OVERVIEW:
//
// The OUTPUT of OpenGL is a two dimensional
// PICTURE of a 3D scene.

// That's right.  I said 2D.

// People often say that the process OpenGL
// goes through to render a 3D scene is just like
// a CAMERA TAKING A PHOTOGRAPH OF A REAL WORLD SCENE.
//
// So, you know obviously that a PHOTOGRAPH
// is PURELY 2D after its been developed,
// and it DEPICTS the 3D scene that it
// "took a picture" of accurately.

// That's exactly what OpenGL does.  OpenGL's JOB
// is to take all your instructions, all your
// glColor3f()'s and your glVertex3f()'s, and
// to ultimately end up DRAWING A 2D PICTURE
// from those instructions that can be displayed
// on a computer screen.

// In THIS program, our MAIN GOAL is to
// CONNECT UP the OUTPUT of OpenGL (that 2D
// image OpenGL produces)
// with YOUR APPLICATION'S WINDOW.

// Does this look familiar?

/*
HDC hdc = GetDC( hwnd );
*/

// You should already know that the way
// you as a Windows programmer draw to
// your application's window is using
// your window's HDC.

// If this doesn't sound familiar,
// then I strongly recommend you go read up
// on Win GDI before continuing!

////////////////////////////////!
//
// So if our way to draw to our application window
// is the HDC, and OpenGL produces some 2D image
// HOW IN THE WORLD DO YOU CONNECT UP the OUTPUT
// of the OpenGL program (that 2D picture)
// WITH the HDC of a WINDOW?
//
// That's the main subject we're tackling here
// in this tutorial.
//
// And its EASY.
//
// WE'RE lucky.  Microsoft created a bunch
// of functions (all beginning with "wgl"),
// that make this job of "connecting up" the output
// of OpenGL with the HDC of our window quite easy!

/////////////////////////
// BIG PICTURE:
//
// Here's the big picture of what
// we're going to be doing here:

/*

|---------|  draws to   |-------|  copied out   |---------|  shows in  |-----------|
|         | ==========> |       | ============> |         | =========> |           |
|---------|             |-------|               |---------|            |-----------|
OPENGL                  HGLRC                     HDC                 application
FUNCTION                                                                 window
CALLS

*/

//////////////////////
// In code:  this is the steps
// we'll follow.
//
// 1.  CREATE WINDOW AS USUAL.
//
// 2.  GET DC OF WINDOW.
//     Get the HDC of our window using a line like:
//          hdc = GetDC( hwnd );
//
// 3.  SET PIXEL FORMAT OF HDC.
//     You do 3 things here:
//          Create a PFD ('pixel format descriptor')
//          ChoosePixelFormat()
//          SetPixelFormat()
//
// 4.  CREATE RENDERING CONTEXT (HGLRC).
//          wglCreateContext()
//     Create the surface to which OpenGL
//     shall draw.  It is created such that
//     it shall be completely compatible
//     with the DC of our window, (it will
//     use the same pixel format!)
//
// 5.  CONNECT THE RENDER CONTEXT (HGLRC)
//     WITH THE DEVICE CONTEXT (HDC) OF WINDOW.
//          wglMakeCurrent()
//
// 6.  DRAW USING OPENGL.
//          glVertex3d(); glColor3d(); // ETC!
//     You call OpenGL functions to perform
//     your drawing!  OpenGL will spit out
//     its result picture to the HGLRC, which is
//     connected to the backbuffer of your HDC.
//
// 7.  SWAP BUFFERS.
//          SwapBuffers( hdc );	
//     Assuming you've picked a DOUBLE BUFFERED
//     pixel format all the way back in step 2, 
//     you'll need to SWAP the buffers so that
//     the image you've created using OpenGL on
//     the backbuffer of your hdc is shown
//     in your application window.

// And that's all!

// Ready for the code??? Let's go!

/////////////////////
// GLOBALS
//
/// Define a structure to hold all
/// of the global variables of this app.
struct Globals
{
	HINSTANCE hInstance;    // window app instance

	HWND hwnd;      // handle for the window

	HDC   hdc;      // handle to device context

	HGLRC hglrc;    // handle to OpenGL rendering context

	int width, height;      // the desired width and
							// height of the CLIENT AREA
							// (DRAWABLE REGION in Window)
};

// declare one struct Globals called g;
Globals g;


// Function prototypes.
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow);
void draw();            // drawing function containing OpenGL function calls

						
// In a C++ Windows app, the starting point is WinMain().
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
	//////////////////
	// First we'll start by saving a copy of
	// the hInstance parameter inside our
	// "glob" of globals "g":
	g.hInstance = hInstance;

#pragma region part 1 - create a window
	// The next few lines you should already
	// be used to:  create a WNDCLASS
	// that describes the properties of
	// the window we're going to soon create.
	// A.  Create the WNDCLASS
	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = TEXT("Ellie");
	wc.lpszMenuName = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	// Register that class with the Windows O/S..
	RegisterClass(&wc);

	/////////////////
	// Ok, AT THIS POINT, we'd normally
	// just go ahead and call CreateWindow().
	// And we WILL call CreateWindow(), but
	// there is something I must explain to
	// you first.  That thing is the RECT structure.

	/////////////////
	// RECT:
	//
	// A RECT is just a C struct meant to represent
	// a rectangle.
	// 
	// The RECT structure WILL DESCRIBE EXACTLY WHERE
	// AND HOW WE WANT OUR WINDOW TO APPEAR WHEN WE
	// CREATE IT.
	//
	//         TOP
	//       --------
	//       |      |
	// LEFT  |      | RIGHT
	//       --------
	//        BOTTOM
	//
	// So, what we do is, we create the RECT
	// struct for our window as follows:
	RECT rect;
	SetRect(&rect, 50,  // left
		50,  // top
		850, // right
		650); // bottom

			  // Save width and height off.
	g.width = rect.right - rect.left;
	g.height = rect.bottom - rect.top;

	// Adjust it.
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// AdjustWindowRect() expands the RECT
	// so that the CLIENT AREA (drawable region)
	// has EXACTLY the dimensions we specify
	// in the incoming RECT.

	// If you didn't just understand that, understand
	// this:  "you have to call AdjustWindowRect()",
	// and move on.  Its not THAT important, but its
	// good for the performance of your app.

	///////////////////
	// NOW we call CreateWindow, using
	// that adjusted RECT structure to
	// specify the width and height of the window.
	g.hwnd = CreateWindow(TEXT("Philip"),
		TEXT("GL WINDOW!"),
		WS_OVERLAPPEDWINDOW,
		rect.left, rect.top,  // adjusted x, y positions
		rect.right - rect.left, rect.bottom - rect.top,  // adjusted width and height
		NULL, NULL,
		hInstance, NULL);

	// check to see that the window
	// was created successfully!
	if (g.hwnd == NULL)
	{
		FatalAppExit(NULL, TEXT("CreateWindow() failed!"));
	}

	// and show.
	ShowWindow(g.hwnd, iCmdShow);
#pragma endregion

#pragma region part 2 - Get DC of window we just made
	//2.  GET DC OF WINDOW, and keep it in our global
	//    variable g.  We will NOT release this DC
	//    until our app is about to exit.
	g.hdc = GetDC(g.hwnd);

	// If this keeping-DC-for-life-of-program-thing
	// disturbs you as much as it disturbed me,
	// GOOD LUCK in finding MS-based documentation to validate
	// this practice!  All examples I've seen and
	// Astle's OpenGL gaming book do it this way,
	// so. . . I suppose its ok.

	// One of the things I make sure to do is to specify
	// CS_OWNDC when I create the window,
	// so that Windows maintains a separate device context
	// for my application's window.

	// I haven't run into problems with this.  I don't
	// think you should either.
#pragma endregion

#pragma region part 3 - SET PIXEL FORMAT OF HDC
	//3.  SET PIXEL FORMAT OF HDC.
	//
	// We now have to set up the PIXELFORMAT
	// of our HDC.

	// A PIXEL FORMAT just describes the
	// "qualities" that each pixel in the 
	// window will have. 24 bit vs 16

	// There are 3 substeps here:
	//    A> create the PFD and set it up to describe
	//       the pixel format we DESIRE (dream of having!)
	//
	//    B> call ChoosePixelFormat() to make windows
	//       choose us the ID of the appropriate pixel format that
	//       is CLOSEST to our dream pixel format.
	//
	//    C> Call SetPixelFormat() using the integer ID number
	//       that ChoosePixelFormat() returned to us in step B>

	// So let's do that:

	////////////////////
	// A> CREATE PFD:
	PIXELFORMATDESCRIPTOR pfd = { 0 };  // create the pfd,
										// and start it out with ALL ZEROs 

										// So we set only the fields of the pfd we care about:
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);    // just its size
	pfd.nVersion = 1;   // always 1

	pfd.dwFlags = PFD_SUPPORT_OPENGL |  // OpenGL support - not DirectDraw
		PFD_DOUBLEBUFFER |				// double buffering support
		PFD_DRAW_TO_WINDOW;				// draw to the app window, not to a bitmap image

	pfd.iPixelType = PFD_TYPE_RGBA;		// red, green, blue, alpha for each pixel
	pfd.cColorBits = 24;                // 24 bit == 8 bits for red, 8 for green, 8 for blue.
										// This count of color bits EXCLUDES alpha.

	pfd.cDepthBits = 32;                // 32 bits to measure pixel depth.  That's accurate!

										///////////////////
										// B> Alright!  We've filled out the pfd
										// and it describes the way we want
										// our pixels to appear on the screen.
										// 
										// Now this next step is a little bit weird.
										// The thing is, there are only a couple of
										// dozen ACCEPTABLE pixel formats in existence.
										//
										// In other words, the system MIGHT NOT
										// be able to use a pixel format the likes
										// of which you have described in your
										// PIXELFORMATDESCRIPTOR.

										// What to do?? It would be awful annoying
										// to have to keep TRYING different
										// PIXELFORMATDESCRIPTORS until we found
										// one that actually WORKED on this system.

										// So MSFT has a better solution.

										// We'll make a call to a function called
										// ChoosePixelFormat().  ChoosePixelFormat()
										// will examine the PIXELFORMATDESCRIPTOR
										// structure that you send it, then it will
										// give you back an ID for the pixel format
										// that MOST CLOSELY MATCHES the pixel format you
										// SAID you wanted.

	int chosenPixelFormat = ChoosePixelFormat(g.hdc, &pfd);
	// what comes back from ChoosePixelFormat() is
	// an integer identifier for a specific pixel
	// format that Windows ALREADY knows about.
	// If you got 0 back, then there was an error.
	if (chosenPixelFormat == 0)
	{
		FatalAppExit(NULL, TEXT("ChoosePixelFormat() failed!"));
	}

	char b[100];
	sprintf_s(b, "You got ID# %d as your pixelformat!\n", chosenPixelFormat);
	MessageBoxA(NULL, b, "Your pixelformat", MB_OK);

	/////////////////
	// C> So finally, we call SetPixelFormat() using the integer ID number
	// that ChoosePixelFormat() returned to us previously.
	int result = SetPixelFormat(g.hdc, chosenPixelFormat, &pfd);

	if (result == NULL)
	{
		FatalAppExit(NULL, TEXT("SetPixelFormat() failed!"));
	}
	// and that's all there is to setting
	// the pixel format!
	//////////
#pragma endregion

#pragma region part 4 - CREATE THE RENDERING CONTEXT
	//4.  CREATE RENDERING CONTEXT (HGLRC).

	// What's a rendering context?
	// Its the "surface" that OpenGL
	// will DRAW to.

	// The HGLRC will be created
	// such that it is COMPATIBLE
	// with the hdc.

	g.hglrc = wglCreateContext(g.hdc);
	// Created the rendering context
	// and saved handle to it in global 'g'.
	//
	// Wasn't that awfully easy to create
	// such a complicated sounding thing?
	///////////////
#pragma endregion

#pragma region part 5 - CONNECT THE RENDERING CONTEXT WITH THE DEVICE CONTEXT OF THE WINDOW
	//5.  CONNECT THE RENDER CONTEXT (HGLRC)
	//    WITH THE DEVICE CONTEXT (HDC) OF WINDOW.
	wglMakeCurrent(g.hdc, g.hglrc);

	//
	// OPEN GL INIT COMPLETED!!
	////////////////////////////
#pragma endregion

#pragma region message loop
	MSG msg;

	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//6.  DRAW USING OPENGL.
			// This region right here is the
			// heart of our application.  THE MOST
			// execution time is spent just repeating
			// this draw() function.
			draw();
		}
	}
#pragma endregion

#pragma region clean up
	// UNmake your rendering context (make it 'uncurrent')
	wglMakeCurrent(NULL, NULL);

	// Delete the rendering context, we no longer need it.
	wglDeleteContext(g.hglrc);

	// release your window's DC
	ReleaseDC(g.hwnd, g.hdc);
#pragma endregion

	// and a cheesy fade exit
	AnimateWindow(g.hwnd, 200, AW_HIDE | AW_BLEND);

	return msg.wParam;
}

////////////////////////
// DRAWING FUNCTION
void draw()
{
	// 1. set up the viewport
	glViewport(0, 0, g.width, g.height); // set viewport
										 // to be the whole width and height
										 // of the CLIENT AREA (drawable region) of the window,
										 // (the CLIENT AREA excludes the titlebar and the 
										 // maximize/minimize buttons).

										 // 2. projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)g.width / (float)g.height, 1, 1000);

	// 3. viewing transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0, 0, 10,
		0, 0, 0,
		0, 1, 0);

	// 4. modelling transformation and drawing
	glClearColor(0.5, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static float i = 0.01f;
	// Notice that 'i' is a STATIC variable.

	// A 'static' variable is created ONCE
	// when the function in which it sits first runs.

	// The static variable will "LIVE ON"
	// between seperate calls to the function
	// in which it lives UNTIL THE PROGRAM ENDS.

	i += 0.1f;     // increase i by 0.001 from its
					 // it had on the LAST FUNCTION CALL to the draw() function

	float c = cos(i);
	float s = sin(i);

	glBegin(GL_TRIANGLES);
	glColor3f(c, 0, 0);      // red
	glVertex3f(1 + c, 0 + s, 0);

	glColor3f(c, s, 0);      // yellow
	glVertex3f(0 + c, 1 + s, 0);

	glColor3f(s, 0.1f, s);   // magenta
	glVertex3f(-1 + c, 0 + s, 0);
	glEnd();

	//7.  SWAP BUFFERS.
	SwapBuffers(g.hdc);
	// Its important to realize that the backbuffer
	// is intelligently managed by the HDC ON ITS OWN,
	// so all's you gots to do is call SwapBuffers
	// on the HDC of your window.
}


////////////////////////
// WNDPROC
// Notice that WndProc is very very neglected.
// We hardly do anything with it!  That's because
// we do all of our processing in the draw()
// function.
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_CREATE:
		Beep(50, 10);
		return 0;
		break;

	case WM_PAINT:
	{
		HDC hdc;
		PAINTSTRUCT ps;
		hdc = BeginPaint(hwnd, &ps);
		// don't draw here.  would be waaay too slow.
		// draw in the draw() function instead.
		EndPaint(hwnd, &ps);
	}
	return 0;
	break;

	case WM_KEYDOWN:
		switch (wparam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		default:
			break;
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	}

	return DefWindowProc(hwnd, message, wparam, lparam);
}



////////////////
// END NOTES:
//
// Some good references:
// WGL function ref on MSDN:
// http://msdn2.microsoft.com/en-us/library/ms673957%28VS.85%29.aspx

// MSDN example from 1994 (but still good!)
// "OpenGL I: Quick Start"
// http://msdn2.microsoft.com/en-us/library/ms970745.aspx


//////////////////
// QUICK Q&A (make sure you know what's going on):
//
// QUESTION:  What's the means by which we can draw
//            to our window itself?
//
// ANSWER:  The HDC (HANDLE TO DEVICE CONTEXT).

// QUESTION:  What's the means by which OpenGL can
//            draw to the window?
//
// ANSWER:  USING that SAME HDC WE woulda used
//          to draw to it!! (more to come on this now).

/////////////////////
// It IS possible to access the bits
// of the output of OpenGL in 2 ways:
//      1)  Use glReadPixels() to obtain
//          the arrayful of pixels on the
//          screen.  You can then save this
//          to a .TGA or .BMP file easily.

//      2)  Render to a BITMAP, then
//          blit that bitmap to your HDC.
//          There's a 1995 msdn article on this:
//          "OpenGL VI: Rendering on DIBs with PFD_DRAW_TO_BITMAP"
//          http://msdn2.microsoft.com/en-us/library/ms970768.aspx
