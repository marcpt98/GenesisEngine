#include "WindowAbout.h"
#include "ImGui/imgui.h"
#include "Application.h"
#include "glew/include/glew.h"
#include "Assimp/Assimp/include/version.h"
#include "FileSystem.h"

WindowAbout::WindowAbout() : EditorWindow() 
{
	type = WindowType::ABOUT_WINDOW;
}

WindowAbout::~WindowAbout() {}

void WindowAbout::Draw()
{
	if (ImGui::Begin("About", &visible))
	{
		ImGui::Text("Unfalse Engine v2.0");

		ImGui::Separator();
		ImGui::Text("By Marc Palma Turon and Pol Juan Santos.");
		ImGui::Spacing();
		ImGui::Text("A false engine better than the original.");
		ImGui::Spacing();
		if (ImGui::Button("Marc Palma Github"))
		{
			ShellExecuteA(NULL, "open", "https://github.com/marcpt98", NULL, NULL, SW_SHOWNORMAL);
		}
		if (ImGui::Button("Pol Juan Github"))
		{
			ShellExecuteA(NULL, "open", "https://github.com/Sauko22", NULL, NULL, SW_SHOWNORMAL);
		}
		ImGui::Spacing();
		

		ImGui::Text("External libraries used: ");

		//SDL Version
		ImGui::BulletText("SDL %d.%d.%d", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);

		//OpenGL Version
		static GLint openGL_major = -1;
		static GLint openGL_minor = -1;

		if (openGL_major == -1)
			glGetIntegerv(GL_MAJOR_VERSION, &openGL_major);
		if (openGL_minor == -1)
			glGetIntegerv(GL_MINOR_VERSION, &openGL_minor);

		ImGui::BulletText("OpenGL %d.%d", openGL_major, openGL_minor);

		//MathGeoLib
		ImGui::BulletText("MathGeoLib 1.5");

		//ImGui
		static const char* imgui_version = { ImGui::GetVersion() };
		ImGui::BulletText("ImGui %s", imgui_version);

		//Glew
		ImGui::BulletText("Glew %d.%d.%d", GLEW_VERSION_MAJOR, GLEW_VERSION_MINOR, GLEW_VERSION_MICRO);

		ImGui::BulletText("DevIL 1.8.0");

		ImGui::BulletText("Assimp %d.%d.%d", aiGetVersionMajor(), aiGetVersionMinor(), aiGetVersionRevision());

		static std::string physfs_version;
		FileSystem::GetPhysFSVersion(physfs_version);
		ImGui::BulletText("PhysFS %s", physfs_version.c_str());

		ImGui::BulletText("Parson 1.1.0");

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Text("Licence:");
		ImGui::Spacing();
		ImGui::Text("MIT License Copyright(c)[2019][Pol Juan Santos and Marc Palma Turon]");
		ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy");
		ImGui::Text("of this softwareand associated documentation files(the Software), to deal");
		ImGui::Text("in the Software without restriction, including without limitation the rights to use, copy,");
		ImGui::Text("modify, merge, publish, distribute, sublicense, and /or sell copies of the Software,");
		ImGui::Text("and to permit persons to whom the Software is furnished to do so, subject to the following conditions:");
		ImGui::Text("The above copyright noticeand this permission notice shall be included in all");
		ImGui::Text("copies or substantial portions of the Software.");
		ImGui::Text("THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,");
		ImGui::Text("INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE");
		ImGui::Text("AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,");
		ImGui::Text("DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,");
		ImGui::Text("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.");
	}
	ImGui::End();
}
