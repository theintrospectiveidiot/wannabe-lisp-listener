#include <raylib.h>

#define MAX_INPUT_CHARS     50
#define MAX_ROWS            10

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 500;

    InitWindow(screenWidth, screenHeight, "raylib [text] example - input box");

    char name[MAX_ROWS][MAX_INPUT_CHARS + 1] = {0};
    // NOTE: One extra space required for null terminator char '\0'
    
    /*for (int i = 0; i < MAX_ROWS; i++) {
        name[i][MAX_INPUT_CHARS + 1] = '\0';
    }*/

    int line_data[MAX_ROWS] = {0};
    int letterCount[MAX_ROWS] = {0};
    int rowCount = 0;

    Rectangle textBox = { 2, 40, 796, 380 };
    bool mouseOnText = false;

    int framesCounter = 0;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
        else mouseOnText = false;

        if (mouseOnText)
        {
            // Set the window's cursor to the I-Beam
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            // Get char pressed (unicode character) on the queue
            int key = GetCharPressed();

            // Check if more characters have been pressed on the same frame
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

        if (mouseOnText) framesCounter++;
        else framesCounter = 0;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("PLACE MOUSE OVER INPUT BOX!", 2, 5, 20, GRAY);

            DrawRectangleRec(textBox, LIGHTGRAY);
            if (mouseOnText) DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
            else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);

            for (int i = 0; i <= MAX_ROWS; i++) {
                
                DrawText(name[i], (int)textBox.x + 5, (int)textBox.y + 8 + (37 * i), 35, MAROON);
            }


            DrawText(TextFormat("INPUT CHARS: %i/%i", letterCount[rowCount], MAX_INPUT_CHARS), 2, 425, 20, DARKGRAY);

            if (mouseOnText)
            {
                if ((letterCount[rowCount] < MAX_INPUT_CHARS) && (rowCount < MAX_ROWS))
                {
                    // Draw blinking underscore char
                    if (((framesCounter/20)%2) == 0) DrawText("_", (int)textBox.x + 8 + MeasureText(name[rowCount], 35), (int)textBox.y + 12 + (37 * rowCount), 35, MAROON);
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
