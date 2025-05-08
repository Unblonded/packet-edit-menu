#include <Windows.h>
#include <functional> 
#include <GL/gl.h>
#include <iostream>
#include <algorithm>
#include "kiero/kiero.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_opengl2.h"
#include "../cfg.h"

void SetCyberpunkNeonTheme()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	// Base styling with sharp corners and tight spacing
	style.WindowPadding = ImVec2(15, 15);
	style.FramePadding = ImVec2(10, 6);
	style.ItemSpacing = ImVec2(10, 8);
	style.ItemInnerSpacing = ImVec2(6, 6);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 20.0f;
	style.ScrollbarSize = 12.0f;
	style.GrabMinSize = 10.0f;

	// Sharp corners for that cyberpunk edge
	style.WindowRounding = 0.0f;
	style.ChildRounding = 0.0f;
	style.FrameRounding = 0.0f;
	style.PopupRounding = 0.0f;
	style.ScrollbarRounding = 0.0f;
	style.GrabRounding = 0.0f;
	style.TabRounding = 0.0f;

	// Cyberpunk color palette - NEON INTENSITY
	const ImVec4 bg_dark = ImVec4(0.03f, 0.03f, 0.06f, 0.98f);  // Deep purple-black
	const ImVec4 bg_medium = ImVec4(0.08f, 0.05f, 0.12f, 0.95f);  // Dark purple
	const ImVec4 bg_light = ImVec4(0.12f, 0.08f, 0.18f, 0.95f);  // Medium purple

	// Neon accent colors
	const ImVec4 neon_pink = ImVec4(1.00f, 0.08f, 0.58f, 1.00f);  // Hot pink
	const ImVec4 neon_blue = ImVec4(0.08f, 0.85f, 1.00f, 1.00f);  // Electric blue
	const ImVec4 neon_purple = ImVec4(0.70f, 0.30f, 1.00f, 1.00f);  // Purple haze
	const ImVec4 neon_green = ImVec4(0.30f, 1.00f, 0.40f, 1.00f);  // Matrix green

	const ImVec4 text_primary = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);  // Bright white
	const ImVec4 text_secondary = ImVec4(0.70f, 0.71f, 0.75f, 1.00f);  // Grayish
	const ImVec4 border_color = ImVec4(0.40f, 0.20f, 0.60f, 0.60f);  // Purple border

	// Main colors
	colors[ImGuiCol_Text] = text_primary;
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = bg_dark;
	colors[ImGuiCol_ChildBg] = bg_medium;
	colors[ImGuiCol_PopupBg] = bg_medium;
	colors[ImGuiCol_Border] = neon_purple;
	colors[ImGuiCol_BorderShadow] = ImVec4(neon_purple.x, neon_purple.y, neon_purple.z, 0.30f);

	// Frame background
	colors[ImGuiCol_FrameBg] = ImVec4(bg_light.x, bg_light.y, bg_light.z, 0.80f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(neon_purple.x * 0.4f, neon_purple.y * 0.4f, neon_purple.z * 0.4f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(neon_purple.x * 0.5f, neon_purple.y * 0.5f, neon_purple.z * 0.5f, 0.60f);

	// Title bars
	colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.05f, 0.12f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(neon_purple.x * 0.25f, neon_purple.y * 0.25f, neon_purple.z * 0.25f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.08f, 0.05f, 0.12f, 0.75f);

	// Buttons - ALTERNATING NEON COLORS
	colors[ImGuiCol_Button] = neon_pink;
	colors[ImGuiCol_ButtonHovered] = ImVec4(neon_pink.x + 0.2f, neon_pink.y + 0.2f, neon_pink.z + 0.2f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(neon_pink.x - 0.1f, neon_pink.y - 0.1f, neon_pink.z - 0.1f, 1.00f);

	// Headers
	colors[ImGuiCol_Header] = neon_blue;
	colors[ImGuiCol_HeaderHovered] = ImVec4(neon_blue.x + 0.2f, neon_blue.y + 0.2f, neon_blue.z + 0.2f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(neon_blue.x - 0.1f, neon_blue.y - 0.1f, neon_blue.z - 0.1f, 1.00f);

	// Scrollbars
	colors[ImGuiCol_ScrollbarBg] = ImVec4(bg_medium.x, bg_medium.y, bg_medium.z, 0.60f);
	colors[ImGuiCol_ScrollbarGrab] = neon_purple;
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(neon_purple.x + 0.2f, neon_purple.y + 0.2f, neon_purple.z + 0.2f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(neon_purple.x - 0.1f, neon_purple.y - 0.1f, neon_purple.z - 0.1f, 1.00f);

	// Checkbox, RadioButton, etc.
	colors[ImGuiCol_CheckMark] = neon_green;
	colors[ImGuiCol_SliderGrab] = neon_pink;
	colors[ImGuiCol_SliderGrabActive] = neon_blue;

	// Text input
	colors[ImGuiCol_TextSelectedBg] = ImVec4(neon_blue.x, neon_blue.y, neon_blue.z, 0.35f);

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4(bg_medium.x, bg_medium.y, bg_medium.z, 0.80f);
	colors[ImGuiCol_TabHovered] = neon_purple;
	colors[ImGuiCol_TabActive] = ImVec4(neon_purple.x * 0.8f, neon_purple.y * 0.8f, neon_purple.z * 0.8f, 0.80f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(bg_medium.x, bg_medium.y, bg_medium.z, 0.80f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(bg_light.x, bg_light.y, bg_light.z, 0.80f);

	// Separators - GLOWING EDGES
	colors[ImGuiCol_Separator] = neon_blue;
	colors[ImGuiCol_SeparatorHovered] = neon_pink;
	colors[ImGuiCol_SeparatorActive] = neon_green;

	// Resize grip
	colors[ImGuiCol_ResizeGrip] = neon_purple;
	colors[ImGuiCol_ResizeGripHovered] = neon_pink;
	colors[ImGuiCol_ResizeGripActive] = neon_blue;

	// Plot lines
	colors[ImGuiCol_PlotLines] = neon_green;
	colors[ImGuiCol_PlotLinesHovered] = neon_pink;
	colors[ImGuiCol_PlotHistogram] = neon_blue;
	colors[ImGuiCol_PlotHistogramHovered] = neon_purple;

	// Navigation
	colors[ImGuiCol_NavHighlight] = neon_pink;
	colors[ImGuiCol_NavWindowingHighlight] = neon_blue;
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

	// For extra glow effect (requires custom rendering)
	style.Alpha = 1.0f;
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
		SetCyberpunkNeonTheme();
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

		static float pulse_speed = 3.5f;
		static float pulse = 0.5f + 0.5f * sinf(ImGui::GetTime() * pulse_speed);
		static float pulse_alt = 0.5f + 0.5f * cosf(ImGui::GetTime() * pulse_speed * 0.8f);
		static ImVec4 neon_pink = ImVec4(1.0f, 0.1f, 0.6f, 1.0f);
		static ImVec4 neon_blue = ImVec4(0.1f, 0.9f, 1.0f, 1.0f);
		static ImVec4 neon_purple = ImVec4(0.7f, 0.3f, 1.0f, 1.0f);

		// ===== MAIN UI RENDERING =====
		render();  // This calls your RenderMain() function

		// ===== POST-RENDER EFFECTS =====
		if (cfg::showMenu) {
			ImDrawList* bg_draw_list = ImGui::GetBackgroundDrawList();
			ImVec2 display_size = ImGui::GetIO().DisplaySize;

			// Fullscreen glow overlay (drawn before ImGui content)
			bg_draw_list->AddRectFilledMultiColor(
				ImVec2(0, 0),
				display_size,
				IM_COL32(0, 0, 0, 0),
				IM_COL32(0, 0, 0, 0),
				IM_COL32(100, 0, 255, 10 + (int)(5 * pulse)),  // Purple glow
				IM_COL32(0, 200, 255, 10 + (int)(5 * pulse))  // Blue glow
			);

			// Scanlines (drawn over everything)
			for (int y = 0; y < display_size.y; y += 3) {
				bg_draw_list->AddLine(
					ImVec2(0, y),
					ImVec2(display_size.x, y),
					IM_COL32(0, 255, 255, 5 + (int)(3 * pulse))
				);
			}
		}

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	}

	// Generic ImGui renderer for OpenGL2 backend
	void RenderOpenGL2(
		IN  HGLRC     WglContext,
		IN  HDC       hDc,
		IN  _VOID_2() render,
		IN  _VOID_1() render_inner,
		OUT bool* status)
	{
		auto tStatus = true;
		auto o_WglContext = wglGetCurrentContext();
		tStatus &= wglMakeCurrent(hDc, WglContext);

		// ===== CYBERPUNK ANIMATION SETUP =====
		static float pulse_speed = 3.5f;
		static float pulse = 0.5f + 0.5f * sinf(ImGui::GetTime() * pulse_speed);
		static float pulse_alt = 0.5f + 0.5f * cosf(ImGui::GetTime() * pulse_speed * 0.8f);
		static ImVec4 neon_pink = ImVec4(1.0f, 0.1f, 0.6f, 1.0f);
		static ImVec4 neon_blue = ImVec4(0.1f, 0.9f, 1.0f, 1.0f);
		static ImVec4 neon_purple = ImVec4(0.7f, 0.3f, 1.0f, 1.0f);

		// Start new ImGui frame
		ImGui_ImplOpenGL2_NewFrame();

		// ===== MAIN UI RENDERING =====
		render(render_inner);  // This calls your RenderMain() function

		// ===== POST-RENDER EFFECTS =====
		if (cfg::showMenu) {
			ImDrawList* bg_draw_list = ImGui::GetBackgroundDrawList();
			ImVec2 display_size = ImGui::GetIO().DisplaySize;

			// Fullscreen glow overlay (drawn before ImGui content)
			bg_draw_list->AddRectFilledMultiColor(
				ImVec2(0, 0),
				display_size,
				IM_COL32(0, 0, 0, 0),
				IM_COL32(0, 0, 0, 0),
				IM_COL32(100, 0, 255, 10 + (int)(5 * pulse)),  // Purple glow
				IM_COL32(0, 200, 255, 10 + (int)(5 * pulse))  // Blue glow
			);

			// Scanlines (drawn over everything)
			for (int y = 0; y < display_size.y; y += 3) {
				bg_draw_list->AddLine(
					ImVec2(0, y),
					ImVec2(display_size.x, y),
					IM_COL32(0, 255, 255, 5 + (int)(3 * pulse))
				);
			}
		}

		// ===== FINAL RENDER =====
		ImGui::Render();
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

		// Restore original OpenGL context
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


