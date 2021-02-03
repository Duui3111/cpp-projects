
#include <iostream>
#include <Windows.h>
#include <dshow.h>
#include <mfapi.h>
#include <mfidl.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

#include "../ConsoleApplication7/qedit.h"

#include <windows.h>
#include <dshow.h>
#include <iostream>
#include <objbase.h>
#include <strmif.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#include "../ConsoleApplication7/bitmap/bitmap_image.hpp"

#define DLIB_TEST_FOR_ODR_VIOLATIONS_H_
#include <dlib/dnn/loss.h>
#include <dlib/dnn/input.h>
#include <dlib/dnn/solvers.h>
#include <dlib/image_loader/load_image.h>
#include <dlib/image_processing/frontal_face_detector.h>

using namespace dlib;
template <template <int, template<typename>class, int, typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual = add_prev1<block<N, BN, 1, tag1<SUBNET>>>;

template <template <int, template<typename>class, int, typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual_down = add_prev2<avg_pool<2, 2, 2, 2, skip1<tag2<block<N, BN, 2, tag1<SUBNET>>>>>>;

template <int N, template <typename> class BN, int stride, typename SUBNET>
using block = BN<con<N, 3, 3, 1, 1, relu<BN<con<N, 3, 3, stride, stride, SUBNET>>>>>;

template <int N, typename SUBNET> using ares = relu<residual<block, N, affine, SUBNET>>;
template <int N, typename SUBNET> using ares_down = relu<residual_down<block, N, affine, SUBNET>>;
template <typename SUBNET> using alevel0 = ares_down<256, SUBNET>;
template <typename SUBNET> using alevel1 = ares<256, ares<256, ares_down<256, SUBNET>>>;
template <typename SUBNET> using alevel2 = ares<128, ares<128, ares_down<128, SUBNET>>>;
template <typename SUBNET> using alevel3 = ares<64, ares<64, ares<64, ares_down<64, SUBNET>>>>;
template <typename SUBNET> using alevel4 = ares<32, ares<32, ares<32, SUBNET>>>;

using anet_type = dlib::loss_metric<dlib::fc_no_bias<128, dlib::avg_pool_everything<alevel0<alevel1<alevel2<alevel3<alevel4<dlib::max_pool<3, 3, 2, 2, dlib::relu<dlib::affine<dlib::con<32, 7, 7, 2, 2, dlib::input_rgb_image_sized<150>>>>>>>>>>>>>;

int faceRecognition(std::string image1, std::string image2, int image = 1)
{
    dlib::frontal_face_detector detector;
    dlib::shape_predictor sp;
    anet_type net;

    detector = dlib::get_frontal_face_detector();
    dlib::deserialize("./dat/shape_predictor_68_face_landmarks.dat") >> sp;
    dlib::deserialize("./dat/dlib_face_recognition_resnet_model_v1.dat") >> net;

    dlib::matrix<dlib::rgb_pixel> img;
    dlib::matrix<dlib::rgb_pixel> img2;

    dlib::load_image(img, image1);
    dlib::load_image(img2, image2);
    double a1 = std::min(img.nc(), img.nr());
    double a2 = std::min(img2.nc(), img2.nr());
    resize_image(250 / a1 * image, img);
    resize_image(250 / a2 * image, img2);

    std::vector<dlib::matrix<dlib::rgb_pixel>> faces1;
    double s1 = clock();

    for (auto face : detector(img, 0))
    {
        auto shape = sp(img, face);
        dlib::matrix<dlib::rgb_pixel> face_chip;
        extract_image_chip(img, get_face_chip_details(shape, 150, 0.25), face_chip);
        faces1.push_back(std::move(face_chip));
    }

    std::vector<dlib::matrix<dlib::rgb_pixel>> faces2;
    for (auto face : detector(img2, 0))
    {
        auto shape = sp(img2, face);
        dlib::matrix<dlib::rgb_pixel> face_chip;
        extract_image_chip(img2, get_face_chip_details(shape, 150, 0.25), face_chip);
        faces2.push_back(std::move(face_chip));
    }

    double s2 = clock();
    std::cout << "# Time detect faces: " << (s2 - s1) / CLOCKS_PER_SEC << std::endl;
    if (faces1.empty())
    {
        std::cout << "No faces found in the first image!" << std::endl;
        return 1;
    }
    else
        std::cout << "The number of faces found in the first image: " << faces1.size() << std::endl;
    if (faces2.empty())
    {
        std::cout << "No faces found in the second image!" << std::endl;
        return 2;
    }
    else
        std::cout << "The number of faces found in the second image: " << faces2.size() << std::endl;

    std::vector<dlib::matrix<float, 0, 1>> face_descriptors1 = net(faces1);
    std::vector<dlib::matrix<float, 0, 1>> face_descriptors2 = net(faces2);
    int flag = 0;

    for (const auto& i : face_descriptors1)
    {
        for (const auto& j : face_descriptors2)
        {
            std::cout << "The distance of two faces: " << length(i - j) << std::endl;
            if (length(i - j) < (double)0.6)
            {
                flag = 1;
                std::cout << "The two pictures have the same face" << std::endl;
                return 4;
            }
        }
    }

    double s3 = clock();
    std::cout << "# Time run Resnet: " << (s3 - s2) / CLOCKS_PER_SEC << std::endl;
    std::cout << "The two pictures don't have the same face" << std::endl;
}

#pragma warning(disable : 4996)

bool __IsFaceRec = false;

void ComprBitmap(std::string file)
{
    bitmap_image image(file);

    unsigned int total_number_of_pixels_r = 0;
    unsigned int total_number_of_pixels_g = 0;
    unsigned int total_number_of_pixels_b = 0;

    const unsigned int height = image.height();
    const unsigned int width = image.width();


    for (std::size_t y = 0; y < height; ++y)
    {
        for (std::size_t x = 0; x < width; ++x)
        {
            rgb_t colour;

            image.get_pixel(x, y, colour);

            if (colour.red >= 111)
                total_number_of_pixels_r++;
            if (colour.green >= 111)
                total_number_of_pixels_g++;
            if (colour.blue >= 111)
                total_number_of_pixels_b++;
        }
    }

    if (total_number_of_pixels_r < 700000)
    {
        __IsFaceRec == true;
        std::cout << "ok" << std::endl;
    }
    else
    {
        __IsFaceRec == false;
        std::cout << "no" << std::endl;
    }


    //std::cout << height << " " << width << std::endl;
    printf("Number of pixels with red %d\n", total_number_of_pixels_r);
    printf("Number of pixels with green %d\n", total_number_of_pixels_g);
    printf("Number of pixels with blue %d\n", total_number_of_pixels_b);
}


void SaveFrame(BITMAPINFO bi, BYTE* data, unsigned long size, const char* filepath)
{
    DWORD bufsize = size;
    BITMAPFILEHEADER bfh;
    memset(&bfh, 0, sizeof(bfh));
    bfh.bfType = 'MB';
    bfh.bfSize = sizeof(bfh) + bufsize + sizeof(BITMAPINFOHEADER);
    bfh.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);

    BITMAPINFOHEADER bih;
    memset(&bih, 0, sizeof(bih));
    bih.biSize = sizeof(bih);
    bih.biWidth = bi.bmiHeader.biWidth;
    bih.biHeight = bi.bmiHeader.biHeight;
    bih.biPlanes = 1;
    bih.biBitCount = 24;

    char FileName[512];
    sprintf(FileName, "capture_%d.bmp", (int)GetTickCount());

    if (filepath != "")
    {
        std::experimental::filesystem::path path{ filepath };
        path /= FileName;
        std::experimental::filesystem::create_directories(path.parent_path());

        std::ofstream ofs(path, std::ios::binary);
        if (ofs)
        {
            ofs.write((const char*)&bfh, sizeof(bfh));
            ofs.write((char*)&bih, sizeof(bih));
            ofs.write((char*)data, bufsize);
            ofs.close();

            //ComprBitmap(path.string());
            while (true)
            {
                faceRecognition("./dat/1b.bmp", path.string());
                std::cout << "begin" << std::endl;
                break;
            }

            std::cout << path.string() << std::endl;
        }
    }
}

class CSampleGrabberCB : public ISampleGrabberCB
{
public:
    DWORD lastTime;
    long Width;
    long Height;
    const char* filepathcap;

    HANDLE BufferEvent;
    LONGLONG prev, step;

    STDMETHODIMP_(ULONG) AddRef() { return 2; }
    STDMETHODIMP_(ULONG) Release() { return 1; }

    CSampleGrabberCB() { lastTime = 0; }

    STDMETHODIMP QueryInterface(REFIID riid, void** ppv)
    {
        if (riid == IID_ISampleGrabberCB || riid == IID_IUnknown)
        {
            *ppv = (void*) static_cast<ISampleGrabberCB*> (this);
            return NOERROR;
        }

        return E_NOINTERFACE;
    }

    STDMETHODIMP SampleCB(double SampleTime, IMediaSample* pSample) { return 0; }

    STDMETHODIMP BufferCB(double SampleTime, BYTE* pBuffer, long BufferSize)
    {
        BITMAPFILEHEADER bfh;
        memset(&bfh, 0, sizeof(bfh));
        bfh.bfType = 'MB';
        bfh.bfSize = sizeof(bfh) + BufferSize + sizeof(BITMAPINFOHEADER);
        bfh.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);

        DWORD Written = 0;

        BITMAPINFOHEADER bih;
        memset(&bih, 0, sizeof(bih));
        bih.biSize = sizeof(bih);
        bih.biWidth = Width;
        bih.biHeight = Height;
        bih.biPlanes = 1;
        bih.biBitCount = 24;

        BITMAPINFO bi;

        bi.bmiHeader = bih;

        DWORD newTime = GetTickCount();
        if (newTime - lastTime > 1000)
        {
            lastTime = newTime;
            SaveFrame(bi, pBuffer, BufferSize, filepathcap);
        }

        return 0;
    }
};


int __rectangleX = 0;
int __rectangleY = 0;
int __rectangleA = 0;
int __rectangleR = 0;
int __rectangleG = 0;
int __rectangleB = 0;
int __rectangleT = 0;

class videocap4
{
public:
    bool IsCapOpened = false;
    bool IsFaceRec = false;

    unsigned int numodevices;
    wchar_t devname[256];
    wchar_t devdescription[256];
    wchar_t devpath[256];
    wchar_t devclsid[256];

    videocap4();
    ~videocap4();

    int LoadFace(const char*);
    void SaveBitmapCap(const char*);
    int ShowVideoWindow(const wchar_t*, int, int, int, int);
    void Rectangle(int, int, int, int, int, int, int);

private:
    IGraphBuilder* GraphBuilder = nullptr;
    ICaptureGraphBuilder2* CaptureGraphBuilder2 = nullptr;
    IMediaControl* MediaControl = nullptr;
    IBaseFilter* DeviceFilter = nullptr;
    IBaseFilter* GrabberF = nullptr;
    ISampleGrabber* SampleGrabber = nullptr;
    ICreateDevEnum* CreateDevEnum = nullptr;
    IEnumMoniker* EnumMoniker = nullptr;
    IMoniker* Moniker = nullptr;
    IVideoWindow* VideoWindow = nullptr;
    IVMRWindowlessControl* WinlessContr = nullptr;
    IPropertyBag* PropertyBag = nullptr;
    IMFActivate** imfactivate = nullptr;
    IMFAttributes* imfAttributes = nullptr;

    HWND paranthwnd;

    const char* filepathcap = "";
    bool SartBitMapRecord();
    void StartDeviceInfo();
};

videocap4::videocap4()
{
    CoInitialize(NULL);
    CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (PVOID*)&CreateDevEnum);
    CreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &EnumMoniker, 0);

    if (EnumMoniker == NULL)
        MessageBox(0, L"No Divices Found", L"Error", MB_ICONERROR);

    EnumMoniker->Reset();

    EnumMoniker->Next(1, &Moniker, NULL);
    Moniker->BindToStorage(0, 0, IID_IPropertyBag, (void**)&PropertyBag);
    Moniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&DeviceFilter);

    IsCapOpened = true;

    StartDeviceInfo();
}

videocap4::~videocap4()
{
    IsCapOpened = false;

    EnumMoniker->Release();
    CreateDevEnum->Release();
    CoUninitialize();
}

void videocap4::SaveBitmapCap(const char* filepath)
{
    this->filepathcap = filepath;
}

bool videocap4::SartBitMapRecord()
{
    CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&GrabberF);
    GrabberF->QueryInterface(IID_ISampleGrabber, (void**)&SampleGrabber);
    CaptureGraphBuilder2->SetFiltergraph(GraphBuilder);

    AM_MEDIA_TYPE mt;
    ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
    mt.majortype = MEDIATYPE_Video;
    mt.subtype = MEDIASUBTYPE_RGB24;
    SampleGrabber->SetMediaType(&mt);
    SampleGrabber->SetBufferSamples(TRUE);

    GraphBuilder->AddFilter(GrabberF, L"Sample Grabber");
    CaptureGraphBuilder2->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, DeviceFilter, GrabberF, 0);
    HRESULT hr = SampleGrabber->GetConnectedMediaType(&mt);

    VIDEOINFOHEADER* vih = (VIDEOINFOHEADER*)mt.pbFormat;

    CSampleGrabberCB* CB = new CSampleGrabberCB();
    CB->filepathcap = this->filepathcap;

    if (!FAILED(hr))
    {
        CB->Width = vih->bmiHeader.biWidth;
        CB->Height = vih->bmiHeader.biHeight;
    }

    SampleGrabber->SetCallback(CB, 1);

    if (this->filepathcap == "")
        return false;
    else
        return true;
}

void videocap4::StartDeviceInfo()
{
    VARIANT var;

    MFCreateAttributes(&imfAttributes, 1);
    imfAttributes->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);
    MFEnumDeviceSources(imfAttributes, &imfactivate, &numodevices);
    imfAttributes->Release();

    var.vt = VT_BSTR;
    PropertyBag->Read(L"FriendlyName", &var, 0);
    WideCharToMultiByte(CP_ACP, 0, var.bstrVal, -1, (LPSTR)devname, sizeof(devname), 0, 0);
    VariantClear(&var);

    var.vt = VT_BSTR;
    PropertyBag->Read(L"Description", &var, 0);
    WideCharToMultiByte(CP_ACP, 0, var.bstrVal, -1, (LPSTR)devdescription, sizeof(devdescription), 0, 0);
    VariantClear(&var);

    var.vt = VT_BSTR;
    PropertyBag->Read(L"DevicePath", &var, 0);
    WideCharToMultiByte(CP_ACP, 0, var.bstrVal, -1, (LPSTR)devpath, sizeof(devpath), 0, 0);
    VariantClear(&var);

    var.vt = VT_BSTR;
    PropertyBag->Read(L"CLSID", &var, 0);
    WideCharToMultiByte(CP_ACP, 0, var.bstrVal, -1, (LPSTR)devclsid, sizeof(devclsid), 0, 0);
    VariantClear(&var);

    // std::cout << "Found A Divice\n" << "Number of Divice " << numodevices << '\n' << "Divice Name " << (const char*)devname << '\n' << "Divice Description " << (const char*)devdescription << '\n' << "Divice Path " << (const char*)devpath << '\n' << "Divice CLSID " << (const char*)devclsid << std::endl;
}

void OnPaint_(HDC hdc)
{
    if (__rectangleX != 0 || __rectangleY != 0)
    {
        Gdiplus::Graphics graphics(hdc);
        Gdiplus::Pen pen(Gdiplus::Color(255, 0, 0, 255), 2); // pen(Color(__rectangleA, __rectangleR, __rectangleG, __rectangleB), __rectangleT); 
        graphics.DrawRectangle(&pen, 10, 10, 240, 240); //240
    }
}

LRESULT CALLBACK _WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        OnPaint_(hdc);
        EndPaint(hWnd, &ps);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

