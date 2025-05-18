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
	style.WindowRounding = 4.0f;
	style.ChildRounding = 4.0f;
	style.FrameRounding = 4.0f;
	style.PopupRounding = 4.0f;
	style.ScrollbarRounding = 4.0f;
	style.GrabRounding = 4.0f;
	style.TabRounding = 4.0f;

	// Cyberpunk color palette - NEON INTENSITY with MORE TRANSPARENCY
	const ImVec4 bg_dark = ImVec4(0.03f, 0.03f, 0.06f, 0.85f);     // Deep purple-black with transparency
	const ImVec4 bg_medium = ImVec4(0.08f, 0.05f, 0.12f, 0.82f);   // Dark purple with transparency
	const ImVec4 bg_light = ImVec4(0.12f, 0.08f, 0.18f, 0.80f);    // Medium purple with transparency

	// Neon accent colors with transparency
	const ImVec4 neon_pink = ImVec4(1.00f, 0.08f, 0.58f, 0.85f);    // Hot pink
	const ImVec4 neon_blue = ImVec4(0.08f, 0.85f, 1.00f, 0.85f);    // Electric blue
	const ImVec4 neon_purple = ImVec4(0.70f, 0.30f, 1.00f, 0.85f);  // Purple haze
	const ImVec4 neon_green = ImVec4(0.30f, 1.00f, 0.40f, 0.85f);   // Matrix green

	// Solid versions of neon colors for text and important elements
	const ImVec4 solid_pink = ImVec4(1.00f, 0.08f, 0.58f, 1.00f);
	const ImVec4 solid_blue = ImVec4(0.08f, 0.85f, 1.00f, 1.00f);
	const ImVec4 solid_purple = ImVec4(0.70f, 0.30f, 1.00f, 1.00f);
	const ImVec4 solid_green = ImVec4(0.30f, 1.00f, 0.40f, 1.00f);

	const ImVec4 text_primary = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);   // Bright white
	const ImVec4 text_secondary = ImVec4(0.70f, 0.71f, 0.75f, 0.85f); // Grayish with transparency
	const ImVec4 border_color = ImVec4(0.40f, 0.20f, 0.60f, 0.40f);   // Purple border with more transparency

	// Main colors
	colors[ImGuiCol_Text] = text_primary;
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 0.65f);
	colors[ImGuiCol_WindowBg] = bg_dark;
	colors[ImGuiCol_ChildBg] = ImVec4(bg_medium.x, bg_medium.y, bg_medium.z, 0.70f); // More transparent
	colors[ImGuiCol_PopupBg] = ImVec4(bg_medium.x, bg_medium.y, bg_medium.z, 0.85f);
	colors[ImGuiCol_Border] = ImVec4(neon_purple.x, neon_purple.y, neon_purple.z, 0.35f); // Subtle border
	colors[ImGuiCol_BorderShadow] = ImVec4(neon_purple.x, neon_purple.y, neon_purple.z, 0.15f);

	// Frame background - more transparent
	colors[ImGuiCol_FrameBg] = ImVec4(bg_light.x, bg_light.y, bg_light.z, 0.60f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(neon_purple.x * 0.4f, neon_purple.y * 0.4f, neon_purple.z * 0.4f, 0.35f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(neon_purple.x * 0.5f, neon_purple.y * 0.5f, neon_purple.z * 0.5f, 0.50f);

	// Title bars - semi-transparent
	colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.05f, 0.12f, 0.80f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(neon_purple.x * 0.25f, neon_purple.y * 0.25f, neon_purple.z * 0.25f, 0.85f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.08f, 0.05f, 0.12f, 0.50f);

	// Buttons - ALTERNATING NEON COLORS with transparency
	colors[ImGuiCol_Button] = ImVec4(neon_pink.x, neon_pink.y, neon_pink.z, 0.60f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(neon_pink.x + 0.2f, neon_pink.y + 0.2f, neon_pink.z + 0.2f, 0.70f);
	colors[ImGuiCol_ButtonActive] = ImVec4(neon_pink.x - 0.1f, neon_pink.y - 0.1f, neon_pink.z - 0.1f, 0.80f);

	// Headers - more transparent
	colors[ImGuiCol_Header] = ImVec4(neon_blue.x, neon_blue.y, neon_blue.z, 0.55f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(neon_blue.x + 0.2f, neon_blue.y + 0.2f, neon_blue.z + 0.2f, 0.65f);
	colors[ImGuiCol_HeaderActive] = ImVec4(neon_blue.x - 0.1f, neon_blue.y - 0.1f, neon_blue.z - 0.1f, 0.75f);

	// Scrollbars - more transparent
	colors[ImGuiCol_ScrollbarBg] = ImVec4(bg_medium.x, bg_medium.y, bg_medium.z, 0.40f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(neon_purple.x, neon_purple.y, neon_purple.z, 0.60f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(neon_purple.x + 0.2f, neon_purple.y + 0.2f, neon_purple.z + 0.2f, 0.70f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(neon_purple.x - 0.1f, neon_purple.y - 0.1f, neon_purple.z - 0.1f, 0.80f);

	// Checkbox, RadioButton, etc. - keeping solid for visibility
	colors[ImGuiCol_CheckMark] = solid_green;
	colors[ImGuiCol_SliderGrab] = ImVec4(neon_pink.x, neon_pink.y, neon_pink.z, 0.75f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(neon_blue.x, neon_blue.y, neon_blue.z, 0.85f);

	// Text input - more subtle selection
	colors[ImGuiCol_TextSelectedBg] = ImVec4(neon_blue.x, neon_blue.y, neon_blue.z, 0.25f);

	// Tabs - with transparency gradient
	colors[ImGuiCol_Tab] = ImVec4(bg_medium.x, bg_medium.y, bg_medium.z, 0.65f);
	colors[ImGuiCol_TabHovered] = ImVec4(neon_purple.x, neon_purple.y, neon_purple.z, 0.70f);
	colors[ImGuiCol_TabActive] = ImVec4(neon_purple.x * 0.8f, neon_purple.y * 0.8f, neon_purple.z * 0.8f, 0.75f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(bg_medium.x, bg_medium.y, bg_medium.z, 0.55f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(bg_light.x, bg_light.y, bg_light.z, 0.65f);

	// Separators - GLOWING EDGES with transparency
	colors[ImGuiCol_Separator] = ImVec4(neon_blue.x, neon_blue.y, neon_blue.z, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(neon_pink.x, neon_pink.y, neon_pink.z, 0.60f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(neon_green.x, neon_green.y, neon_green.z, 0.70f);

	// Resize grip - with transparency
	colors[ImGuiCol_ResizeGrip] = ImVec4(neon_purple.x, neon_purple.y, neon_purple.z, 0.40f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(neon_pink.x, neon_pink.y, neon_pink.z, 0.60f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(neon_blue.x, neon_blue.y, neon_blue.z, 0.80f);

	// Plot lines - keeping more visible
	colors[ImGuiCol_PlotLines] = ImVec4(neon_green.x, neon_green.y, neon_green.z, 0.80f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(neon_pink.x, neon_pink.y, neon_pink.z, 0.90f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(neon_blue.x, neon_blue.y, neon_blue.z, 0.80f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(neon_purple.x, neon_purple.y, neon_purple.z, 0.90f);

	// Navigation - slightly transparent
	colors[ImGuiCol_NavHighlight] = ImVec4(neon_pink.x, neon_pink.y, neon_pink.z, 0.80f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(neon_blue.x, neon_blue.y, neon_blue.z, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.60f);

	// For extra glow effect
	style.Alpha = 0.95f; // Global transparency
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


