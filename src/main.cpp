#include "image_convert.h"
#include "pacman_lvl_detector.h"
#include "serialize.h"

#include "raylib_helpers.h"
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include "graph.h"

using namespace principia;

Image* lvlgen_to_raylib_image(const lvlgen::Image& img) {
	Image* ret = new Image();
	ret->width = img.width;
	ret->height = img.height;
	ret->mipmaps = 1;
	ret->format = PixelFormat::PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;

	auto* data = new std::vector<std::vector<std::vector<l_byte>>>(4, std::vector<std::vector<l_byte>>(img.data.size(), std::vector<l_byte>(img.data[0].size())));
	for (size_t k = 0; k < 4; ++k) {
		for (size_t i = 0; i < img.data.size(); ++i) {
			for (size_t j = 0; j < img.data[0].size(); ++j) {
				data->at(k).at(j)[i] = img.data[i][j][k];
			}
		}
	}
	ret->data = data;
	return ret;
}

void draw_lvlgen_img(lvlgen::Image img) {
	for (size_t i = 0; i < img.width; ++i) {
		for (size_t j = 0; j < img.height; ++j) {
			Color col = lvlgen_pixel_to_color(img.data[i][j]);
			DrawPixel(i, j, col);
			img.data[i][j].DisplayPixel();
		}
		std::cout << "\n\n";
	}
}

auto raylib_to_lvlgen_image(const Image& img) {
	std::vector<std::vector<lvlgen::Pixel>> data = std::vector<std::vector<lvlgen::Pixel>>(img.width, std::vector<lvlgen::Pixel>(img.height));
	for (int k = 0; k < 4; ++k) {
		for (int j = 0; j < img.height; ++j) {
			for (int i = 0; i < img.width; ++i) {
				char* d = (char*)img.data;
				data[j][i][k] = d[k + 4 * i + 4 * img.width * j];
			}
		}
	}
	lvlgen::Image ret = lvlgen::Image(img.width, img.height, 4);
	ret.data = data;
	return ret;
}

auto print_raylib_img_data(const Image& img) {
	//ret.data = &img.data;
	std::string channels[5] = { "Red", "Blue", "Green", "Alpha" "\n" };
	std::cout << "\nImage Start:\n" << channels[0] << std::endl;
	for (size_t k = 0; k < 4; ++k) {
		for (size_t i = 0; i < img.width; ++i) {
			for (size_t j = 0; j < img.height; ++j) {
				auto* helpa = ((std::vector<std::vector<std::vector<l_byte>>>*)img.data);
				auto hd = helpa->at(k).at(j)[i];
				std::cout << " " << (int)hd << " ";
			}
			std::cout << std::endl;
		}
		std::cout << "\nend\n" << channels[k + 1] << std::endl;
	}
}

std::vector<std::vector<bool>> colormatrix_to_wallsmatrix(lvlgen::ColorGraph cg) {
	std::vector<std::vector<bool>> ret = std::vector<std::vector<bool>>(cg.size(), std::vector<bool>(cg[0].size()));
	for (int r = 0; r < cg.size(); ++r) {
		for (int c = 0; c < cg[0].size(); ++c) {
			if (cg[r][c] == lvlgen::kColorBlue) ret[r][c] = false;
			else ret[r][c] = true;
		}
	}
	return ret;
}

