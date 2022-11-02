#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

////////////////

#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN    // Exclude rarely-used stuff from Windows headers.
#endif

#include <windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include "d3dx12.h"
#include <string>
#include <wincodec.h>

//Liniking the libraries
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")


// this will only call release if an object exists (prevents exceptions calling release on non existant objects)
#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }

using namespace DirectX; // we will be using the directxmath library

class Texture
{
public:
    Texture()
    {
        //Constructor
    }

    ~Texture()
    {
        //Destructor
    }
    // Handle to the window
    HWND hwnd = NULL;

    // name of the window (not the title)
    LPCTSTR WindowName = L"BzTutsApp";

    // title of the window
    LPCTSTR WindowTitle = L"Bz Window";

    // width and height of the window
    int Width = 800;
    int Height = 600;

    // is window full screen?
    bool FullScreen = false;

    // we will exit the program when this becomes false
    bool Running = true;

    // create a window
    bool InitializeWindow(HINSTANCE hInstance,
        int ShowWnd,
        bool fullscreen);

    // main application loop
    void mainloop();

    // direct3d stuff
    const static int frameBufferCount = 3; // number of buffers we want, 2 for double buffering, 3 for tripple buffering

    ID3D12Device* device; // direct3d device

    IDXGISwapChain3* swapChain; // swapchain used to switch between render targets

    ID3D12CommandQueue* commandQueue; // container for command lists

    ID3D12DescriptorHeap* rtvDescriptorHeap; // a descriptor heap to hold resources like the render targets

    ID3D12Resource* renderTargets[frameBufferCount]; // number of render targets equal to buffer count

    ID3D12CommandAllocator* commandAllocator[frameBufferCount]; // we want enough allocators for each buffer * number of threads (we only have one thread)

    ID3D12GraphicsCommandList* commandList; // a command list we can record commands into, then execute them to render the frame

    ID3D12Fence* fence[frameBufferCount];    // an object that is locked while our command list is being executed by the gpu. We need as many 
    //as we have allocators (more if we want to know when the gpu is finished with an asset)

    HANDLE fenceEvent; // a handle to an event when our fence is unlocked by the gpu

    UINT64 fenceValue[frameBufferCount]; // this value is incremented each frame. each fence will have its own value

    int frameIndex; // current rtv we are on

    int rtvDescriptorSize; // size of the rtv descriptor on the device (all front and back buffers will be the same size)
    // function declarations

    bool InitD3D(); // initializes direct3d 12

    void Update(); // update the game logic

    void UpdatePipeline(); // update the direct3d pipeline (update command lists)

    void Render(); // execute the command list

    void Cleanup(); // release com ojects and clean up memory

    void WaitForPreviousFrame(); // wait until gpu is finished with command list

    ID3D12PipelineState* pipelineStateObject; // pso containing a pipeline state

    ID3D12RootSignature* rootSignature; // root signature defines data shaders will access

    D3D12_VIEWPORT viewport; // area that output from rasterizer will be stretched to.

    D3D12_RECT scissorRect; // the area to draw in. pixels outside that area will not be drawn onto

    ID3D12Resource* vertexBuffer; // a default buffer in GPU memory that we will load vertex data for our triangle into
    ID3D12Resource* indexBuffer; // a default buffer in GPU memory that we will load index data for our triangle into

    D3D12_VERTEX_BUFFER_VIEW vertexBufferView; // a structure containing a pointer to the vertex data in gpu memory
    // the total size of the buffer, and the size of each element (vertex)

    D3D12_INDEX_BUFFER_VIEW indexBufferView; // a structure holding information about the index buffer

    ID3D12Resource* depthStencilBuffer; // This is the memory for our depth buffer. it will also be used for a stencil buffer in a later tutorial
    ID3D12DescriptorHeap* dsDescriptorHeap; // This is a heap for our depth/stencil buffer descriptor

    ID3D12Resource* constantBufferUploadHeaps[frameBufferCount]; // this is the memory on the gpu where constant buffers for each frame will be placed

    UINT8* cbvGPUAddress[frameBufferCount]; // this is a pointer to each of the constant buffer resource heaps

    XMFLOAT4X4 cameraProjMat; // this will store our projection matrix
    XMFLOAT4X4 cameraViewMat; // this will store our view matrix

    XMFLOAT4 cameraPosition; // this is our cameras position vector
    XMFLOAT4 cameraTarget; // a vector describing the point in space our camera is looking at
    XMFLOAT4 cameraUp; // the worlds up vector

    XMFLOAT4X4 cube1WorldMat; // our first cubes world matrix (transformation matrix)
    XMFLOAT4X4 cube1RotMat; // this will keep track of our rotation for the first cube
    XMFLOAT4 cube1Position; // our first cubes position in space

    XMFLOAT4X4 cube2WorldMat; // our first cubes world matrix (transformation matrix)
    XMFLOAT4X4 cube2RotMat; // this will keep track of our rotation for the second cube
    XMFLOAT4 cube2PositionOffset; // our second cube will rotate around the first cube, so this is the position offset from the first cube

    int numCubeIndices; // the number of indices to draw the cube

    ID3D12Resource* textureBuffer; // the resource heap containing our texture

    int LoadImageDataFromFile(BYTE** imageData, D3D12_RESOURCE_DESC& resourceDescription, LPCWSTR filename, int& bytesPerRow);

    int GetDXGIFormatBitsPerPixel(DXGI_FORMAT& dxgiFormat);

    ID3D12DescriptorHeap* mainDescriptorHeap;
    ID3D12Resource* textureBufferUploadHeap;
};

DXGI_FORMAT GetDXGIFormatFromWICFormat(WICPixelFormatGUID& wicFormatGUID);
WICPixelFormatGUID GetConvertToWICFormat(WICPixelFormatGUID& wicFormatGUID);

// callback function for windows messages
LRESULT CALLBACK WndProc(HWND hWnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance,    //Main windows function
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd);