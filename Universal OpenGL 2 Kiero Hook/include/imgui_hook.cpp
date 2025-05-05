#include <Windows.h>
#include <functional> 
#include <GL/gl.h>
#include <iostream>
#include <algorithm>
#include "kiero/kiero.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_opengl2.h"

void setDarkThemeImGui() {
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	// Base styling
	style.WindowRounding = 12.0f;
	style.FrameRounding = 6.0f;
	style.PopupRounding = 6.0f;
	style.GrabRounding = 12.0f;
	style.ScrollbarRounding = 9.0f;
	style.ChildRounding = 12.0f;
	style.FramePadding = ImVec2(8, 6);
	style.ItemSpacing = ImVec2(8, 6);
	// Backgrounds
	colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.10f, 0.7f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.09f, 0.09f, 0.11f, 0.7f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.11f, 0.11f, 0.14f, 0.92f);

	// Interactive elements
	colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.25f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.30f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.30f, 0.35f, 1.00f);

	// Buttons
	colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.56f, 0.78f, 0.80f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.50f, 0.72f, 1.00f);

	// Accents
	colors[ImGuiCol_Header] = ImVec4(0.28f, 0.56f, 0.78f, 0.45f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.28f, 0.56f, 0.78f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.28f, 0.56f, 0.78f, 1.00f);

	// Custom title bar
	colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.12f, 0.7f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.15f, 0.18f, 0.9f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.08f, 0.08f, 0.10f, 0.65f);

	// Custom scrollbars
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.00f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.50f, 0.30f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.50f, 0.50f, 0.60f, 0.40f);
}


#define _CAST(t,v)	reinterpret_cast<t>(v)
#define _VOID_1(v)	std::function<void(v)>
#define _VOID_2(v)	_VOID_1(_VOID_1(v))

typedef BOOL(__stdcall* wglSwapBuffers_t) (
	HDC hDc
);

typedef LRESULT(CALLBACK* WNDPROC) (
	IN  HWND   hwnd,
	IN  UINT   uMsg,
	IN  WPARAM wParam,
	IN  LPARAM lParam
);

extern LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd, 
	UINT msg, 
	WPARAM wParam, 
	LPARAM lParam
);

extern void RenderMain();

namespace ImGuiHook 
{
	// Original functions variable
	static WNDPROC			o_WndProc;
	static wglSwapBuffers_t o_wglSwapBuffers;

	// Global variable
	static HGLRC      g_WglContext;
	static bool	      initImGui = false;
	static _VOID_1()  RenderMain;

	// WndProc callback ImGui handler
	LRESULT CALLBACK h_WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
			return true;

		ImGuiIO& io = ImGui::GetIO();

		if (io.WantCaptureKeyboard)
			if (uMsg == WM_KEYDOWN || uMsg == WM_KEYUP || uMsg == WM_SYSKEYDOWN || uMsg == WM_SYSKEYUP || uMsg == WM_CHAR)
				return true;

		if (io.WantCaptureMouse)
			if (uMsg == WM_MOUSEMOVE || uMsg == WM_MOUSEWHEEL || uMsg == WM_MOUSEHWHEEL || 
				uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP || uMsg == WM_RBUTTONDOWN || 
				uMsg == WM_RBUTTONUP)
					return true;

		return CallWindowProc(o_WndProc, hWnd, uMsg, wParam, lParam);
	}

	// Helper function
	void ExitStatus(bool* status, bool value)
	{
		if (status) *status = value;
	}
	
	HWND hWnd = nullptr;

	// Initialisation for ImGui
	void InitOpenGL2(
		IN  HDC	  hDc, 
		OUT bool* init,
		OUT bool* status)
	{
		if (WindowFromDC(hDc) == hWnd && *init) return;
		auto tStatus = true;

		hWnd = WindowFromDC(hDc);
		auto wLPTR = SetWindowLongPtr(hWnd, GWLP_WNDPROC, _CAST(LONG_PTR, h_WndProc));

		if (*init) 
		{
			ImGui_ImplWin32_Init(hWnd);
			ImGui_ImplOpenGL2_Init();
			return;
		}
		
		if (!wLPTR) return ExitStatus(status, false);

		o_WndProc = _CAST(WNDPROC, wLPTR);
		g_WglContext = wglCreateContext(hDc);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		tStatus &= ImGui_ImplWin32_Init(hWnd);
		tStatus &= ImGui_ImplOpenGL2_Init();

		*init = true;
		setDarkThemeImGui();
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 16.0f);
		return ExitStatus(status, tStatus);
	}

	// Generic ImGui renderer for Win32 backend
	void RenderWin32(
		IN  std::function<void()> render)
	{
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		render();

		ImGui::EndFrame();
		ImGui::Render();
	}

	// Generic ImGui renderer for OpenGL2 backend
	void RenderOpenGL2(
		IN  HGLRC 	  WglContext,
		IN  HDC		  hDc,
		IN  _VOID_2() render,
		IN  _VOID_1() render_inner,
		OUT bool*	  status)
	{
		auto tStatus = true;

		auto o_WglContext = wglGetCurrentContext();
		tStatus &= wglMakeCurrent(hDc, WglContext);

		ImGui_ImplOpenGL2_NewFrame();
		render(render_inner);
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

		tStatus &= wglMakeCurrent(hDc, o_WglContext);

		return ExitStatus(status, tStatus);
	}

	// Hooked wglSwapBuffers function
	BOOL __stdcall h_wglSwapBuffers(
		IN  HDC hDc)
	{
		InitOpenGL2(hDc, &initImGui, nullptr);
		RenderOpenGL2(g_WglContext, hDc, RenderWin32, RenderMain, nullptr);

		return o_wglSwapBuffers(hDc);
	}

	// Function to get the pointer of wglSwapBuffers
	wglSwapBuffers_t* get_wglSwapBuffers()
	{
		auto hMod = GetModuleHandleA("OPENGL32.dll");
		if (!hMod) return nullptr;

		return (wglSwapBuffers_t*)GetProcAddress(hMod, "wglSwapBuffers");
	}

	// Initialise hook
	bool InitHook()
	{

		if (kiero::init(kiero::RenderType::Auto) == kiero::Status::Success)
			return kiero::bind(get_wglSwapBuffers(), (void**)&o_wglSwapBuffers, h_wglSwapBuffers) == kiero::Status::Success;

		return false;
	}

	// Main load function
	bool Load(
		IN  _VOID_1() render)
	{
		RenderMain = render;
		return InitHook();
	}

	// Main unload function
	void Unload()
	{
		kiero::shutdown();
	}
}