int videocap4::ShowVideoWindow(const wchar_t* Caption, int Left, int Top, int Width, int Height)
{
    HINSTANCE hinstance = GetModuleHandle(NULL);
    RegisterClass(new WNDCLASS({ CS_HREDRAW | CS_VREDRAW, _WndProc, 0L, 0L, hinstance, LoadIcon(NULL, IDI_APPLICATION), LoadCursor(NULL, IDC_CROSS), (HBRUSH)GetStockObject(WHITE_BRUSH), 0L, L"CAPWINDOW" }));
    paranthwnd = CreateWindowExW(0, L"CAPWINDOW", 0, WS_OVERLAPPEDWINDOW, 500, 150, 600, 500, 0, 0, hinstance, 0);

    CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (LPVOID*)&GraphBuilder);
    CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC, IID_ICaptureGraphBuilder2, (LPVOID*)&CaptureGraphBuilder2);
    CaptureGraphBuilder2->SetFiltergraph(GraphBuilder);
    GraphBuilder->QueryInterface(IID_IMediaControl, (LPVOID*)&MediaControl);
    GraphBuilder->AddFilter(DeviceFilter, L"Device Filter");
    if (SartBitMapRecord() == true) SartBitMapRecord();
    CaptureGraphBuilder2->RenderStream(&PIN_CATEGORY_PREVIEW, NULL, DeviceFilter, NULL, NULL);
    GraphBuilder->QueryInterface(IID_IVideoWindow, (LPVOID*)&VideoWindow);
    SetWindowTextW(paranthwnd, Caption);
    VideoWindow->put_Owner((OAHWND)paranthwnd);
    VideoWindow->SetWindowPosition(Left, Top, Width, Height);
    VideoWindow->put_WindowStyle(WS_CLIPCHILDREN);

    RECT rc;
    GetClientRect(paranthwnd, &rc);
    VideoWindow->SetWindowPosition(0, 0, rc.right, rc.bottom);

    VideoWindow->put_Visible(OATRUE);
    ShowWindow(paranthwnd, SW_SHOWDEFAULT);
    MediaControl->Run();

    if (__rectangleX != 0 || __rectangleY != 0)
    {
        Gdiplus::GdiplusStartupInput gdiplusStartupInput;
        ULONG_PTR gdiplusToken;
        GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
        RegisterClass(new WNDCLASS({ 0, _WndProc, 0L, 0L, hinstance, LoadIcon(NULL, IDI_APPLICATION), LoadCursor(NULL, IDC_CROSS), (HBRUSH)GetStockObject(BLACK_BRUSH), 0L, L"FACEBOXCAP" }));
        HWND hWnd = CreateWindowExW(WS_EX_LAYERED, L"FACEBOXCAP", 0, WS_POPUP | WS_VISIBLE | WS_SYSMENU, __rectangleX /*650*/, __rectangleY /*300*/, 320, 320, paranthwnd, 0, hinstance, 0);
        SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 255, LWA_COLORKEY | LWA_ALPHA);
        ShowWindow(hWnd, SW_SHOWDEFAULT);
        UpdateWindow(hWnd);
        Gdiplus::GdiplusShutdown(gdiplusToken);
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    Moniker->Release();
    PropertyBag->Release();
    MediaControl->Release();
    CaptureGraphBuilder2->Release();
    GraphBuilder->Release();
    VideoWindow->Release();

    return (int)msg.wParam;
}

void videocap4::Rectangle(int X, int Y, int A, int R, int G, int B, int thinkness)
{
    __rectangleX = X;
    __rectangleY = Y;
    __rectangleA = A;
    __rectangleR = R;
    __rectangleG = B;
    __rectangleB = B;
    __rectangleT = thinkness;
}


int main(int argc, char** argv) 
{
    //faceRecognition("./dat/1b.bmp", "./dat/2b.bmp");

    videocap4 vcap4;

    if (vcap4.IsCapOpened)
    {
        vcap4.SaveBitmapCap("./captures");

        vcap4.Rectangle(650, 300, 255, 255, 0, 0, 6);

        if (vcap4.IsFaceRec)
            std::cout << "face rec" << std::endl;

        vcap4.ShowVideoWindow(L"my window caption", 500, 150, 600, 500);
    }

    return 0;
}
