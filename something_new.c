#include <raylib.h>
#include <stdio.h>
#include <string.h>

#define INPUT_CHARS_CAPACITY 1000
#define ROWS_CAPACITY       8000

// have hardcoded a long capacity, hoping that we dont need to worry about what happens beyond that.
// cuz the main aim of this isnt a good text editor, its a basic text editor with hopefully something cool!

#define MAX_INPUT_CHARS     79
#define MAX_ROWS            17

#define TEXT_SIZE           20
#define LINE_GAP            (TEXT_SIZE + 2)
#define TEXT_SPACING        0.00f

#define MY_COL_ONE          CLITERAL(Color){ 201, 195, 131, 245 }
#define MY_COL_TWO          CLITERAL(Color){ 190, 185, 161, 145 }

int mult_fact;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth  = 806;
    const int screenHeight = 500;
    
    mult_fact = 1;
    InitWindow(screenWidth, screenHeight, "cheap text editor fow now");
    Font pref = LoadFontEx("Fonts/Terminus.ttf", (mult_fact * TEXT_SIZE), NULL, 0);
    //printf("baseSize = %d\n", pref.baseSize);
    //printf("glyphCount = %d\n", pref.glyphCount);
    //printf("texture.id = %u\n", pref.texture.id);
    char name[ROWS_CAPACITY][INPUT_CHARS_CAPACITY] = {0};
    // NOTE: One extra space required for null terminator char '\0'
    
    /*for (int i = 0; i < MAX_ROWS; i++) {
        name[i][MAX_INPUT_CHARS + 1] = '\0';
    }*/
    
    int lb_rows  = 0;
    int lb_chars = 0;
    
    int cur_pos[MAX_ROWS]     = {0};
    int letterCount[MAX_ROWS] = {0};
    int rowCount              =  0;
    
    int mode = 0; 
    // 0 when the cur_pos and letterCount + 1 are same.
    // 1 when the cur_pos and letterCount + 1 are NOT same.

    Rectangle textBox = { 0, 40, 806, 390 };
    bool mouseOnText = true;

    int framesCounter = 0;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        
        if (mode == 0) {
            cur_pos[rowCount] = letterCount[rowCount];
        }

        if (mouseOnText)
        {
            // Set the window's cursor to the I-Beam
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            // Get char pressed (unicode character) on the queue
            int key = GetCharPressed();

            while (key > 0)
            {
                // NOTE: Only allow keys in range [32..125]
                if (((key >= 32) && (key <= 125)) && (letterCount[rowCount] < INPUT_CHARS_CAPACITY))
                {
                    //at the last position
                    if (mode == 0)
                    {
                        name[rowCount][letterCount[rowCount]] = (char) key;
                        name[rowCount][letterCount[rowCount] + 1] = '\0'; // Add null terminator at the end of the string
                        letterCount[rowCount]++;
                    }
                    
                    //letterCount[rowCount] is always at the null terminator. 

                    //not at the last position
                    else if (mode == 1) {
                        char temp[letterCount[rowCount] + 1 - cur_pos[rowCount]];
                        memcpy(temp, name[rowCount] + cur_pos[rowCount], letterCount[rowCount] + 1 - cur_pos[rowCount]);
                        name[rowCount][cur_pos[rowCount]] = (char) key;
                        cur_pos[rowCount]++;   
                        //abcd0
                        //01234
                        memcpy(name[rowCount] + cur_pos[rowCount], temp, letterCount[rowCount] + 2 - cur_pos[rowCount]);
                        letterCount[rowCount]++;
                    }
                }

                key = GetCharPressed();  // Check next character in the queue
            }
            
            if (IsKeyPressed(KEY_BACKSPACE))
            {
                
                if (mode == 0) {
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
                
                else if (mode == 1) {
                    if (cur_pos[rowCount] > 0) {
                        char temp[letterCount[rowCount] + 1 - cur_pos[rowCount]];
                        memcpy(temp, name[rowCount] + cur_pos[rowCount], letterCount[rowCount] + 1 - cur_pos[rowCount]);
                        cur_pos[rowCount]--;   
                        //abd0
                        //0123
                        memcpy(name[rowCount] + cur_pos[rowCount], temp, letterCount[rowCount] - cur_pos[rowCount]);
                        letterCount[rowCount]--;

                    }

                    else if (cur_pos[rowCount] == 0) {
                        if (rowCount > 0) {
                            char temp[letterCount[rowCount] + 1 - cur_pos[rowCount]];
                            memcpy(temp, name[rowCount] + cur_pos[rowCount], letterCount[rowCount] + 1 - cur_pos[rowCount]);
                            name[rowCount][cur_pos[rowCount]] = '\0';
                        
                            //fprintf(stderr, "[|%s|\n|%s|\n", name[rowCount - 1], temp);
                            rowCount--;
                            //abd0
                            //0123
                            memcpy(name[rowCount] + letterCount[rowCount], temp, sizeof(temp));
                            //fprintf(stderr, "|%c|\n", name[rowCount][letterCount[rowCount]]);
                            cur_pos[rowCount] = letterCount[rowCount];
                            letterCount[rowCount] += sizeof(temp) - 1;
                            //fprintf(stderr, "|%s|\ncur_pos = %d\nletterCount = %d]\n", name[rowCount], cur_pos[rowCount], letterCount[rowCount]);
                        }

                        else if (rowCount <= 0) {
                            rowCount = 0;
                        }
                
                    }
        
                }

            }
            
            if (IsKeyPressed(KEY_UP)) {
                if (rowCount > 0) {
                    if (letterCount[rowCount] < letterCount[rowCount - 1]) {
                        cur_pos[rowCount - 1] = cur_pos[rowCount];
                        rowCount -= 1;
                        mode = 1;
                    }
                    else if (letterCount[rowCount] >= letterCount[rowCount + 1]) {
                        rowCount--;
                        mode = 0;
                    }
                }
                else if (rowCount == 0) {
                    rowCount = 0;
                }
            }

            if (IsKeyPressed(KEY_DOWN)) {
                if (rowCount < MAX_ROWS) {
                    if (letterCount[rowCount] > letterCount[rowCount + 1]) {
                        cur_pos[rowCount + 1] = cur_pos[rowCount];
                        rowCount += 1;
                        mode = 1;
                    }
                    else if (letterCount[rowCount] <= letterCount[rowCount + 1]) {
                        rowCount += 1;
                        mode = 0;
                    }
                }
                else if (rowCount == MAX_ROWS) {
                    rowCount = MAX_ROWS;
                }
            }

            if (IsKeyPressed(KEY_LEFT)) {
                //fprintf(stderr, "Bonjour! je suis LEFT!\n");
                
                if (mode == 0) {
                    
                    //fprintf(stderr, "Bonjour! je suis LEFT et mode est 0!\n");

                    if (letterCount[rowCount] <= 0) {
                        cur_pos[rowCount] = 0;
                    }
                    else {
                        cur_pos[rowCount] = letterCount[rowCount] - 1;
                        
                        //fprintf(stderr, "Bonjour! je suis LEFT et mode est 0! est cur_pos has been updated to %d\n", cur_pos[rowCount]);   
                        mode = 1;
                    }
                }
            

                else if (mode == 1) {
                    if (cur_pos[rowCount] <= 0) {
                        cur_pos[rowCount] = 0;
                    }
                    
                    else {
                        cur_pos[rowCount]--;
                    }
                }
            }

            if (IsKeyPressed(KEY_RIGHT)) {
                if (mode == 0) {
                    //area we havent explored yet
                    cur_pos[rowCount] = letterCount[rowCount];
                }

                else if (mode == 1) {
                    cur_pos[rowCount]++;
                    
                    if (letterCount[rowCount] == cur_pos[rowCount]) {
                        mode = 0;
                    }


                }
            }

            if (IsKeyPressed(KEY_ENTER))
            {
                if (mode == 0) {
                    rowCount++;
                    if (rowCount >= MAX_ROWS) {
                        lb_rows = rowCount + 1 - MAX_ROWS;
                    }
                }

                else if (mode == 1) 
                {    
                    
                    char temp[letterCount[rowCount] + 1 - cur_pos[rowCount]];
                    memcpy(temp, name[rowCount] + cur_pos[rowCount], letterCount[rowCount] + 1 - cur_pos[rowCount]);
                    name[rowCount][cur_pos[rowCount]] = '\0';
                    letterCount[rowCount] = cur_pos[rowCount];
                    //fprintf(stderr, "|%s|\n|%s|\n", name[rowCount], temp); 
                    rowCount++;
                    if (rowCount >= MAX_ROWS) {
                        lb_rows = rowCount + 1 - MAX_ROWS;
                    }
                    //abcd0
                    //0123
                    if (letterCount[rowCount] > 0) {
                       char temp2[letterCount[rowCount]];
                       memcpy(temp2, name[rowCount], letterCount[rowCount] + 1);
                       memcpy(name[rowCount], temp, sizeof(temp));
                       memcpy(name[rowCount] + sizeof(temp) - 1, temp2, sizeof(temp2));
                       letterCount[rowCount] += sizeof(temp) - 1;
                       cur_pos[rowCount] = 0;
                    }

                    else if (letterCount[rowCount] == 0) {
                         memcpy(name[rowCount], temp, sizeof(temp));
                         letterCount[rowCount] += sizeof(temp) - 1;
                         cur_pos[rowCount] = 0;
                    }

                   
                }
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

            DrawText("DO ANYTHING YOU WANT!", 2, 5, 20, GRAY);

            DrawRectangleRec(textBox, MY_COL_ONE);
            if (mouseOnText) DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
            else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);

            for (int i = lb_rows; i <= lb_rows + MAX_ROWS; i++) {
                
                DrawTextEx(pref, name[i] + lb_chars, (Vector2) {(int)textBox.x + 5, (int)textBox.y + 8 + (LINE_GAP * (i - lb_rows))}, (mult_fact * TEXT_SIZE), TEXT_SPACING, BLACK);
            }


            DrawText(TextFormat("CURSOR POSITION: (%d, %d)", rowCount, cur_pos[rowCount]), 2, 450, 20, DARKGRAY);

            if (mouseOnText)
            {
                if ((letterCount[rowCount] < INPUT_CHARS_CAPACITY) && (rowCount < ROWS_CAPACITY))
                {
                    // Draw blinking underscore char
                    float cursorWidth = (MeasureTextEx(pref, name[rowCount], (mult_fact * TEXT_SIZE), TEXT_SPACING).x) / letterCount[rowCount];
                    //fprintf(stdout, "%f\n", cursorWidth);
                    float x = MeasureTextEx(pref, name[rowCount], (mult_fact * TEXT_SIZE), TEXT_SPACING).x;
                    if (((framesCounter / 20) % 2) == 0) DrawTextEx(pref, "_", (Vector2) {(int)textBox.x + 5 + ((float)(mult_fact * TEXT_SIZE) / 2) * ((mode == 0) ? letterCount[rowCount]:cur_pos[rowCount]), (int)textBox.y + 12 + (LINE_GAP * ((rowCount >= MAX_ROWS) ? (rowCount - lb_rows) : rowCount))}, (mult_fact * TEXT_SIZE), TEXT_SPACING, BLACK);
                }
                //else DrawText("Press BACKSPACE to delete chars...", 2, 460, 20, GRAY);
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
