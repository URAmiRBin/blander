#include"Application.h"

int main() {	
	Application app("Blander 0.1b", 640, 480, true);
	while (!app.getWindowShouldClose()) {
		app.update();
		app.render();
	}
	return 0;
}