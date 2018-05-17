//#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#include <iostream>
#include <Windows.h>

#include "main.h"

#include "BCI_Interface.h"
#include "EmotivInterface.h"
#include "spatial_filter.h"

#include "lib/glad.h"
#include "lib/glfw3.h"

#include "mainwindow.h"
#include <QApplication>

#include "resources.h"

int main(int argc, char *argv[])
{
	recordMemoryLeaks();

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/*
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
	std::cout << "Failed to create GLFW window" << std::endl;
	glfwTerminate();
	return -1;
	}
	glfwMakeContextCurrent(window);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
	std::cout << "Failed to initialize GLAD" << std::endl;
	return -1;
	}

	glViewport(0, 0, 800, 600);
	*/
	//OutputDebugString("\n\n!!!!!! SOME BULLSHIT !!!!!!\n\n");
	
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	try
	{

		std::auto_ptr<bci::BCI_Interface> bci_hardware{ new bci::EmotivInterface };
		bci_hardware->connect();
		std::cout << bci::spatial_filter::CAR(bci_hardware->getData()) << std::endl;

		std::this_thread::sleep_for(std::chrono::milliseconds(666));
	}
	catch (DetailedException e)
	{
		DebugOutput::println("ERROR" + e.getMessage(), e.getFile(), e.getLine(), 0);
		//std::cout << "ERROR: " << e.what() << std::endl;
		//std::cout << e.details() << std::endl;
	}
	catch (std::exception e)
	{

		std::cout << "std exception: " << e.what() << std::endl;
	}

	glfwTerminate();

	pause();
	return a.exec();
}

void pause()
{
	std::cout << "\nPRESS ENTER TO CLOSE APPLICATION\n";
	std::cin.clear(); // reset any error flags
					  //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore any characters in the input buffer until we find an enter character
	std::cin.get(); // get one more char from the user
}

void recordMemoryLeaks()
{
	// get warnings about memory leaks - only debug mode
	// need to run program from console to catch them
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}
