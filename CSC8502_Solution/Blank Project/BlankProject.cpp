#include "../NCLGL/window.h"
#include "Renderer.h"

/*
	~			 CONTROLS			 ~

	O - toggles automatic camera movement
	P - toggles manual camera movement

	B - increases blur + shake post processing effect strength
	N - reduces blur + shake post processing effect strength

*/
int main()	{
	Window w("Advanced Graphics Coursework", 1280, 720, false);
	Renderer renderer(w);

	if(!(w.HasInitialised() & renderer.HasInitialised())) {
		return -1;
	}

	float dTime;
	float fps;

	while(w.UpdateWindow()  && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
	
		dTime = w.GetTimer()->GetTimeDeltaSeconds();
		float fps = 1 / dTime;
		w.SetTitle("Advanced Graphics Coursework " + std::to_string(fps) + " fps ");

		// Close the game
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_F5)) {
			Shader::ReloadAllShaders();
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_B))
		{
			renderer.incrementPostPasses();
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_N))
		{
			renderer.decrementPostPasses();
		}

		renderer.setElapsedTime(w.GetTimer()->GetTotalTimeMSec());

		w.LockMouseToWindow(true);
		w.ShowOSPointer(false);

		renderer.UpdateScene(dTime);
		renderer.RenderScene(dTime);
		renderer.SwapBuffers();

	}

	return 0;
}