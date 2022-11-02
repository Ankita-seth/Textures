#include "framework.h"

int WINAPI WinMain(HINSTANCE hInstance,    //Main windows function
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd)

{
    Texture* texture = new Texture();
    if (texture)
    {
        // create the window
        if (!texture->InitializeWindow(hInstance, nShowCmd, texture->FullScreen))
        {
            MessageBox(0, L"Window Initialization - Failed",
                L"Error", MB_OK);
            return 1;
        }

        // initialize direct3d
        if (!texture->InitD3D())
        {
            MessageBox(0, L"Failed to initialize direct3d 12",
                L"Error", MB_OK);
            texture->Cleanup();
            return 1;
        }

        // start the main loop
        texture->mainloop();

        // we want to wait for the gpu to finish executing the command list before we start releasing everything
        texture->WaitForPreviousFrame();

        // close the fence event
        CloseHandle(texture->fenceEvent);

        // clean up everything
        texture->Cleanup();
    }
    delete texture;
    texture = NULL;
    return 0;
}