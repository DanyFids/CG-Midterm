#include "UI.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Test_Primitives.h"

UI::UI(int width, int height, glm::vec2 pos, Material* ma)
{
	material = ma;
	dim.x = (float)width;
	dim.y = (float)height;
	position = pos;
}

void UI::Draw(glm::vec2 scrn) {
	SHADER->Use();

	SHADER->SetF("SCREEN.x", scrn.x);
	SHADER->SetF("SCREEN.y", scrn.y);

	SHADER->SetF("DIM.x", dim.x);
	SHADER->SetF("DIM.y", dim.y);

	SHADER->SetF("POS.x", position.x);
	SHADER->SetF("POS.y", position.y);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, material->DIFF);
	SHADER->SetI("tex", 0);

	QUAD->Draw(SHADER, cams);
}

Mesh* UI::QUAD = new Mesh(square, 4, square_index, 6);
Shader* UI::SHADER = nullptr;

