// mkdir build
// gcc -o build/ogl ogl.cc -lX11 -lstdc++ -lGL

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysymdef.h>
#include <iostream>

#include <GL/glx.h>

int main(int argc, char **argv)
{

	Display *d;
	Window w;
	Screen *s;
	int sid;
	XEvent e;

	// Open the display
	d = XOpenDisplay(NULL);
	if (d == NULL)
	{
		printf("%s\n", "Could not open display");
		return 1;
	}
	s = DefaultScreenOfDisplay(d);
	sid = DefaultScreen(d);

	GLint mjv, mnv;
	glXQueryVersion(d, &mjv, &mnv);

	printf("MajorGLX = %d, MinorGLX = %d\n", mjv, mnv);

	GLint glxAttribs[] = {
		GLX_RGBA,
		GLX_DOUBLEBUFFER,
		GLX_DEPTH_SIZE, 24,
		GLX_STENCIL_SIZE, 8,
		GLX_RED_SIZE, 8,
		GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE, 8,
		GLX_SAMPLE_BUFFERS, 0,
		GLX_SAMPLES, 0,
		None};
	XVisualInfo *visual = glXChooseVisual(d, sid, glxAttribs);

	XSetWindowAttributes windowAttribs;
	windowAttribs.border_pixel = BlackPixel(d, sid);
	windowAttribs.background_pixel = WhitePixel(d, sid);
	windowAttribs.override_redirect = True;
	windowAttribs.colormap = XCreateColormap(d, RootWindow(d, sid), visual->visual, AllocNone);
	windowAttribs.event_mask = ExposureMask;
	w = XCreateWindow(d, RootWindow(d, sid), 0, 0, 320, 200, 0, visual->depth, InputOutput, visual->visual, CWBackPixel | CWColormap | CWBorderPixel | CWEventMask, &windowAttribs);

	// w = XCreateSimpleWindow(d, RootWindowOfScreen(s), 0, 0, 320, 200, 1, BlackPixel(d, sid), WhitePixel(d, sid));

	GLXContext context = glXCreateContext(d, visual, NULL, GL_TRUE);
	glXMakeCurrent(d, w, context);

	std::cout << "GL Vendor: " << glGetString(GL_VENDOR) << "\n";
	std::cout << "GL Renderer: " << glGetString(GL_RENDERER) << "\n";
	std::cout << "GL Version: " << glGetString(GL_VERSION) << "\n";
	std::cout << "GL Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

	XSelectInput(d, w, ExposureMask);

	// Name the window
	XStoreName(d, w, "Named Window");

	// Show the window
	XClearWindow(d, w);
	XMapRaised(d, w);

	// How large is the window
	XWindowAttributes wa;
	XGetWindowAttributes(d, w, &wa);
	std::cout << "Window Width: " << wa.width << ", Height: " << wa.height << "\n";

	// Resize window
	XWindowChanges wc;
	wc.width = 800;
	wc.height = 600;
	XConfigureWindow(d, w, CWWidth | CWHeight, &wc);

	int x, y;

	// Enter message loop

	int i = 0;
	while (true)
	{
		i++;
		XNextEvent(d, &e);
		switch (e.type)
		{
		case Expose:
			std::cout << "Expose event " << i << std::endl;
			XGetWindowAttributes(d, w, &wa);
			std::cout << "\tWindow Width: " << wa.width << ", Height: " << wa.height << "\n";
			glViewport(0, 0, wa.width, wa.height);
			break;
		}
		glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, -1.0f, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		glEnd();
		glXSwapBuffers(d, w);
	}

	// Cleanup GLX
	glXDestroyContext(d, context);

	// Cleanup X11
	XFree(visual);
	XFreeColormap(d, windowAttribs.colormap);
	XDestroyWindow(d, w);
	XCloseDisplay(d);
	return 0;

	
}