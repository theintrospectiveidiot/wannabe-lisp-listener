#include <raylib.h>
#include <stdio.h>

#define MAX_INPUT_CHARS     100
#define MAX_ROWS            10

#define TEXT_SIZE           20
#define LINE_GAP            (TEXT_SIZE + 2)
#define TEXT_SPACING        0.00f

#define MY_COL_ONE          CLITERAL(Color){ 201, 195, 131, 245 }
#define MY_COL_TWO          CLITERAL(Color){ 190, 185, 161, 145 }

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 500;
    
    //Font pref = LoadFontEx("");
    InitWindow(screenWidth, screenHeight, "raylib [text] example - input box");
    Font pref = LoadFontEx("all-stuff2/Terminus.ttf", TEXT_SIZE, NULL, 0);
    //printf("baseSize = %d\n", pref.baseSize);
    //printf("glyphCount = %d\n", pref.glyphCount);
    //printf("texture.id = %u\n", pref.texture.id);
    char name[MAX_ROWS][MAX_INPUT_CHARS] = {0};
    // NOTE: One extra space required for null terminator char '\0'
    
    /*for (int i = 0; i < MAX_ROWS; i++) {
        name[i][MAX_INPUT_CHARS + 1] = '\0';
    }*/

    int line_data[MAX_ROWS] = {0};
    int letterCount[MAX_ROWS] = {0};
    int rowCount = 0;

    Rectangle textBox = { 2, 40, 796, 380 };
    bool mouseOnText = true;

    int framesCounter = 0;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        if (mouseOnText)
        {
            // Set the window's cursor to the I-Beam
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            // Get char pressed (unicode character) on the queue
            int key = GetCharPressed();

            while (key > 0)
            {
                // NOTE: Only allow keys in range [32..125]
                if ((((key >= 32) && (key <= 125)) || (key == 10) || (key == 9)) && (letterCount[rowCount] < MAX_INPUT_CHARS))
                {
                    name[rowCount][letterCount[rowCount]] = (char)key;
                    name[rowCount][letterCount[rowCount] + 1] = '\0'; // Add null terminator at the end of the string
                    letterCount[rowCount]++;
                }

                key = GetCharPressed();  // Check next character in the queue
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                if (letterCount[rowCount] > 0) {
                    letterCount[rowCount]--;
                }

                
                else if (letterCount[rowCount] == 0) {
                    
                    if (rowCount > 0) {
                    rowCount -= 1;
                    }

                    else if (rowCount <= 0) {
                        rowCount = 0;
                    }
                }
                
                name[rowCount][letterCount[rowCount]] = '\0';
            }

            if (IsKeyPressed(KEY_ENTER))
            {
                rowCount++;
            }
        }
        else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        framesCounter++;
        //else framesCounter = 0;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(MY_COL_TWO);

            DrawText("PLACE MOUSE OVER INPUT BOX!", 2, 5, 20, GRAY);

            DrawRectangleRec(textBox, MY_COL_ONE);
            if (mouseOnText) DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
            else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);

            for (int i = 0; i <= MAX_ROWS; i++) {
                
                DrawTextEx(pref, name[i], (Vector2) {(int)textBox.x + 5, (int)textBox.y + 8 + (LINE_GAP * i)}, TEXT_SIZE, TEXT_SPACING, BLACK);
            }


            DrawText(TextFormat("INPUT CHARS: %i/%i", letterCount[rowCount], MAX_INPUT_CHARS), 2, 425, 20, DARKGRAY);

            if (mouseOnText)
            {
                if ((letterCount[rowCount] < MAX_INPUT_CHARS) && (rowCount < MAX_ROWS))
                {
                    // Draw blinking underscore char
                    float cursorWidth = (MeasureTextEx(pref, name[rowCount], TEXT_SIZE, TEXT_SPACING).x) / letterCount[rowCount];
                    //fprintf(stdout, "%f\n", cursorWidth);
                    float x = MeasureTextEx(pref, name[rowCount], TEXT_SIZE, TEXT_SPACING).x;
                    if (((framesCounter / 20) % 2) == 0) DrawTextEx(pref, "_", (Vector2) {(int)textBox.x + 5 + ((float)TEXT_SIZE / 2) * (letterCount[rowCount]), (int)textBox.y + 12 + (LINE_GAP * rowCount)}, TEXT_SIZE, TEXT_SPACING, BLACK);
                }
                else DrawText("Press BACKSPACE to delete chars...", 2, 440, 20, GRAY);
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// Check if any key is pressed
// NOTE: We limit keys check to keys between 32 (KEY_SPACE) and 126
bool IsAnyKeyPressed()
{
    bool keyPressed = false;
    int key = GetKeyPressed();

    if ((key >= 32) && (key <= 126)) keyPressed = true;

    return keyPressed;
}