/*int main()
{
    // Initialization
    //---------------------------------------------------------------------------------------
    const int screenWidth = 690;
    const int screenHeight = 560;

    InitWindow(screenWidth, screenHeight, "raygui - controls test suite");
    SetExitKey(0);

    // GUI controls initialization
    //----------------------------------------------------------------------------------
    int dropdownBox000Active = 0;
    bool dropDown000EditMode = false;

    int dropdownBox001Active = 0;
    bool dropDown001EditMode = false;

    int spinner001Value = 0;
    bool spinnerEditMode = false;

    int valueBox002Value = 0;
    bool valueBoxEditMode = false;

    char textBoxText[64] = "Text box";
    bool textBoxEditMode = false;

    int listViewScrollIndex = 0;
    int listViewActive = -1;

    int listViewExScrollIndex = 0;
    int listViewExActive = 2;
    int listViewExFocus = -1;
    const char* listViewExList[8] = { "This", "is", "a", "list view", "with", "disable", "elements", "amazing!" };

    char multiTextBoxText[256] = "Multi text box";
    bool multiTextBoxEditMode = false;
    Color colorPickerValue = RED;

    int sliderValue = 50;
    int sliderBarValue = 60;
    float progressValue = 0.4f;

    bool forceSquaredChecked = false;

    float alphaValue = 0.5f;

    int comboBoxActive = 1;

    int toggleGroupActive = 0;

    Vector2 viewScroll = { 0, 0 };
    //----------------------------------------------------------------------------------

    // Custom GUI font loading
    //Font font = LoadFontEx("fonts/rainyhearts16.ttf", 12, 0, 0);
    //GuiSetFont(font);

    bool exitWindow = false;
    bool showMessageBox = false;

    char textInput[256] = { 0 };
    bool showTextInputBox = false;

    char textInputFileName[256] = { 0 };

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!exitWindow)    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        exitWindow = WindowShouldClose();

        if (IsKeyPressed(KEY_ESCAPE)) showMessageBox = !showMessageBox;

        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) showTextInputBox = true;

        if (IsFileDropped())
        {
            FilePathList droppedFiles = LoadDroppedFiles();

            if ((droppedFiles.count > 0) && IsFileExtension(droppedFiles.paths[0], ".rgs")) GuiLoadStyle(droppedFiles.paths[0]);

            UnloadDroppedFiles(droppedFiles);    // Clear internal buffers
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        // raygui: controls drawing
        //----------------------------------------------------------------------------------
        if (dropDown000EditMode || dropDown001EditMode) GuiLock();
        else if (!dropDown000EditMode && !dropDown001EditMode) GuiUnlock();
        //GuiDisable();

        // First GUI column
        //GuiSetStyle(CHECKBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
        forceSquaredChecked = GuiCheckBox(Rectangle({ 25, 108, 15, 15 }), "FORCE CHECK!", forceSquaredChecked);

        GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
        //GuiSetStyle(VALUEBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
        if (GuiSpinner(Rectangle({ 25, 135, 125, 30 }), NULL, & spinner001Value, 0, 100, spinnerEditMode)) spinnerEditMode = !spinnerEditMode;
        if (GuiValueBox(Rectangle({ 25, 175, 125, 30 }), NULL, & valueBox002Value, 0, 100, valueBoxEditMode)) valueBoxEditMode = !valueBoxEditMode;
        GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
        if (GuiTextBox(Rectangle({ 25, 215, 125, 30 }), textBoxText, 64, textBoxEditMode)) textBoxEditMode = !textBoxEditMode;

        GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

        if (GuiButton(Rectangle({ 25, 255, 125, 30 }), GuiIconText(ICON_FILE_SAVE, "Save File"))) showTextInputBox = true;

        GuiGroupBox(Rectangle({ 25, 310, 125, 150 }), "STATES");
        //GuiLock();
        GuiSetState(STATE_NORMAL); if (GuiButton(Rectangle({ 30, 320, 115, 30 }), "NORMAL")) {}
        GuiSetState(STATE_FOCUSED); if (GuiButton(Rectangle({ 30, 355, 115, 30 }), "FOCUSED")) {}
        GuiSetState(STATE_PRESSED); if (GuiButton(Rectangle({ 30, 390, 115, 30 }), "#15#PRESSED")) {}
        GuiSetState(STATE_DISABLED); if (GuiButton(Rectangle({ 30, 425, 115, 30 }), "DISABLED")) {}
        GuiSetState(STATE_NORMAL);
        //GuiUnlock();

        comboBoxActive = GuiComboBox(Rectangle({ 25, 470, 125, 30 }), "ONE;TWO;THREE;FOUR", comboBoxActive);

        // NOTE: GuiDropdownBox must draw after any other control that can be covered on unfolding
        GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
        if (GuiDropdownBox(Rectangle({ 25, 65, 125, 30 }), "#01#ONE;#02#TWO;#03#THREE;#04#FOUR", & dropdownBox001Active, dropDown001EditMode)) dropDown001EditMode = !dropDown001EditMode;

        GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
        if (GuiDropdownBox(Rectangle({ 25, 25, 125, 30 }), "ONE;TWO;fREE", & dropdownBox000Active, dropDown000EditMode)) dropDown000EditMode = !dropDown000EditMode;

        // Second GUI column
        listViewActive = GuiListView(Rectangle({ 165, 25, 140, 140 }), "Charmander;Bulbasaur;#18#Squirtel;Pikachu;Eevee;Pidgey", & listViewScrollIndex, listViewActive);
        listViewExActive = GuiListViewEx(Rectangle({ 165, 180, 140, 200 }), listViewExList, 8, & listViewExFocus, & listViewExScrollIndex, listViewExActive);

        //toggleGroupActive = GuiToggleGroup(Rectangle({ 165, 400, 140, 25 }), "#1#ONE\n#3#TWO\n#8#THfREE\n#23#", toggleGroupActive);
        toggleGroupActive = DisplayMenuOptions(toggleGroupActive);
        // Third GUI column
        if (GuiTextBoxMulti(Rectangle({ 320, 25, 225, 140 }), multiTextBoxText, 256, multiTextBoxEditMode)) multiTextBoxEditMode = !multiTextBoxEditMode;
        colorPickerValue = GuiColorPicker(Rectangle({ 320, 185, 196, 192 }), NULL, colorPickerValue);

        sliderValue = GuiSlider(Rectangle({ 355, 400, 165, 20 }), "TEST", TextFormat("%2.2f", (float)sliderValue), sliderValue, -50, 100);
        sliderBarValue = GuiSliderBar(Rectangle({ 320, 430, 200, 20 }), NULL, TextFormat("%i", (int)sliderBarValue), sliderBarValue, 0, 100);
        progressValue = GuiProgressBar(Rectangle({ 320, 460, 200, 20 }), NULL, NULL, progressValue, 0, 1);

        // NOTE: View rectangle could be used to perform some scissor test
        Rectangle view = GuiScrollPanel(Rectangle({ 560, 25, 100, 160 }), NULL, Rectangle({ 560, 25, 200, 400 }), & viewScroll);

        GuiPanel(Rectangle({ 560, 25 + 180, 100, 160 }), "Panel Info\niiil;hj\pika");
        //DisplayKey(true);
        GuiGrid(Rectangle({ 560, 25 + 180 + 180, 100, 120 }), NULL, 20, 2);

        GuiStatusBar(Rectangle({ 0, (float)GetScreenHeight() - 20, (float)GetScreenWidth(), 20 }), "This is a status bar");

        alphaValue = GuiColorBarAlpha(Rectangle({ 320, 490, 200, 30 }), NULL, alphaValue);

        if (showMessageBox)
        {
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, 0.8f));
            int result = GuiMessageBox(Rectangle({ (float)GetScreenWidth() / 2 - 125, (float)GetScreenHeight() / 2 - 50, 250, 100 }), GuiIconText(ICON_EXIT, "Close Window"), "Do you really want to exit?", "Yes;No");

            if ((result == 0) || (result == 2)) showMessageBox = false;
            else if (result == 1) exitWindow = true;
        }

        if (showTextInputBox)
        {
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, 0.8f));
            int result = GuiTextInputBox(Rectangle({ (float)GetScreenWidth() / 2 - 120, (float)GetScreenHeight() / 2 - 60, 240, 140 }), "Save", GuiIconText(ICON_FILE_SAVE, "Save file as..."), "Ok;Cancel", textInput, 255, NULL);

            if (result == 1)
            {
                // TODO: Validate textInput value and save

                strcpy(textInputFileName, textInput);
            }

            if ((result == 0) || (result == 1) || (result == 2))
            {
                showTextInputBox = false;
                strcpy(textInput, "\0");
            }
        }
        //----------------------------------------------------------------------------------

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}*/

