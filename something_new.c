#include <raylib.h>
#include <stdio.h>
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

typedef struct Line Line;

struct Line {
    char *line_data;
    
    int char_count;                                //letterCount basically
    int index;                                     //rowCount basically
    int cursor_pos;                                //cur_pos, duh!

    Line *prev;
    Line *next;
}; 

Line *make_line(int index) {
    Line *new_line = malloc (sizeof (Line));

    new_line->line_data = malloc(sizeof(char) * (INPUT_CHARS_CAPACITY + 1));
    
    new_line->index = index;
    new_line->char_count = 0;
    new_line->cursor_pos = 0;
    
    new_line->prev = NULL;
    new_line->next = NULL;

    return new_line;
}

void on_stage(Line performers[], Line *lead) {
    
    performers[0] = lead;
    for (int i = 1, i < MAX_ROWS, ++i) {
        
        if (performers[i - 1]->next != NULL) {
            perfomers[i] = performers[i - 1]->next;
        }
        else performers[i] = NULL;
    }
}

Line *starting_line;

Line *search_by_index(int val, Line *lead) {
    
    if (lead == NULL) {
        return lead;
    }

    if (lead->index == val) {
        return temp;
    } 
    else if (lead->next != NULL) {
        search_by_index(val, temp->next);
    }
    else if (lead->next == NULL) {
        return lead;
    }
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
    
    int cur_pos[ROWS_CAPACITY]     = {0};
    int letterCount[ROWS_CAPACITY] = {0};
    int rowCount                   =  0;
    
    starting_line = make_line(0);
    Line *cur_line = starting_line;

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
                        /*name[rowCount][letterCount[rowCount]] = (char) key;
                        name[rowCount][letterCount[rowCount] + 1] = '\0'; // Add null terminator at the end of the string
                        letterCount[rowCount]++;*/

                        cur_line->line_data[cur_line->char_count] = (char) key;
                        cur_line->line_data[(cur_line->char_count) + 1] = '\0';
                        cur_line->char_count += 1;
                    }
                    
                    //letterCount[rowCount] is always at the null terminator. 

                    //not at the last position
                    else if (mode == 1) {
                        /*
                        char temp[letterCount[rowCount] + 1 - cur_pos[rowCount]];
                        memcpy(temp, name[rowCount] + cur_pos[rowCount], letterCount[rowCount] + 1 - cur_pos[rowCount]);
                        name[rowCount][cur_pos[rowCount]] = (char) key;
                        cur_pos[rowCount]++;   
                        //abcd0
                        //01234
                        memcpy(name[rowCount] + cur_pos[rowCount], temp, letterCount[rowCount] + 2 - cur_pos[rowCount]);
                        letterCount[rowCount]++;
                        */

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
                        cur_line->char_count -= 1;
                        cur_line->line_data[cur_line->char_count] = '\0';
                    }

                
                    else if (cur_line->char_count == 0) {
                       
                        if (cur_line->index > 0) {
                            if ((cur_line->index - lb_rows) == 0) {
                                lb_rows -= 1;
                            }
                            
                            cur_line->next->prev = cur_line->prev;
                            cur_line->prev->next = cur_line->next;
                            
                            cur_line = cur_line->prev;
                        }

                        else if (cur_line->index <= 0) {
                            cur_line = cur_line;
                        }
            
                    }
            
                    cur_line->line_data[cur_line->char_count] = '\0';                
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
                        name[rowCount][letterCount[rowCount]] = '\0';
                    }

                    else if (cur_pos[rowCount] == 0) {
                        if (rowCount > 0) {
                            char temp[letterCount[rowCount] + 1 - cur_pos[rowCount]];
                            memcpy(temp, name[rowCount] + cur_pos[rowCount], letterCount[rowCount] + 1 - cur_pos[rowCount]);
                            name[rowCount][cur_pos[rowCount]] = '\0';
                            letterCount[rowCount] = 0; 
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
                    if ((lb_rows > 0) && ((rowCount - lb_rows) == 0)) {
                        fprintf(stderr, "bonjour! lb_rows est %d\n", lb_rows);
                        lb_rows -= 1;
                    }
                    if (letterCount[rowCount] < letterCount[rowCount - 1]) {
                        cur_pos[rowCount - 1] = cur_pos[rowCount];
                        rowCount -= 1;
                        mode = 1;
                    }
                    else if (letterCount[rowCount] >= letterCount[rowCount - 1]) {
                        rowCount--;
                        mode = 0;
                    }
                }
                else if (rowCount == 0) {
                    rowCount = 0;
                }
            }

            if (IsKeyPressed(KEY_DOWN)) {
                if ((rowCount + 1 - lb_rows) < MAX_ROWS) {
                    if (letterCount[rowCount] < letterCount[rowCount + 1]) {
                        cur_pos[rowCount + 1] = cur_pos[rowCount];
                        rowCount += 1;
                        mode = 1;
                    }
                    else if (letterCount[rowCount] >= letterCount[rowCount + 1]) {
                        rowCount += 1;
                        mode = 0;
                    }
                }
                else if ((rowCount + 1 - lb_rows) == MAX_ROWS) {
                    
                    rowCount++;
                    lb_rows++;
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

            DrawText("DO ANYTHING YOU WANT!", 2, 5, 20, BLACK);

            DrawRectangleRec(textBox, MY_COL_ONE);
            if (mouseOnText) DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
            else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);

            Line to_show[MAX_ROWS];
            on_stage(to_show, starting_line);


            for (int i = lb_rows; i < lb_rows + MAX_ROWS; i++) {
                
                DrawTextEx(pref, i->line_data + lb_chars, (Vector2) {(int)textBox.x + 5, (int)textBox.y + 8 + (LINE_GAP * (i - lb_rows))}, (mult_fact * TEXT_SIZE), TEXT_SPACING, BLACK);
            }


            DrawText(TextFormat("CURSOR POSITION: (%d, %d)", cur_line->index, cur_line->cursor_pos), 2, 450, 20, BLACK);

            if (mouseOnText)
            {
                if ((cur_line->char_count < INPUT_CHARS_CAPACITY) && (cur_line->index < ROWS_CAPACITY))
                {
                    // Draw blinking underscore char
                    //float cursorWidth = (MeasureTextEx(pref, name[rowCount], (mult_fact * TEXT_SIZE), TEXT_SPACING).x) / letterCount[rowCount];
                    //fprintf(stdout, "%f\n", cursorWidth);
                    //float x = MeasureTextEx(pref, name[rowCount], (mult_fact * TEXT_SIZE), TEXT_SPACING).x;
                    if (((framesCounter / 20) % 2) == 0) DrawTextEx(pref, "_", (Vector2) {(int)textBox.x + 5 + ((float)(mult_fact * TEXT_SIZE) / 2) * ((mode == 0) ? cur_line->char_count:cur_line->cursor_pos), (int)textBox.y + 12 + (LINE_GAP * ((lb_rows > 0) ? (cur_line->index - lb_rows) : cur_line->index))}, (mult_fact * TEXT_SIZE), TEXT_SPACING, BLACK);
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
