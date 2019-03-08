#include "Application.h"
#include <Windows.h>

//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow){
int main(){
	Application app("Tetris");
	app.run();
}