#include <Windows.h>
#include <functional> 
#include <GL/gl.h>
#include "kiero/kiero.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_opengl2.h"
#include "../cfg.h"
#include "../IconsFontAwesome5.h"
#include "../font_data.h"

bool InitFonts(ImGuiIO& io)
{
	// Clear any existing fonts
	io.Fonts->Clear();

	// Load regular font (Segoe UI)
	ImFont* mainFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 16.0f);
	if (!mainFont) {
		// Fallback to default font
		mainFont = io.Fonts->AddFontDefault();
	}

	// Font Awesome config
	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;

	// Merge Font Awesome into the regular font
	io.Fonts->AddFontFromMemoryTTF(
		(void*)fa_solid_900_ttf,
		fa_solid_900_ttf_len,
		16.0f,
		&icons_config,
		icons_ranges
	);

	// Build font atlas
	io.Fonts->Build();

	return true;
}

void SetCyberpunkNeonTheme()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	// ULTRA FUTURISTIC SPACING - Making everything feel premium
	style.WindowPadding = ImVec2(20, 20);
	style.FramePadding = ImVec2(14, 8);
	style.ItemSpacing = ImVec2(14, 10);
	style.ItemInnerSpacing = ImVec2(8, 8);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 25.0f;
	style.ScrollbarSize = 16.0f;
	style.GrabMinSize = 14.0f;
	style.TabMinWidthForCloseButton = 0.0f;

	// SURGICAL PRECISION ROUNDING - Perfect balance between sharp and smooth
	style.WindowRounding = 8.0f;
	style.ChildRounding = 6.0f;
	style.FrameRounding = 6.0f;
	style.PopupRounding = 8.0f;
	style.ScrollbarRounding = 12.0f;
	style.GrabRounding = 4.0f;
	style.TabRounding = 6.0f;
	style.WindowTitleAlign = ImVec2(0.50f, 0.50f);

	// BORDER STYLING - Glow effect simulation
	style.WindowBorderSize = 1.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupBorderSize = 1.0f;
	style.FrameBorderSize = 1.0f;
	style.TabBorderSize = 1.0f;

	// ULTRA ENHANCED COLOR PALETTE - Each color scientifically crafted for maximum impact

	// BASE LAYERS - Deep space darkness with subtle energy
	const ImVec4 void_black = ImVec4(0.02f, 0.02f, 0.08f, 0.95f);      // The abyss itself
	const ImVec4 shadow_purple = ImVec4(0.06f, 0.03f, 0.15f, 0.90f);   // Dark matter
	const ImVec4 deep_violet = ImVec4(0.10f, 0.06f, 0.20f, 0.85f);     // Quantum foam
	const ImVec4 medium_purple = ImVec4(0.15f, 0.10f, 0.25f, 0.80f);   // Digital mist

	// NEON ARSENAL - Pure energy in color form
	const ImVec4 plasma_pink = ImVec4(1.00f, 0.05f, 0.65f, 0.90f);     // Hot plasma
	const ImVec4 electric_cyan = ImVec4(0.00f, 0.90f, 1.00f, 0.90f);   // Lightning essence
	const ImVec4 void_purple = ImVec4(0.75f, 0.25f, 1.00f, 0.90f);     // Psychic energy
	const ImVec4 matrix_green = ImVec4(0.25f, 1.00f, 0.35f, 0.90f);    // Code rain
	const ImVec4 solar_orange = ImVec4(1.00f, 0.45f, 0.00f, 0.90f);    // Digital fire
	const ImVec4 neural_blue = ImVec4(0.15f, 0.70f, 1.00f, 0.90f);     // Brain waves

	// SOLID VARIANTS - For maximum visibility when needed
	const ImVec4 solid_plasma = ImVec4(1.00f, 0.05f, 0.65f, 1.00f);
	const ImVec4 solid_cyan = ImVec4(0.00f, 0.90f, 1.00f, 1.00f);
	const ImVec4 solid_purple = ImVec4(0.75f, 0.25f, 1.00f, 1.00f);
	const ImVec4 solid_green = ImVec4(0.25f, 1.00f, 0.35f, 1.00f);

	// TEXT HIERARCHY - Crystal clear communication
	const ImVec4 hologram_white = ImVec4(0.98f, 0.98f, 1.00f, 1.00f);     // Pure information
	const ImVec4 ghost_gray = ImVec4(0.75f, 0.78f, 0.85f, 0.90f);         // Secondary data
	const ImVec4 whisper_gray = ImVec4(0.55f, 0.58f, 0.65f, 0.70f);       // Disabled state

	// GLOW EFFECTS - Simulated with alpha layering
	const ImVec4 glow_pink = ImVec4(1.00f, 0.05f, 0.65f, 0.25f);
	const ImVec4 glow_cyan = ImVec4(0.00f, 0.90f, 1.00f, 0.25f);
	const ImVec4 glow_purple = ImVec4(0.75f, 0.25f, 1.00f, 0.25f);

	// FOUNDATION - The canvas of cyberspace
	colors[ImGuiCol_Text] = hologram_white;
	colors[ImGuiCol_TextDisabled] = whisper_gray;
	colors[ImGuiCol_WindowBg] = void_black;
	colors[ImGuiCol_ChildBg] = ImVec4(shadow_purple.x, shadow_purple.y, shadow_purple.z, 0.60f);
	colors[ImGuiCol_PopupBg] = ImVec4(deep_violet.x, deep_violet.y, deep_violet.z, 0.95f);

	// BORDERS - The electric edges of reality
	colors[ImGuiCol_Border] = ImVec4(void_purple.x, void_purple.y, void_purple.z, 0.55f);
	colors[ImGuiCol_BorderShadow] = ImVec4(plasma_pink.x, plasma_pink.y, plasma_pink.z, 0.10f);

	// INTERACTIVE SURFACES - Where user meets machine
	colors[ImGuiCol_FrameBg] = ImVec4(deep_violet.x, deep_violet.y, deep_violet.z, 0.65f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(void_purple.x * 0.6f, void_purple.y * 0.6f, void_purple.z * 0.6f, 0.45f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(void_purple.x * 0.8f, void_purple.y * 0.8f, void_purple.z * 0.8f, 0.65f);

	// TITLE BARS - Command center aesthetics
	colors[ImGuiCol_TitleBg] = ImVec4(shadow_purple.x, shadow_purple.y, shadow_purple.z, 0.85f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(void_purple.x * 0.4f, void_purple.y * 0.4f, void_purple.z * 0.4f, 0.90f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(shadow_purple.x, shadow_purple.y, shadow_purple.z, 0.45f);

	// BUTTONS - The keys to digital power
	colors[ImGuiCol_Button] = ImVec4(plasma_pink.x * 0.7f, plasma_pink.y * 0.7f, plasma_pink.z * 0.7f, 0.70f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(plasma_pink.x, plasma_pink.y, plasma_pink.z, 0.85f);
	colors[ImGuiCol_ButtonActive] = ImVec4(plasma_pink.x + 0.15f, plasma_pink.y + 0.15f, plasma_pink.z + 0.15f, 0.95f);

	// HEADERS - Information superhighway
	colors[ImGuiCol_Header] = ImVec4(electric_cyan.x * 0.6f, electric_cyan.y * 0.6f, electric_cyan.z * 0.6f, 0.65f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(electric_cyan.x, electric_cyan.y, electric_cyan.z, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(electric_cyan.x + 0.1f, electric_cyan.y + 0.1f, electric_cyan.z + 0.1f, 0.90f);

	// SEPARATOR LINES - The grid of cyberspace
	colors[ImGuiCol_Separator] = ImVec4(electric_cyan.x, electric_cyan.y, electric_cyan.z, 0.60f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(plasma_pink.x, plasma_pink.y, plasma_pink.z, 0.80f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(matrix_green.x, matrix_green.y, matrix_green.z, 0.90f);

	// SCROLLBARS - Navigating the data stream
	colors[ImGuiCol_ScrollbarBg] = ImVec4(shadow_purple.x, shadow_purple.y, shadow_purple.z, 0.40f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(void_purple.x, void_purple.y, void_purple.z, 0.70f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(void_purple.x + 0.15f, void_purple.y + 0.15f, void_purple.z + 0.15f, 0.85f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(plasma_pink.x, plasma_pink.y, plasma_pink.z, 0.95f);

	// CONTROL ELEMENTS - Precision instruments
	colors[ImGuiCol_CheckMark] = solid_green;
	colors[ImGuiCol_SliderGrab] = ImVec4(plasma_pink.x, plasma_pink.y, plasma_pink.z, 0.85f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(electric_cyan.x, electric_cyan.y, electric_cyan.z, 0.95f);

	// TEXT SELECTION - Highlighting the important bits
	colors[ImGuiCol_TextSelectedBg] = ImVec4(electric_cyan.x, electric_cyan.y, electric_cyan.z, 0.35f);

	// DRAG AND DROP - Moving data through space
	colors[ImGuiCol_DragDropTarget] = ImVec4(matrix_green.x, matrix_green.y, matrix_green.z, 0.80f);

	// NAVIGATION - Moving through digital space
	colors[ImGuiCol_NavHighlight] = ImVec4(plasma_pink.x, plasma_pink.y, plasma_pink.z, 0.85f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(electric_cyan.x, electric_cyan.y, electric_cyan.z, 0.75f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.10f, 0.10f, 0.15f, 0.60f);

	// MODAL OVERLAYS - The focus field
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.70f);

	// TAB SYSTEM - Multitasking in the matrix
	colors[ImGuiCol_Tab] = ImVec4(shadow_purple.x, shadow_purple.y, shadow_purple.z, 0.70f);
	colors[ImGuiCol_TabHovered] = ImVec4(void_purple.x, void_purple.y, void_purple.z, 0.85f);
	colors[ImGuiCol_TabActive] = ImVec4(void_purple.x * 0.9f, void_purple.y * 0.9f, void_purple.z * 0.9f, 0.90f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(shadow_purple.x, shadow_purple.y, shadow_purple.z, 0.50f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(deep_violet.x, deep_violet.y, deep_violet.z, 0.70f);

	// RESIZE CONTROLS - Shaping reality
	colors[ImGuiCol_ResizeGrip] = ImVec4(void_purple.x, void_purple.y, void_purple.z, 0.45f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(plasma_pink.x, plasma_pink.y, plasma_pink.z, 0.70f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(electric_cyan.x, electric_cyan.y, electric_cyan.z, 0.90f);

	// DATA VISUALIZATION - Making numbers beautiful
	colors[ImGuiCol_PlotLines] = ImVec4(matrix_green.x, matrix_green.y, matrix_green.z, 0.85f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(plasma_pink.x, plasma_pink.y, plasma_pink.z, 0.95f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(electric_cyan.x, electric_cyan.y, electric_cyan.z, 0.85f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(void_purple.x, void_purple.y, void_purple.z, 0.95f);

	// TABLE SYSTEM - Organized data streams
	colors[ImGuiCol_TableHeaderBg] = ImVec4(deep_violet.x, deep_violet.y, deep_violet.z, 0.80f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(void_purple.x, void_purple.y, void_purple.z, 0.70f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(void_purple.x, void_purple.y, void_purple.z, 0.35f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(shadow_purple.x, shadow_purple.y, shadow_purple.z, 0.25f);

	// FINAL TOUCHES - The polish that makes it shine
	style.Alpha = 0.95f;  // Overall transparency for that holographic feel
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
		InitFonts(io);
		return ExitStatus(status, tStatus);
	}

	// Generic ImGui renderer for Win32 backend
	void RenderWin32(std::function<void()> render) {
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		render();

		if (cfg::showMenu && cfg::backgroundFx) RenderCustomShaders();
		if (cfg::nightFx && cfg::nightFxDraw) CosmicCrosshair();

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

		// Start new ImGui frame
		ImGui_ImplOpenGL2_NewFrame();

		// ===== MAIN UI RENDERING =====
		render(render_inner);  // This calls your RenderMain() function

		// ===== POST-RENDER EFFECTS =====

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


