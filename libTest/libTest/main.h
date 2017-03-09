#pragma once
#ifdef LIB_H
#define LIB_H
extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstance, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		// attach to process  
		// return FALSE to fail DLL load  
		MessageBoxA(0, "I am a dll!", "DLL Message", MB_OK | MB_ICONINFORMATION);
		break;

	case DLL_PROCESS_DETACH:
		// detach from process  
		break;

	case DLL_THREAD_ATTACH:
		// attach to thread  
		break;

	case DLL_THREAD_DETACH:
		// detach from thread  
		break;
	}
	return TRUE; // succesful  
}
#endif