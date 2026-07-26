#include <raylib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define INPUT_CHARS_CAPACITY    1000
#define ROWS_CAPACITY           8000

// have hardcoded a long capacity, hoping that we dont need to worry about what happens beyond that.
// cuz the main aim of this isnt a good text editor, its a basic text editor with hopefully something cool!

#define MAX_INPUT_CHARS         79
#define MAX_ROWS                17

#define TEXT_SIZE               20
#define LINE_GAP                (TEXT_SIZE + 2)
#define TEXT_SPACING            0.00f

#define MY_COL_ONE              CLITERAL(Color){ 201, 195, 131, 245 }
#define MY_COL_TWO              CLITERAL(Color){ 190, 185, 161, 145 }

// ADD 2 BUTTONS, ONE FOR CLEARING THE PAGE, AND ONE FOR SAVING IT. THINK ABOUT HOW TO IMPLEMENT AND ALL. AND DONT FORGET THE STYLE

int lb_chars;
int lb_rows;

int row_count;

typedef struct Line Line;

Line *top;
Line *cur_line;

int mode;
struct Line {
    char *line_data;
    
    int char_count;                                //letterCount basically
    int cursor_pos;                                //cur_pos, duh!

    int random;
    Line *prev;
    Line *next;
}; 

Line *make_line() {
    Line *new_line = malloc (sizeof (Line));

    new_line->line_data = malloc(sizeof(char) * (INPUT_CHARS_CAPACITY + 1));
     
    srand(time(0));
    new_line->random = rand();
    new_line->char_count = 0;
    new_line->cursor_pos = 0;

    new_line->line_data[new_line->char_count] = '\0';
    
    new_line->prev = NULL;
    new_line->next = NULL;

    return new_line;
}

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
    Font pref   = LoadFontEx("Fonts/Terminus.ttf", (mult_fact * TEXT_SIZE), NULL, 0);
    Font pref_b = LoadFontEx("Fonts/TerminusTTF-Bold-4.49.3.ttf", (mult_fact * TEXT_SIZE), NULL, 0);
    //printf("baseSize = %d\n", pref.baseSize);
    //printf("glyphCount = %d\n", pref.glyphCount);
    //printf("texture.id = %u\n", pref.texture.id);
    
    lb_rows  = 0;
    lb_chars = 0;
    

    row_count   =  0;
    top       = make_line();
    cur_line  = top;

    mode = 0; 
    // 0 when the cur_pos and letterCount + 1 are same.
    // 1 when the cur_pos and letterCount + 1 are NOT same.

    Rectangle typing_window = { 0, 40, 806, 390 };
    // 390 = 8 (the top empty part, for it to look good) + 8 (bottom empty part) + 17 * (TEXT_SIZE + LINE_GAP)

    bool mouseOnText = true;

    int framesCounter = 0;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        
        if (mode == 0) {
            cur_line->cursor_pos = cur_line->char_count;

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
                if (((key >= 32) && (key <= 126)) && (cur_line->char_count < INPUT_CHARS_CAPACITY))
                {
                    //at the last position
                    if (mode == 0)
                    {
                        if ((cur_line->cursor_pos - lb_chars) >= MAX_INPUT_CHARS) {
                            lb_chars = cur_line->cursor_pos - MAX_INPUT_CHARS + 1;   //single character, so we can do this
                        }
                        cur_line->line_data[cur_line->char_count] = (char) key;
                        cur_line->line_data[(cur_line->char_count) + 1] = '\0';
                        cur_line->char_count += 1;
                    }
                    

                    //not at the last position
                    else if (mode == 1) {
                        if ((cur_line->cursor_pos - lb_chars) == MAX_INPUT_CHARS) {
                            lb_chars = cur_line->cursor_pos - MAX_INPUT_CHARS + 1;
                        }
                        char *temp = malloc(sizeof(char) * (cur_line->char_count + 1 - cur_line->cursor_pos));
                        memcpy(temp, cur_line->line_data + cur_line->cursor_pos, cur_line->char_count + 1 - cur_line->cursor_pos);
                        cur_line->line_data[cur_line->cursor_pos] = (char) key;
                        cur_line->cursor_pos += 1;
                        cur_line->char_count += 1;

                        memcpy(cur_line->line_data + cur_line->cursor_pos, temp, cur_line->char_count + 1 - cur_line->cursor_pos);
                        free(temp);
                    }
                }

                key = GetCharPressed();  // Check next character in the queue
            }
            
            if (IsKeyPressed(KEY_BACKSPACE))
            { 
                if (mode == 0) {
                    
               
                    if (cur_line->char_count > 0) {
                        if ((lb_chars > 0) && (cur_line->cursor_pos - lb_chars == 0)) {
                            lb_chars -= 1;
                        }
                        
                        cur_line->char_count -= 1;
                        cur_line->line_data[cur_line->char_count] = '\0';
                    }

                
                    else if (cur_line->char_count == 0) {
                       
                        if (row_count > 0) {
                           
                            if (cur_line->prev->char_count >= MAX_INPUT_CHARS) {
                                lb_chars = cur_line->prev->char_count - MAX_INPUT_CHARS + 1;
                            }

                            if ((lb_rows > 0) && (row_count - lb_rows == 0)) {
                                lb_rows -= 1;
                                top = top->prev;
                            }
                            row_count -= 1;
                            if (cur_line->next != NULL) {
                                cur_line->next->prev = cur_line->prev;
                                cur_line->prev->next = cur_line->next;
                            }

                            else if (cur_line->next == NULL) {
                                cur_line->prev->next = cur_line->next;
                            }   
                            cur_line = cur_line->prev;
                        }
                        else if (row_count <= 0) {
                            row_count = 0;
                        }
                    }
                }
                
                else if (mode == 1) {
                    if (cur_line->cursor_pos > 0) {

                        if (cur_line->cursor_pos - lb_chars == 0) {
                            lb_chars -= 1;
                        }

                        char *temp = malloc (sizeof(char) * (cur_line->char_count + 1 - cur_line->cursor_pos));
                        memcpy(temp, cur_line->line_data + cur_line->cursor_pos, cur_line->char_count + 1 - cur_line->cursor_pos);
                        cur_line->cursor_pos -= 1;
                        cur_line->char_count -= 1;

                        memcpy(cur_line->line_data + cur_line->cursor_pos, temp, cur_line->char_count + 1 - cur_line->cursor_pos);
                        cur_line->line_data[cur_line->char_count] = '\0';           //maybe we dont need it

                        free(temp);
                    }

                    else if (cur_line->cursor_pos == 0) {
                        if (row_count > 0) {
                            
                            if (row_count - lb_rows == 0) {
                                lb_rows -= 1; 
                                top = top->prev;
                            }

                            char *temp = malloc (sizeof(char) * (cur_line->char_count + 1));
                            int temp_size = (cur_line->char_count + 1);
                            memcpy(temp, cur_line->line_data, cur_line->char_count + 1);                             
                            
                            if (cur_line->next != NULL) {
                                cur_line->prev->next = cur_line->next;
                                cur_line->next->prev = cur_line->prev;
                            }

                            else if (cur_line->next == NULL) {
                                cur_line->prev->next = cur_line->next;
                            }

                            cur_line = cur_line->prev;
                            row_count -= 1;
                            
                            memcpy(cur_line->line_data + cur_line->char_count, temp, temp_size);
                            cur_line->cursor_pos = cur_line->char_count;
                            cur_line->char_count += temp_size - 1;
                            
                            if (cur_line->cursor_pos > MAX_INPUT_CHARS) {
                                lb_chars = cur_line->cursor_pos - MAX_INPUT_CHARS;
                            }

                            free(temp);
                        }

                        else if (row_count <= 0) {
                            row_count = 0;
                        }
                
                    }
        
                }

            }
            
            if (IsKeyDown(KEY_UP)) {
                if (row_count > 0) {
                    if ((lb_rows > 0) && ((row_count - lb_rows) == 0)) {
                        top = top->prev;
                        lb_rows -= 1;
                    }
                    if (cur_line->cursor_pos < cur_line->prev->char_count) {
                        cur_line->prev->cursor_pos = cur_line->cursor_pos;
                        cur_line = cur_line->prev;
                        row_count -= 1;
                        mode = 1;
                    }
                    else if (cur_line->cursor_pos >= cur_line->prev->char_count) {
                        
                        if (((cur_line->prev->char_count - lb_chars) < 0) && (lb_chars > 0)) {
                            lb_chars = ((cur_line->prev->char_count >= MAX_INPUT_CHARS) ? (cur_line->prev->char_count - MAX_INPUT_CHARS + 1) : 0);    // if that is longer than what we can display, then show the last one, else just come back to original.
                        }

                        cur_line = cur_line->prev;
                        row_count--;
                        mode = 0;
                         
                        if (cur_line->char_count >= MAX_INPUT_CHARS) {
                            lb_chars = cur_line->char_count - MAX_INPUT_CHARS + 1;
                        }
                        
                    }
                }
                else if (row_count == 0) {
                    row_count = 0;
                }
            }

            if (IsKeyPressed(KEY_DOWN)) {
                
                if (cur_line->next != NULL) {
                    if ((row_count + 1 - lb_rows) >= MAX_ROWS) {
                        top = top->next;
                        lb_rows += 1;
                    }

                    if (cur_line->cursor_pos < cur_line->next->char_count) {
                        cur_line->next->cursor_pos = cur_line->cursor_pos;
                        cur_line = cur_line->next;
                        row_count += 1;
                        mode = 1;
                    }
                    
                    else if (cur_line->cursor_pos >= cur_line->next->char_count) {
                        cur_line = cur_line->next;
                        row_count += 1;
                        mode = 0;
                        
                        // by less than 0, what we mean is that the slider is longer than the line, i.e. your lb_chars has overpowered the line and now, u need to go back.
                        if ((cur_line->char_count - lb_chars < 0) && (lb_chars > 0)) {
                            lb_chars = ((cur_line->char_count > MAX_INPUT_CHARS) ? (cur_line->char_count - MAX_INPUT_CHARS + 1) : 0);
                        }
                    }
                }
             

                else if (cur_line->next == NULL) {
                    //do nothing
                    
                }
            }

            if (IsKeyPressed(KEY_LEFT)) {
   
                if ((lb_chars > 0) && (cur_line->cursor_pos - lb_chars == 0)) {
                    lb_chars -= 1;
                }

                if (mode == 0) {
    
                    if (cur_line->cursor_pos == 0) {
                        cur_line->cursor_pos = 0;
                    }
                    
                    else {
                        cur_line->cursor_pos = cur_line->char_count - 1;
                        mode = 1;
                    }
                }
            

                else if (mode == 1) {
                    if (cur_line->cursor_pos == 0) {
                        cur_line->cursor_pos = 0;
                    }
                    
                    else {
                        cur_line->cursor_pos -= 1;
                    }
                }
            }

            if (IsKeyPressed(KEY_RIGHT)) {
                if (mode == 0) {
                    //area we havent explored yet
                    cur_line->cursor_pos = cur_line->char_count;
                }

                else if (mode == 1) {
                    if ((cur_line->cursor_pos - lb_chars >= MAX_INPUT_CHARS)) {
                        lb_chars = cur_line->cursor_pos + 1 - MAX_INPUT_CHARS;    
                    }

                    cur_line->cursor_pos += 1;
                    
                    if (cur_line->cursor_pos == cur_line->char_count) {
                        mode = 0;
                    }
                    
                }
            }

            if (IsKeyPressed(KEY_ENTER))
            {
                lb_chars = 0; //u hit enter and then start writing from the beginning, regardless of where u were in prev line
                
                if (mode == 0) {


                    row_count++;
                    Line *new_line = make_line();
                    if (cur_line->next != NULL) { 
                        cur_line->next->prev = new_line;
                        new_line->prev = cur_line;
                        new_line->next = cur_line->next;
                        cur_line->next = new_line;
                        cur_line = cur_line->next;
                    }

                    else if (cur_line->next == NULL) {
                        new_line->prev = cur_line;
                        cur_line->next = new_line;
                        cur_line = cur_line->next;
                    }

                    if (row_count >= MAX_ROWS) {
                        lb_rows += 1;
                        top = top->next;
                    }
                }

                else if (mode == 1) 
                {    
                    char *temp = malloc (sizeof(char) * (cur_line->char_count + 1 - cur_line->cursor_pos));
                    memcpy(temp, cur_line->line_data + cur_line->cursor_pos, (cur_line->char_count + 1 - cur_line->cursor_pos));
                    int temp_size = (cur_line->char_count + 1 - cur_line->cursor_pos);
                    cur_line->line_data[cur_line->cursor_pos] = '\0';
                    cur_line->char_count = cur_line->cursor_pos;

                    row_count += 1;

                    Line *new_line = make_line();
                    if (cur_line->next != NULL) {
                        cur_line->next->prev = new_line;
                        new_line->next = cur_line->next;
                        new_line->prev = cur_line;
                        cur_line->next = new_line;
                        cur_line = cur_line->next;
                    }

                    else if (cur_line->next == NULL) {
                        cur_line->next = new_line;
                        new_line->prev = cur_line;
                        cur_line = cur_line->next;
                    }
                    
                    memcpy(cur_line->line_data, temp, temp_size);
                    cur_line->char_count = temp_size - 1;
                    cur_line->cursor_pos = 0;

                    free(temp);

                    if (row_count >= MAX_ROWS) {
                        lb_rows += 1;
                        top = top->next;
                    }
                    //abc4
                    //
                    //0123
                }
            }
        }
        else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        framesCounter++;
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(MY_COL_TWO);

            DrawText("DO ANYTHING YOU WANT!", 2, 5, 20, DARKGRAY);

            DrawRectangleRec(typing_window, MY_COL_ONE);
            if (mouseOnText) DrawRectangleLines((int)typing_window.x + 1, (int)typing_window.y, (int)typing_window.width - 1, (int)typing_window.height, RED);
            else DrawRectangleLines((int)typing_window.x, (int)typing_window.y, (int)typing_window.width, (int)typing_window.height, DARKGRAY);

            
            Line *temp = top;

            for (int i = lb_rows; i < (lb_rows + MAX_ROWS) && (temp != NULL); i++) {
                
                DrawTextEx(pref, ((temp->char_count > lb_chars) ? (temp->line_data + lb_chars) : '\0'), (Vector2) {(int)typing_window.x + 5, (int)typing_window.y + 8 + (LINE_GAP * (i - lb_rows))}, (mult_fact * TEXT_SIZE), TEXT_SPACING, BLACK);
                
                temp = temp->next;
            }


            DrawText(TextFormat("CURSOR POSITION: (%d, %d)", row_count, cur_line->cursor_pos), 2, 450, 20, DARKGRAY);

            if (mouseOnText)
            {
                if ((cur_line->char_count < INPUT_CHARS_CAPACITY) && (row_count < ROWS_CAPACITY))
                {
                    // Draw blinking underscore char
                  
                    if (((framesCounter / 20) % 2) == 0) DrawTextEx(pref, "_", (Vector2) {(int)typing_window.x + 5 + ((float)(mult_fact * TEXT_SIZE) / 2) * ((mode == 0) ? ((lb_chars > 0) ? (cur_line->char_count - lb_chars) : cur_line->char_count) : ((lb_chars > 0) ? (cur_line->cursor_pos - lb_chars) : cur_line->cursor_pos)), (int)typing_window.y + 12 + (LINE_GAP * ((lb_rows > 0) ? (row_count - lb_rows) : row_count))}, (mult_fact * TEXT_SIZE), TEXT_SPACING, BLACK);

                }
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
