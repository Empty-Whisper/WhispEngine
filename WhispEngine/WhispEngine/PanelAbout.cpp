#include "PanelAbout.h"
#include "Application.h"

PanelAbout::PanelAbout(const bool &start_active, const SDL_Scancode &shortcut1, const SDL_Scancode &shortcut2, const SDL_Scancode &shortcut3)
	: Panel("Configuration", start_active, shortcut1, shortcut2, shortcut3)
{
}

PanelAbout::~PanelAbout()
{
}

void PanelAbout::Update()
{
	if (ImGui::Begin("About Whisp Engine", &active))
	{
		static ImVec2 logo_size(106, 163);
		static Texture* logo_txt = App->importer->ImportTexture("Assets/logo.png");
		ImGui::Text("Version 0.3-alpha");
		ImGui::Image((ImTextureID)logo_txt->id, logo_size);

		ImGui::Separator();
		ImGui::NewLine();
		ImGui::Text("By Christian Martinez @christt105 and Marc Galvez @optus23 for learning purposes.");
		ImGui::Text("Whisp Engine is licensed under the MIT LICENSE, see LICENSE for more information.");
		ImGui::NewLine();
		if (ImGui::Button("Github @christ105")) 	ShellExecuteA(NULL, "open", "https://github.com/christt105", NULL, NULL, SW_SHOWNORMAL);
		ImGui::SameLine();
		if (ImGui::Button("Github @optus23")) 	ShellExecuteA(NULL, "open", "https://github.com/optus23", NULL, NULL, SW_SHOWNORMAL);
		ImGui::NewLine();
		ImGui::Text("3rd Party Libraries used:");

		ImGui::BulletText("SDL v2.0.6"); ImGui::SameLine();
		if (ImGui::Button("Link")) 	ShellExecuteA(NULL, "open", "https://www.libsdl.org/license.php", NULL, NULL, SW_SHOWNORMAL);

		ImGui::BulletText("STL"); ImGui::SameLine();
		if (ImGui::Button("Link")) 	ShellExecuteA(NULL, "open", "https://docs.oracle.com/database/bdb181/html/installation/win_build_stl.html", NULL, NULL, SW_SHOWNORMAL);

		ImGui::BulletText("Glew.2.0"); ImGui::SameLine();
		if (ImGui::Button("Link")) 	ShellExecuteA(NULL, "open", "https://github.com/nigels-com/glew", NULL, NULL, SW_SHOWNORMAL);

		ImGui::BulletText("OpenGL3"); ImGui::SameLine();
		if (ImGui::Button("Link")) 	ShellExecuteA(NULL, "open", "https://opengl.org/", NULL, NULL, SW_SHOWNORMAL);

		ImGui::BulletText("MathGeoLib v1.5"); ImGui::SameLine();
		if (ImGui::Button("Link")) 	ShellExecuteA(NULL, "open", "https://github.com/juj/MathGeoLib", NULL, NULL, SW_SHOWNORMAL);

		ImGui::BulletText("PCG v0.9"); ImGui::SameLine();
		if (ImGui::Button("Link")) 	ShellExecuteA(NULL, "open", "http://www.pcg-random.org/", NULL, NULL, SW_SHOWNORMAL);

		ImGui::BulletText("Mmgr"); ImGui::SameLine();
		if (ImGui::Button("Link")) 	ShellExecuteA(NULL, "open", "http://www.flipcode.com/archives/Presenting_A_Memory_Manager.shtml", NULL, NULL, SW_SHOWNORMAL);

		ImGui::BulletText("ImGui v1.73"); ImGui::SameLine();
		if (ImGui::Button("Link")) 	ShellExecuteA(NULL, "open", "https://github.com/ocornut/imgui", NULL, NULL, SW_SHOWNORMAL);

		ImGui::BulletText("JSON parser v3.7"); ImGui::SameLine();
		if (ImGui::Button("Link")) 	ShellExecuteA(NULL, "open", "https://github.com/nlohmann/json", NULL, NULL, SW_SHOWNORMAL);

		ImGui::BulletText("par_shape.h"); ImGui::SameLine();
		if (ImGui::Button("Link")) 	ShellExecuteA(NULL, "open", "https://prideout.net/shapes", NULL, NULL, SW_SHOWNORMAL);

		ImGui::BulletText("Assimp v4.1"); ImGui::SameLine();
		if (ImGui::Button("Link")) 	ShellExecuteA(NULL, "open", "http://www.assimp.org/", NULL, NULL, SW_SHOWNORMAL);

		ImGui::BulletText("DevIL v1.8"); ImGui::SameLine();
		if (ImGui::Button("Link")) 	ShellExecuteA(NULL, "open", "http://openil.sourceforge.net/", NULL, NULL, SW_SHOWNORMAL);

		ImGui::BulletText("GPU detect v1.1"); ImGui::SameLine();
		if (ImGui::Button("Link")) 	ShellExecuteA(NULL, "open", "https://github.com/MatthewKing/DeviceId", NULL, NULL, SW_SHOWNORMAL);


		ImGui::NewLine();
		ImGui::Checkbox("Show MIT LICENSE", &checkbox_mit_license_window);
		if (checkbox_mit_license_window)
		{
			ImGui::SetWindowSize(ImVec2(580, 1000));
			ImGui::Text("MIT LICENSE");	ImGui::NewLine();
			ImGui::Text("Copyright (c) 2019 Empty-Whisper"); ImGui::NewLine();
			ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy\nof this software and associated documentation files(the 'Software'), to deal\nin the Software without restriction, including without limitation the rights\nto use, copy, modify, merge, publish, distribute, sublicense, and/or sell\ncopies of the Software, and to permit persons to whom the Software is\nfurnished to do so, subject to the following conditions :"); ImGui::NewLine();
			ImGui::Text("The above copyright notice and this permission notice shall be included in all\ncopies or substantial portions of the Software."); ImGui::NewLine();
			ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\nIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\nFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE\nAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\nLIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\nOUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\nSOFTWARE."); ImGui::NewLine();
		}
		else
		{
			ImGui::SetWindowSize(ImVec2(580, 700));
		}
	}
	ImGui::End();

}
