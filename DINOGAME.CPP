#include <graphics.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <stdio.h>

int dinoY = 300, obstacleX = 600; 
int jump = 0, gameOver = 0, score = 0, highScore = 0;

// Load high score from file
void loadHighScore() {
    FILE *f = fopen("HIRECOR.TXT", "r");
    if (f != NULL) {
        fscanf(f, "%d", &highScore);
        fclose(f);
    } else {
        highScore = 0;
    }
}

// Save high score to file
void saveHighScore() {
    if (score > highScore) {
        highScore = score;
        FILE *f = fopen("HIRECOR.TXT", "w");
        if (f != NULL) {
            fprintf(f, "%d", highScore);
            fclose(f);
        }
    }
}

void drawGround() {
    setcolor(WHITE);
    line(0, 350, 639, 350); // Ground line
}

void drawDino() {
    // Dinosaur Body (Green)
    setfillstyle(SOLID_FILL, GREEN);
    bar(100, dinoY + 10, 125, dinoY + 25); // Main body

    // Head
    bar(120, dinoY, 135, dinoY + 15);
    
    // Snout
    bar(132, dinoY + 3, 138, dinoY + 8);
    
    // Eye
    setcolor(BLACK);
    circle(127, dinoY + 4, 1);
    
    // Tail
    setfillstyle(SOLID_FILL, GREEN);
    bar(90, dinoY + 12, 100, dinoY + 16);
    
    // Legs
    bar(105, dinoY + 25, 110, dinoY + 30);
    bar(118, dinoY + 25, 123, dinoY + 30);
}

void drawObstacle() {
    // Cactus (Brown)
    setfillstyle(SOLID_FILL, BROWN);
    
    // Main stem
    bar(obstacleX, 320, obstacleX + 12, 350); 
    
    // Left branch
    bar(obstacleX - 4, 328, obstacleX, 335);
    bar(obstacleX - 4, 323, obstacleX, 328);
    
    // Right branch
    bar(obstacleX + 12, 333, obstacleX + 16, 340);
    bar(obstacleX + 12, 328, obstacleX + 16, 333);
}

int updateGame() {
    cleardevice(); 

    drawGround();

    // Dynamic Speed: Obstacle moves faster as score increases
    int obstacleSpeed = 10 + (score / 3); 
    if (obstacleSpeed > 22) obstacleSpeed = 22; // Cap maximum speed

    obstacleX -= obstacleSpeed;
    if (obstacleX < 0) {
        obstacleX = 600;
        score++; // Increase score
    }

    // Dino Jump Logic
    if (jump > 0) {
        dinoY -= 5;
        jump--;
    } else if (dinoY < 300) {
        dinoY += 5; // Fall down
    }

    drawDino();
    drawObstacle();

    // Display Current Score & High Score
    char sc[10], hsc[10]; 
    sprintf(sc, "%d", score); 
    sprintf(hsc, "%d", highScore); 

    setcolor(WHITE);
    outtextxy(10, 10, "Score: ");
    outtextxy(60, 10, sc); 

    outtextxy(500, 10, "High Score: ");
    outtextxy(590, 10, hsc); 

    // Precise Collision Detection
    if (obstacleX + 16 >= 90 && obstacleX - 4 <= 138 && dinoY + 30 >= 320) {
        gameOver = 1;
        saveHighScore(); // Save if new high score achieved
    }

    // Return dynamic delay (game loop speeds up slightly as score goes up)
    int gameDelay = 45 - (score * 1.5);
    if (gameDelay < 15) gameDelay = 15;
    return gameDelay;
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI"); 

    loadHighScore(); // Load high score on startup

    while (!gameOver) {
        int currentDelay = updateGame();

        if (kbhit()) {
            char key = getch();
            if (key == ' ' && dinoY >= 300) { // Jump only when grounded
                jump = 12; 
            }
        }

        delay(currentDelay); 
    }

    // Game Over Screen
    cleardevice();
    setcolor(RED);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(230, 160, "GAME OVER!");
    
    char finalScoreStr[20], highScoreStr[20];
    sprintf(finalScoreStr, "Final Score: %d", score);
    sprintf(highScoreStr, "High Score: %d", highScore);

    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(235, 210, finalScoreStr);
    outtextxy(235, 235, highScoreStr);
    outtextxy(200, 280, "Press any key to exit.");
    
    getch();
    closegraph();
    return 0;
}