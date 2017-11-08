//#pragma once
//
//#include <Windows.h>
//#include <GL\glew.h>
////#include <gl\GL.h>
////#include <gl\GLU.h>
//
////Set up variables
//HGLRC           hRC = NULL;										// Permanent Rendering Context
//HDC             hDC = NULL;										// Private GDI Device Context
//HWND            hWnd = NULL;									// Holds Our Window Handle
//HINSTANCE       hInstance;										// Holds The Instance Of The Application
//
//bool    keys[256];												// Array Used For The Keyboard Routine
//bool    active = TRUE;											// Window Active Flag Set To TRUE By Default
//bool    fullscreen = TRUE;										// Fullscreen Flag Set To Fullscreen Mode By Default
//
//LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);           // Declaration For WndProc
//
//LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow);
//int initGL(GLvoid);
////void draw();            // drawing function containing OpenGL function calls
//// Here's Where We Do All The Drawing;
//
//GLvoid ReSizeGLScene(GLsizei width, GLsizei height)             // Resize And Initialize The GL Window
//{
//	if (height == 0)											// Prevent A Divide By Zero By
//	{
//		height = 1;												// Making Height Equal One
//	}
//
//	glViewport(0, 0, width, height);							// Reset The Current Viewport
//	glMatrixMode(GL_PROJECTION);								// Select The Projection Matrix
//	glLoadIdentity();											// Reset The Projection Matrix
//
//																// Calculate The Aspect Ratio Of The Window
//	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
//
//	glMatrixMode(GL_MODELVIEW);									// Select The Modelview Matrix
//	glLoadIdentity();											// Reset The Modelview Matrix
//}
//
//
//int DrawGLScene(GLvoid)											// Here's Where We Do All The Drawing
//{
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // Clear The Screen And The Depth Buffer
//	glLoadIdentity();											// Reset The Current Modelview Matrix
//	return TRUE;												// Everything Went OK
//}
//
////this is PROBLEM
//int InitGL(GLvoid)
//{
//	glShadeModel(GL_SMOOTH);									//enables smooth shading
//	glClearColor(0.0f, 0.5f, 0.0f, 0.5f);						// dark green Background
//	glClearDepth(1.0f);											// Depth Buffer Setup
//	glEnable(GL_DEPTH_TEST);									// Enables Depth Testing
//	glDepthFunc(GL_LEQUAL);										// The Type Of Depth Test To Do
//	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);          // Really Nice Perspective Calculations
//	return TRUE;
//}
//
//GLvoid KillGLWindow(GLvoid)
//{
//	if (fullscreen)
//	{
//		ChangeDisplaySettings(NULL, 0);
//		ShowCursor(TRUE);
//	}
//	if (hRC)
//	{
//		if (!wglMakeCurrent(NULL, NULL))
//		{
//			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
//		}
//		hRC = NULL;
//	}
//	if (hDC && !ReleaseDC(hWnd, hDC))
//	{
//		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
//		hDC = NULL;												// Set DC To NULL
//	}
//	if (hWnd && !DestroyWindow(hWnd))							// Are We Able To Destroy The Window?
//	{
//		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
//		hWnd = NULL;											// Set hWnd To NULL
//	}
//	if (!UnregisterClass("OpenGL", hInstance))					// Are We Able To Unregister Class
//	{
//		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
//		hInstance = NULL;										// Set hInstance To NULL
//	}
//}
//
//
//BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
//{
//	GLuint PixelFormat;
//	WNDCLASS wc;
//	DWORD dwExStyle;											//window extended style
//	DWORD dwStyle;												//window style
//
//	RECT WindowRect;											// Grabs Rectangle Upper Left / Lower Right Values
//	WindowRect.left = (long)0;									// Set Left Value To 0
//	WindowRect.right = (long)width;								// Set Right Value To Requested Width
//	WindowRect.top = (long)0;									// Set Top Value To 0
//	WindowRect.bottom = (long)height;							// Set Bottom Value To Requested Height
//
//	fullscreen = fullscreenflag;								// Set The Global Fullscreen Flag
//
//	hInstance = GetModuleHandle(NULL);							// Grab An Instance For Our Window
//	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;				// Redraw On Move, And Own DC For Window
//	wc.lpfnWndProc = (WNDPROC)WndProc;							// WndProc Handles Messages
//	wc.cbClsExtra = 0;											// No Extra Window Data
//	wc.cbWndExtra = 0;											// No Extra Window Data
//	wc.hInstance = hInstance;									// Set The Instance
//	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);						// Load The Default Icon
//	wc.hCursor = LoadCursor(NULL, IDC_ARROW);					// Load The Arrow Pointer
//	wc.hbrBackground = NULL;									// No Background Required For GL
//	wc.lpszMenuName = NULL;										// We Don't Want A Menu
//	wc.lpszClassName = "OpenGL";								// Set The Class Name
//
//	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
//	{
//		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
//		return FALSE;											// Exit And Return FALSE
//	}
//
//	if (fullscreen)												// Attempt Fullscreen Mode?
//	{
//		DEVMODE dmScreenSettings;                   // Device Mode
//		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));       // Makes Sure Memory's Cleared
//		dmScreenSettings.dmSize = sizeof(dmScreenSettings);       // Size Of The Devmode Structure
//		dmScreenSettings.dmPelsWidth = width;            // Selected Screen Width
//		dmScreenSettings.dmPelsHeight = height;           // Selected Screen Height
//		dmScreenSettings.dmBitsPerPel = bits;             // Selected Bits Per Pixel
//		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
//		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
//		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
//		{
//			// If The Mode Fails, Offer Two Options.  Quit Or Run In A Window.
//			if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
//			{
//				fullscreen = FALSE;               // Select Windowed Mode (Fullscreen=FALSE)
//			}
//			else
//			{
//				// Pop Up A Message Box Letting User Know The Program Is Closing.
//				MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
//				return FALSE;                   // Exit And Return FALSE
//			}
//		}
//	}
//	if (fullscreen)
//	{
//		dwExStyle = WS_EX_APPWINDOW;                  // Window Extended Style
//		dwStyle = WS_POPUP;                       // Windows Style
//		ShowCursor(FALSE);                      // Hide Mouse Pointer
//	}
//	else
//	{
//		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;           // Window Extended Style
//		dwStyle = WS_OVERLAPPEDWINDOW;                    // Windows Style
//	}
//
//	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);     // Adjust Window To True Requested Size
//
//	if (!(hWnd = CreateWindowEx(dwExStyle,              // Extended Style For The Window
//		"OpenGL",               // Class Name
//		title,                  // Window Title
//		WS_CLIPSIBLINGS |           // Required Window Style
//		WS_CLIPCHILDREN |           // Required Window Style
//		dwStyle,                // Selected Window Style
//		0, 0,                   // Window Position
//		WindowRect.right - WindowRect.left,   // Calculate Adjusted Window Width
//		WindowRect.bottom - WindowRect.top,   // Calculate Adjusted Window Height
//		NULL,                   // No Parent Window
//		NULL,                   // No Menu
//		hInstance,              // Instance
//		NULL)))                 // Don't Pass Anything To WM_CREATE
//	{
//		KillGLWindow();                         // Reset The Display
//		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
//		return FALSE;                           // Return FALSE
//	}
//
//	static  PIXELFORMATDESCRIPTOR pfd =                  // pfd Tells Windows How We Want Things To Be
//	{
//		sizeof(PIXELFORMATDESCRIPTOR),                  // Size Of This Pixel Format Descriptor
//		1,                              // Version Number
//		PFD_DRAW_TO_WINDOW |                        // Format Must Support Window
//		PFD_SUPPORT_OPENGL |                        // Format Must Support OpenGL
//		PFD_DOUBLEBUFFER,                       // Must Support Double Buffering
//		PFD_TYPE_RGBA,                          // Request An RGBA Format
//		bits,                               // Select Our Color Depth
//		0, 0, 0, 0, 0, 0,                       // Color Bits Ignored
//		0,                              // No Alpha Buffer
//		0,                              // Shift Bit Ignored
//		0,                              // No Accumulation Buffer
//		0, 0, 0, 0,                         // Accumulation Bits Ignored
//		16,                             // 16Bit Z-Buffer (Depth Buffer)
//		0,                              // No Stencil Buffer
//		0,                              // No Auxiliary Buffer
//		PFD_MAIN_PLANE,                         // Main Drawing Layer
//		0,                              // Reserved
//		0, 0, 0                             // Layer Masks Ignored
//	};
//	//LOTS OF ERROR CHECKING
//	if (!(hDC = GetDC(hWnd)))                         // Did We Get A Device Context?
//	{
//		KillGLWindow();                         // Reset The Display
//		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
//		return FALSE;                           // Return FALSE
//	}
//
//	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))             // Did Windows Find A Matching Pixel Format?
//	{
//		KillGLWindow();                         // Reset The Display
//		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
//		return FALSE;                           // Return FALSE
//	}
//
//	if (!SetPixelFormat(hDC, PixelFormat, &pfd))               // Are We Able To Set The Pixel Format?
//	{
//		KillGLWindow();                         // Reset The Display
//		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
//		return FALSE;                           // Return FALSE
//	}
//
//	if (!(hRC = wglCreateContext(hDC)))                   // Are We Able To Get A Rendering Context?
//	{
//		KillGLWindow();                         // Reset The Display
//		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
//		return FALSE;                           // Return FALSE
//	}
//
//	if (!wglMakeCurrent(hDC, hRC))                        // Try To Activate The Rendering Context
//	{
//		KillGLWindow();                         // Reset The Display
//		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
//		return FALSE;                           // Return FALSE
//	}
//
//	//IF EVERYTHING WENT SMOOTHLY...
//	ShowWindow(hWnd, SW_SHOW);                       // Show The Window
//	SetForegroundWindow(hWnd);                      // Slightly Higher Priority
//	SetFocus(hWnd);                             // Sets Keyboard Focus To The Window
//	ReSizeGLScene(width, height);                       // Set Up Our Perspective GL Screen
//
//	if (!InitGL())                              // Initialize Our Newly Created GL Window
//	{
//		KillGLWindow();                         // Reset The Display
//		MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
//		return FALSE;                           // Return FALSE
//	}
//	return TRUE;								//success!
//
//}
//
//LRESULT CALLBACK WndProc(HWND    hWnd,  // Handle For This Window
//	UINT    uMsg,						// Message For This Window
//	WPARAM  wParam,						// Additional Message Information
//	LPARAM  lParam)						// Additional Message Information
//{
//	switch (uMsg)                               // Check For Windows Messages
//	{
//
//	case WM_ACTIVATE:								// Watch For Window Activate Message
//	{
//		if (!HIWORD(wParam))                    // Check Minimization State
//		{
//			active = TRUE;						// Program Is Active
//		}
//		else
//		{
//			active = FALSE;						// Program Is No Longer Active
//		}
//
//		return 0;								// Return To The Message Loop
//	}
//	case WM_SYSCOMMAND:								// Intercept System Commands
//	{
//		switch (wParam)							// Check System Calls
//		{
//		case SC_SCREENSAVE:						// Screensaver Trying To Start?
//		case SC_MONITORPOWER:					// Monitor Trying To Enter Powersave?
//			return 0;							// Prevent From Happening
//		}
//		break;									// Exit
//	}
//	case WM_CLOSE:									// Did We Receive A Close Message?
//	{
//		PostQuitMessage(0);						// Send A Quit Message
//		return 0;								// Jump Back
//	}
//	case WM_KEYDOWN:								// Is A Key Being Held Down?
//	{
//		keys[wParam] = TRUE;                    // If So, Mark It As TRUE
//		return 0;								// Jump Back
//	}
//	case WM_KEYUP:									// Has A Key Been Released?
//	{
//		keys[wParam] = FALSE;                   // If So, Mark It As FALSE
//		return 0;								// Jump Back
//	}
//	case WM_SIZE:									// Resize The OpenGL Window
//	{
//		ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));// LoWord=Width, HiWord=Height
//		return 0;								// Jump Back
//	}
//	// Pass All Unhandled Messages To DefWindowProc
//	return DefWindowProc(hWnd, uMsg, wParam, lParam);
//	}
//}
//
////This is the entry point of our Windows Application. This is where we call our window creation routine, deal with window messages, and watch for human interaction.
//int WINAPI WinMain(HINSTANCE   hInstance, HINSTANCE   hPrevInstance, LPSTR    lpCmdLine, int  nCmdShow)
//{
//	MSG msg;
//	BOOL done = FALSE;
//	if (!CreateGLWindow("NeHe's OpenGL Framework", 640, 480, 16, fullscreen))
//	{
//		return 0;                           // Quit If Window Was Not Created
//	}
//	while (!done)                                // Loop That Runs Until done=TRUE
//	{
//		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
//		{
//			if (msg.message == WM_QUIT)               // Have We Received A Quit Message?
//			{
//				done = TRUE;                  // If So done=TRUE
//			}
//			else                            // If Not, Deal With Window Messages
//			{
//				TranslateMessage(&msg);             // Translate The Message
//				DispatchMessage(&msg);              // Dispatch The Message
//			}
//		}
//		else                                // If There Are No Messages
//		{
//			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
//			if (active)                     // Program Active?
//			{
//				if (keys[VK_ESCAPE])                // Was ESC Pressed?
//				{
//					done = TRUE;              // ESC Signalled A Quit
//				}
//				else                        // Not Time To Quit, Update Screen
//				{
//					DrawGLScene();              // Draw The Scene
//					SwapBuffers(hDC);           // Swap Buffers (Double Buffering)
//				}
//			}
//		}
//	}
//	// Shutdown
//	KillGLWindow();                             // Kill The Window
//	return (msg.wParam);                            // Exit The Program
//}