/**/
int main(int argc, char** argv) {
	auto img = lvlgen::Image("assets/pacmap001.bmp");
	auto converter = lvlgen::ImageConverter(img);
	auto lvl = lvlgen::BlacmanLvlConverter(converter.ColorMatrix()).GetLvl();

	auto grid = colormatrix_to_wallsmatrix(converter.ColorMatrix());
	lvlgen::Graph graf = lvlgen::Graph(grid.size(), grid[0].size());
	graf.build(grid);

	auto firstwall_pos = lvl.walls[0].pos;
	auto firstwall_size = lvl.walls[0].size;
	auto target_pos = glm::vec3(16.f, 1.f, 31.5f);
	auto target_size = glm::vec3(15.f, 1.f, 0.5f);

	std::cout << "Position (" << display_vec3(firstwall_pos) << ")\n";
	std::cout << "Size (" << display_vec3(firstwall_size) << ")\n";
	std::cout << "\nTarget Pos (" << display_vec3(target_pos) << ")\n";
	std::cout << "Target Size (" << display_vec3(target_size) << ")\n\n";
	
	//assert(firstwall_pos == target_pos);
	//assert(firstwall_size == target_size);
	

	//auto serializer = lvlgen::Serializer();
	//serializer.SaveLvl("C:\\dev\\PrincipiumGames\\ShinyAfroMan\\Assets\\Levels\\Test\\Scenes\\testlvl5.xml", lvl, 1);
	//return 0;




	// ------------------------------------- RAY LIB ------------------------------- \\

	int scale = 20;
	int width = img.width * scale;
	int height = img.height * scale;

    int menu_options = 0;
    int graph_options = 0;
    int search_options = -1;


	SetConfigFlags(FLAG_WINDOW_UNDECORATED);
	// General variables
	Vector2 mousePosition = { 0 };
	Vector2 windowPosition = { 500, 200 };
	Vector2 panOffset = mousePosition;
	Rectangle panelRec = { 20, 40, 200, 150 };
	Rectangle panelContentRec = { 0, 0, 340, 340 };
	Vector2 panelScroll = { 99, -20 };

	float extra_box_width = (float)width / 3;
	float tab = 10;
    float panel_height = 175;

	bool dragWindow = false;

	SetWindowPosition(windowPosition.x, windowPosition.y);
	SetTargetFPS(60);
	bool exitWindow = false;


	InitWindow(width + extra_box_width, height, "raylib [core] example - basic window");


    //TEXTURES//
	auto rimg = LoadImage("C://dev//levelgenerator//assets//pacmap001.png");
	copy_lvlgen_to_raylib(&rimg, img);
	ImageResizeNN(&rimg, width, height);
    auto txtr = LoadTextureFromImage(rimg);

    //rimg = LoadImage("C://dev//levelgenerator//assets//pacmap001.png");
    //copy_graph_to_raylib(&rimg, graf);
    //ImageResizeNN(&rimg, width, height);
    //textures[1] = LoadTextureFromImage(rimg);


	while (!exitWindow && !WindowShouldClose())
	{
		mousePosition = GetMousePosition();
		BeginDrawing();
		DrawTexture(txtr, 0, 0, WHITE);

		exitWindow = GuiWindowBox(Rectangle({(float)width, 0, extra_box_width, (float)height}), "#198# PORTABLE WINDOW");

		//Rectangle view = GuiScrollPanel(panelRec, NULL, panelContentRec, &panelScroll);

		//BeginScissorMode(view.x, view.y, view.width, view.height);
		//GuiGrid(Rectangle({ panelRec.x + panelScroll.x, panelRec.y + panelScroll.y, panelContentRec.width, panelContentRec.height }), NULL, 16, 3);
		//EndScissorMode();

		DrawText(TextFormat("Mouse Position: [ %.0f, %.0f ]", mousePosition.x, mousePosition.y), width + tab, 30, 10, DARKGRAY);
        DrawText(TextFormat("Graph Position: [ %.0f, %.0f ]", mousePosition.x / scale, mousePosition.y / scale), width + tab, 45, 10, DARKGRAY);
		
        menu_options = GuiToggleGroup(Rectangle({ (float)width + tab, 55 + tab, extra_box_width - 2 * tab, 25 }), "Load Level\nSave Level\nDisplay Graph\nDisplay Image", menu_options);
        if (menu_options == 2) {
            rimg = LoadImage("C://dev//levelgenerator//assets//pacmap001.png");
            copy_graph_to_raylib(&rimg, graf);
            ImageResizeNN(&rimg, width, height);
            txtr = LoadTextureFromImage(rimg);
            menu_options = -2;
        }

        if (menu_options == -2) {
           // GuiPanel(Rectangle({ (float)width + tab, 200, extra_box_width - 2 * tab, 160}), "White = Node\nBlack = Wall\nPurple = Multi Directional\nBlue = SRC\nRed = DST\nGreen = SEARCH");
            //GuiLabel(Rectangle({ (float)width + tab, 200, extra_box_width - 2 * tab, 160 }), "White = Node\nBlack = Wall\nPurple = Multi Directional\nBlue = SRC\nRed = DST\nGreen = SEARCH");
            GuiPanel(Rectangle({ (float)width + tab, panel_height, extra_box_width - 2 * tab, 160 }), "KEY:");
            DrawText("Node", width + 2 * tab, panel_height + 20 + tab, 20, LIGHTGRAY);
            DrawText("Wall", width + 2 * tab, panel_height + 40 + tab, 20, BLACK);
            DrawText("Multi Dir", width + 2 * tab, panel_height + 60 + tab, 20, PURPLE);
            
            if(graf.src.IsValid())
                DrawText(TextFormat("Sorce: [ %.0f, %.0f ]", (float)graf.src.x, (float)graf.src.y), width + 2 * tab, panel_height + 80 + tab, 20, GREEN);
            else
                DrawText("Sorce", width + 2 * tab, panel_height + 80 + tab, 20, GREEN);
            
            if(graf.dst.IsValid())
                DrawText(TextFormat("Sorce: [ %.0f, %.0f ]", (float)graf.dst.x, (float)graf.dst.y), width + 2 * tab, panel_height + 100 + tab, 20, RED);
            else
                DrawText("Dest", width + 2 * tab, panel_height + 100 + tab, 20, RED);
            DrawText("Path", width + 2 * tab, panel_height + 120 + tab, 20, BLUE);

            //GuiLabelButton(Rectangle({ width + 2 * tab, panel_height + 150, 20, 50 }), "Set Source");
            graph_options = GuiToggleGroup(Rectangle({ width +  tab, panel_height + 165, extra_box_width - 2 * tab, 25 }), "Set Source\nSet Destination\nDrawPath", graph_options);
            if (graph_options == 2) {
                search_options = GuiToggleGroup(Rectangle({ width + tab, panel_height + 250 , extra_box_width - 2 * tab, 40 }), "DFS\nBFS\nA*", search_options);
                if (search_options == 0) {
                    search_options = -1;
                    ResetGraphPath(graf);
                    auto path = graf.FindTarget();
                    rimg = LoadImage("C://dev//levelgenerator//assets//pacmap001.png");
                    copy_graph_to_raylib(&rimg, graf);
                    ImageResizeNN(&rimg, width, height);
                    txtr = LoadTextureFromImage(rimg);
                }
            }
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (mousePosition.x < width) {
                    if (graph_options == 0) {
                        SetGraphSrc(graf, Mouse_To_Img_Pos(scale, mousePosition));
                        rimg = LoadImage("C://dev//levelgenerator//assets//pacmap001.png");
                        copy_graph_to_raylib(&rimg, graf);
                        ImageResizeNN(&rimg, width, height);
                        txtr = LoadTextureFromImage(rimg);
                    }
                    if (graph_options == 1) {
                        SetGraphDst(graf, Mouse_To_Img_Pos(scale, mousePosition));
                        rimg = LoadImage("C://dev//levelgenerator//assets//pacmap001.png");
                        copy_graph_to_raylib(&rimg, graf);
                        ImageResizeNN(&rimg, width, height);
                        txtr = LoadTextureFromImage(rimg);
                    }
                }
            }
        }

        if (menu_options == 3) {
            rimg = LoadImage("C://dev//levelgenerator//assets//pacmap001.png");
            copy_lvlgen_to_raylib(&rimg, img);
            ImageResizeNN(&rimg, width, height);
            txtr = LoadTextureFromImage(rimg);

            menu_options = -3;
        }

        if (menu_options == 0 || menu_options == 1) { menu_options = -1; }
        EndDrawing();
	}

	CloseWindow();
	
}