#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <sstream>
#include <thread>
#include <math.h>
#include "Consts.h"
#include "BlockColor.h"
#include "Shapes.h"
#include "GameState.h"
using namespace std;
using namespace sf;


int convertColToPx(int col);
int convertRowToPx(int row);
vector<vector<vector<BlockColor>>> randomTetromino();
bool canRotate(vector<vector<BlockColor>>& grid, vector<vector<vector<BlockColor>>> currentTetromino, int& rotation, int tetrominoCoordinateX, int tetrominoCoordinateY);
void eraseTetromino(vector<vector<BlockColor>>& grid, vector<vector<vector<BlockColor>>> currentTetromino, int rotation, int tetrominoCoordinateX, int tetrominoCoordinateY);
void drawTetromino(vector<vector<BlockColor>>& grid, vector<vector<vector<BlockColor>>> currentTetromino, int rotation, int tetrominoCoordinateX, int tetrominoCoordinateY);
void moveTetrominoLeft(vector<vector<BlockColor>> grid, vector<vector<vector<BlockColor>>> currentTetromino, int rotation, int& tetrominoCoordinateX, int tetrominoCoordinateY);
void moveTetrominoRight(vector<vector<BlockColor>> grid, vector<vector<vector<BlockColor>>> currentTetromino, int rotation, int& tetrominoCoordinateX, int tetrominoCoordinateY);
void applyGravityOnTetromino(vector<vector<BlockColor>> grid, vector<vector<vector<BlockColor>>> currentTetromino, int rotation, int tetrominoCoordinateX, int& tetrominoCoordinateY, bool& isDone);
void drawCell(RenderWindow& window, int col, int row, BlockColor color);
void drawGrid(RenderWindow& window, vector<vector<BlockColor>>& grid);
void clearLine(vector<vector<BlockColor>>& grid);

bool isGameDone(vector<vector<BlockColor>>& grid);

void drawHoldTetromino(RenderWindow& holdWindow, vector<vector<vector<BlockColor>>> holdTetromino);

int main()
{
	ContextSettings settings;
	settings.antialiasingLevel = 8;

	RenderWindow window(VideoMode(board_width * total_block_size + 2, (board_height - 4) * total_block_size + 2), "TETRIS", Style::Close | Style::Titlebar);
	RenderWindow holdWindow;
	window.setFramerateLimit(60);

	GameState state = GameState::start;

	vector<vector<BlockColor>> grid(board_height, vector<BlockColor>(board_width, BlockColor::NONE));

	int dropRate = 30;
	int shouldDropRateGoDown = 0;
	int count = 0;

	vector<vector<vector<BlockColor>>> currentTetromino = randomTetromino();
	int rotation = 0;
	#define defaultCoorX ceil(board_width / 2 - currentTetromino[rotation][0].size() / 2)
	int tetrominoCoordinateX = defaultCoorX;
	int tetrominoCoordinateY = 0;
	bool done = false;

	vector<vector<vector<BlockColor>>> holdTetromino = vector<vector<vector<BlockColor>>>(0);
	bool canHold = true;

	bool shouldRotateRight = false;
	bool shouldRotateLeft = false;
	bool shouldGoRight = false;
	bool shouldGoLeft = false;
	bool shouldGoDown = false;
	bool shouldGoDownInstantly = false;
	bool shouldHold = false;

	bool isFirstTime = true;

	Clock clock;
	Clock clock2;
	Clock timeClock;

	fstream readBestTime;
	fstream writeBestTime;

	string time = "";
	int totalTime = 0;

	fstream fileStream;

	Font retroFont;
	retroFont.loadFromFile("font.ttf");

	Event event;
	while (window.isOpen())
	{
		switch (state)
		{
		case start: 
			{
			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed) window.close();
				if (event.type == Event::KeyPressed && event.key.code == Keyboard::A)
				{
					timeClock.restart();
					holdWindow.create(VideoMode(150, 150), "HOLD", Style::Titlebar);
					holdWindow.setPosition(Vector2i(window.getPosition().x - 200, window.getPosition().y));
					window.requestFocus();
					state = GameState::playing;
				}
			}			RectangleShape black;
			black.setFillColor(Color(0, 0, 0, 100));
			black.setSize(Vector2f(window.getSize().x, window.getSize().y));
			black.setPosition(Vector2f(0, 0));
			Text startText1;
			Text startText2;
			startText1.setFont(retroFont);
			int text1Size = clock2.getElapsedTime().asMilliseconds() / 10;
			if (text1Size > 80) text1Size = 80;
			else clock.restart();
			startText1.setCharacterSize(text1Size);
			startText1.setString("NOT");
			startText1.setFillColor(Color::White);
			startText1.setOrigin(Vector2f(startText1.getLocalBounds().width / 2, startText1.getLocalBounds().height / 2));
			startText1.setPosition(Vector2f(window.getSize().x / 2, 150));
			startText2.setFont(retroFont);
			startText2.setCharacterSize(38);
			startText2.setString("TETRIS");
			startText2.setFillColor(Color::White);
			int text2YPos = window.getSize().y - (clock2.getElapsedTime().asMilliseconds() / 1.5f);
			if (text2YPos < 225) text2YPos = 225;
			startText2.setOrigin(Vector2f(startText2.getLocalBounds().width / 2, startText2.getLocalBounds().height / 2));
			startText2.setPosition(Vector2f(window.getSize().x / 2 - 10, text2YPos));
			Text restartText;
			restartText.setFont(retroFont);
			restartText.setString("Press A to start");
			restartText.setCharacterSize(18);
			restartText.setOrigin(Vector2f(restartText.getLocalBounds().width / 2, restartText.getLocalBounds().height / 2));
			restartText.setPosition(Vector2f(window.getSize().x / 2, window.getSize().y / 2));
			restartText.setFillColor(Color::White);
			float timer = clock.getElapsedTime().asMilliseconds();
			window.clear();
			drawGrid(window, grid);
			window.draw(black);
			window.draw(startText1);
			window.draw(startText2);
			if (timer >= 900) clock.restart();
			else if (timer >= 450) window.draw(restartText);
			window.display();
			}
			break;
		case playing:
			{
			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed) {
					window.close();
					holdWindow.close();
				}
				if (event.type == Event::KeyPressed)
				{
					if (event.key.code == Keyboard::E) shouldRotateRight = true;
					if (event.key.code == Keyboard::Q) shouldRotateLeft = true;
					if (event.key.code == Keyboard::D) shouldGoRight = true;
					if (event.key.code == Keyboard::A) shouldGoLeft = true;
					if (event.key.code == Keyboard::S) shouldGoDown = true;
					if (event.key.code == Keyboard::W) shouldGoDownInstantly = true;
					if (event.key.code == Keyboard::Space) shouldHold = true;
				}
			}
			eraseTetromino(grid, currentTetromino, rotation, tetrominoCoordinateX, tetrominoCoordinateY);
			if (shouldRotateLeft) {
				int initial = rotation;
				rotation++;
				if (rotation > currentTetromino.size() - 1) rotation = 0;
				if (!canRotate(grid, currentTetromino, rotation, tetrominoCoordinateX, tetrominoCoordinateY)) rotation = initial;
				shouldRotateLeft = false;
			}
			if (shouldRotateRight) {
				int initial = rotation;
				rotation--;
				if (rotation < 0) rotation = currentTetromino.size() - 1;
				if (!canRotate(grid, currentTetromino, rotation, tetrominoCoordinateX, tetrominoCoordinateY)) rotation = initial;
				shouldRotateRight = false;
			}
			if (shouldGoLeft) {
				moveTetrominoLeft(grid, currentTetromino, rotation, tetrominoCoordinateX, tetrominoCoordinateY);
				shouldGoLeft = false;
			}
			if (shouldGoRight) {
				moveTetrominoRight(grid, currentTetromino, rotation, tetrominoCoordinateX, tetrominoCoordinateY);
				shouldGoRight = false;
			}
			if (shouldGoDown) {
				applyGravityOnTetromino(grid, currentTetromino, rotation, tetrominoCoordinateX, tetrominoCoordinateY, done);
				shouldGoDown = false;
			}
			if (shouldGoDownInstantly) {
				while (!done)
				{
					applyGravityOnTetromino(grid, currentTetromino, rotation, tetrominoCoordinateX, tetrominoCoordinateY, done);
				}
				shouldGoDownInstantly = false;
			}
			clearLine(grid);
			if (isGameDone(grid)) {
				fileStream.open("time.txt", ios::in);
				string bestTime = "";
				int bestTimeInt;
				fileStream >> bestTime;
				if (bestTime == "") bestTime = "0";
				fileStream.close();
				fileStream.open("time.txt", ios::out);
				bestTimeInt = stoi(bestTime);
				if (bestTimeInt < totalTime) fileStream << totalTime;
				else fileStream << bestTime;
				fileStream.close();
				state = GameState::end_game;
			}

			if (shouldHold) {
				if (canHold) {
					vector<vector<vector<BlockColor>>> tempHoldTetromino = holdTetromino;
					holdTetromino = currentTetromino;
					if (tempHoldTetromino.size() == 0) currentTetromino = randomTetromino();
					else currentTetromino = tempHoldTetromino;
					rotation = 0;
					tetrominoCoordinateY = 0;
					tetrominoCoordinateX = defaultCoorX;
					canHold = false;
				}
				shouldHold = false;
			}
			drawTetromino(grid, currentTetromino, rotation, tetrominoCoordinateX, tetrominoCoordinateY);
			if (done) {
				canHold = true;
				currentTetromino = randomTetromino();
				rotation = 0;
				tetrominoCoordinateX = defaultCoorX;
				tetrominoCoordinateY = 0;
				done = false;
				if (shouldDropRateGoDown == 3) {
					float temDropRate = dropRate;
					temDropRate -= temDropRate / 65;
					if (temDropRate < 1) temDropRate = 1;
					dropRate = floor(temDropRate);
					shouldDropRateGoDown = 0;
				}
				else shouldDropRateGoDown++;
			}

			if (count >= dropRate) {
				eraseTetromino(grid, currentTetromino, rotation, tetrominoCoordinateX, tetrominoCoordinateY);
				applyGravityOnTetromino(grid, currentTetromino, rotation, tetrominoCoordinateX, tetrominoCoordinateY, done);
				drawTetromino(grid, currentTetromino, rotation, tetrominoCoordinateX, tetrominoCoordinateY);
				if (done) {
					canHold = true;
					currentTetromino = randomTetromino();
					rotation = 0;
					tetrominoCoordinateX = defaultCoorX;
					tetrominoCoordinateY = 0;
					done = false;
					if (shouldDropRateGoDown == 3) {
						float temDropRate = dropRate;
						temDropRate -= temDropRate / 65;
						if (temDropRate < 1) temDropRate = 1;
						dropRate = floor(temDropRate);
						shouldDropRateGoDown = 0;
					}
					else shouldDropRateGoDown++;
				}
				count = 0;
			}

			int seconds, hours, minutes;
			totalTime = timeClock.getElapsedTime().asSeconds();
			seconds = totalTime;
			minutes = seconds / 60;
			hours = minutes / 60;
			seconds -= 60 * minutes;
			minutes -= 60 * hours;
			string hoursStr, minutesStr, secondsStr;
			if (seconds < 10) secondsStr = "0" + to_string(seconds);
			else secondsStr = to_string(seconds);
			if (hours < 10) hoursStr = "0" + to_string(hours);
			else hoursStr = to_string(seconds);
			if (minutes < 10) minutesStr = "0" + to_string(minutes);
			else minutesStr = to_string(minutes);
			time = hoursStr + ":" + minutesStr + ":" + secondsStr;
			Text timeText;
			timeText.setFont(retroFont);
			timeText.setFillColor(Color::White);
			timeText.setString(time);
			timeText.setCharacterSize(18);
			timeText.setPosition(5, 5);

			holdWindow.clear();
			drawHoldTetromino(holdWindow, holdTetromino);
			holdWindow.display();

			window.clear();
			drawGrid(window, grid);
			window.draw(timeText);
			window.display();

			count++;
			}
			break;
		case end_game:
			{
			if (isFirstTime)
			{
				holdWindow.close();
				clock.restart();
				isFirstTime = false;
			}
			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed) {
					holdWindow.close();
					window.close();
				}
				if (event.type == Event::KeyPressed && event.key.code == Keyboard::A) { 
					grid = vector<vector<BlockColor>>(board_height, vector<BlockColor>(board_width, BlockColor::NONE));
					currentTetromino = randomTetromino();
					tetrominoCoordinateX = defaultCoorX;
					tetrominoCoordinateY = 0;
					holdTetromino = vector<vector<vector<BlockColor>>>(0);
					time = "";
					isFirstTime = true;
					totalTime = 0;
					canHold = true;
					dropRate = 30;
					done = false;
					timeClock.restart();
					holdWindow.close();
					holdWindow.create(VideoMode(150, 150), "HOLD", Style::Titlebar);
					holdWindow.setPosition(Vector2i(window.getPosition().x - 200, window.getPosition().y));
					window.requestFocus();
					state = playing; 
				}
			}
			RectangleShape black;
			black.setFillColor(Color(0, 0, 0, 128));
			black.setSize(Vector2f(window.getSize().x, window.getSize().y));
			black.setPosition(Vector2f(0, 0));
			Text endText;
			endText.setFont(retroFont);
			endText.setString("Game\nOver");
			endText.setCharacterSize(68);
			endText.setOrigin(Vector2f(endText.getLocalBounds().width / 2, endText.getLocalBounds().height / 2));
			endText.setPosition(Vector2f(window.getSize().x / 2, window.getSize().y / 2 - 125));
			endText.setFillColor(Color::White);
			Text restartText;
			restartText.setFont(retroFont);
			restartText.setString("Press A to restart");
			restartText.setCharacterSize(18);
			restartText.setOrigin(Vector2f(restartText.getLocalBounds().width / 2, restartText.getLocalBounds().height / 2));
			restartText.setPosition(Vector2f(window.getSize().x / 2, window.getSize().y / 2));
			restartText.setFillColor(Color::White);
			Text timeText;
			timeText.setFont(retroFont);
			timeText.setString("Your time:");
			timeText.setCharacterSize(20);
			timeText.setOrigin(Vector2f(timeText.getLocalBounds().width / 2, timeText.getLocalBounds().height / 2));
			timeText.setPosition(Vector2f(window.getSize().x / 2, window.getSize().y / 2 + 75));
			timeText.setFillColor(Color::White);
			Text timeText2;
			timeText2.setFont(retroFont);
			timeText2.setString(time);
			timeText2.setCharacterSize(24);
			timeText2.setOrigin(Vector2f(timeText2.getLocalBounds().width / 2, timeText2.getLocalBounds().height / 2));
			timeText2.setPosition(Vector2f(window.getSize().x / 2, window.getSize().y / 2 + 110));
			timeText2.setFillColor(Color::White);
			Text bestTimeText;
			bestTimeText.setFont(retroFont);
			bestTimeText.setString("Your best time:");
			bestTimeText.setCharacterSize(20);
			bestTimeText.setOrigin(Vector2f(bestTimeText.getLocalBounds().width / 2, bestTimeText.getLocalBounds().height / 2));
			bestTimeText.setPosition(Vector2f(window.getSize().x / 2, window.getSize().y / 2 + 200));
			bestTimeText.setFillColor(Color::White);
			string bestTimeStr;
			fileStream.open("time.txt", ios::in);
			fileStream >> bestTimeStr;
			fileStream.close();
			int seconds, minutes, hours;
			seconds = stoi(bestTimeStr);
			minutes = seconds / 60;
			hours = minutes / 60;
			seconds -= 60 * minutes;
			minutes -= 60 * hours;
			string hoursStr, minutesStr, secondsStr;
			if (seconds < 10) secondsStr = "0" + to_string(seconds);
			else secondsStr = to_string(seconds);
			if (hours < 10) hoursStr = "0" + to_string(hours);
			else hoursStr = to_string(seconds);
			if (minutes < 10) minutesStr = "0" + to_string(minutes);
			else minutesStr = to_string(minutes);
			bestTimeStr = hoursStr + ":" + minutesStr + ":" + secondsStr;
			Text bestTimeText2;
			bestTimeText2.setFont(retroFont);
			bestTimeText2.setString(bestTimeStr);
			bestTimeText2.setCharacterSize(24);
			bestTimeText2.setOrigin(Vector2f(bestTimeText2.getLocalBounds().width / 2, bestTimeText2.getLocalBounds().height / 2));
			bestTimeText2.setPosition(Vector2f(window.getSize().x / 2, window.getSize().y / 2 + 235));
			bestTimeText2.setFillColor(Color::White);
			float timer = clock.getElapsedTime().asMilliseconds();
			window.clear();	
			drawGrid(window, grid);
			window.draw(black);
			window.draw(endText);
			window.draw(timeText);
			window.draw(timeText2);
			window.draw(bestTimeText);
			window.draw(bestTimeText2);
			if (timer >= 900) clock.restart();
			else if (timer >= 450) window.draw(restartText);
			window.display();
			}
			break;
		}
	}
	return 0;
}

int convertColToPx(int col) 
{
	return col * total_block_size;
}

int convertRowToPx(int row) 
{
	return (row - 4) * total_block_size;
}

vector<vector<vector<BlockColor>>> randomTetromino() 
{
	random_device rd;
	uniform_int_distribution<int> random(0, 6);
	int shape = random(rd);
	switch (shape)
	{
	case 0:
		return Shapes::shapeI;
		break;
	case 1:
		return Shapes::shapeJ;
		break;
	case 2:
		return Shapes::shapeL;
		break;
	case 3:
		return Shapes::shapeO;
		break;
	case 4:
		return Shapes::shapeS;
		break;
	case 5:
		return Shapes::shapeT;
		break;
	case 6:
		return Shapes::shapeZ;
		break;
	}
}

bool canRotate(vector<vector<BlockColor>>& grid, vector<vector<vector<BlockColor>>> currentTetromino, int& rotation, int tetrominoCoordinateX, int tetrominoCoordinateY) {
	bool canRotate = true;
	for (int row = 0; row < currentTetromino[rotation].size(); row++)
	{
		for (int col = 0; col < currentTetromino[rotation][row].size(); col++)
		{
			if (currentTetromino[rotation][row][col] != BlockColor::NONE)
			{
				int coordinateX = tetrominoCoordinateX + col;
				int coordinateY = tetrominoCoordinateY + row;
				if (coordinateX >= board_width || coordinateY >= board_height || coordinateX < 0) {
					canRotate = false;
					break;
				}
				if (grid[coordinateY][coordinateX] != BlockColor::NONE) {
					canRotate = false;
					break;
				}
			}
		}
	}
	return canRotate;
}

void eraseTetromino(vector<vector<BlockColor>>& grid, vector<vector<vector<BlockColor>>> currentTetromino, int rotation, int tetrominoCoordinateX, int tetrominoCoordinateY)
{
	for (int row = 0; row < currentTetromino[rotation].size(); row++)
	{
		for (int col = 0; col < currentTetromino[rotation][row].size(); col++)
		{
			if (currentTetromino[rotation][row][col] != BlockColor::NONE)
			{
				int coordinateX = tetrominoCoordinateX + col;
				int coordinateY = tetrominoCoordinateY + row;
				grid[coordinateY][coordinateX] = BlockColor::NONE;
			}
		}
	}
}

void drawTetromino(vector<vector<BlockColor>>& grid, vector<vector<vector<BlockColor>>> currentTetromino, int rotation, int tetrominoCoordinateX, int tetrominoCoordinateY) {
	for (int row = 0; row < currentTetromino[rotation].size(); row++)
	{
		for (int col = 0; col < currentTetromino[rotation][row].size(); col++)
		{
			if (currentTetromino[rotation][row][col] != BlockColor::NONE) 
			{
				int coordinateX = tetrominoCoordinateX + col;
				int coordinateY = tetrominoCoordinateY + row;
				grid[coordinateY][coordinateX] = currentTetromino[rotation][row][col];
			}
		}
	}
}

void moveTetrominoLeft(vector<vector<BlockColor>> grid, vector<vector<vector<BlockColor>>> currentTetromino, int rotation, int& tetrominoCoordinateX, int tetrominoCoordinateY)
{
	bool canGoLeft = true;
	for (int row = 0; row < currentTetromino[rotation].size(); row++)
	{
		for (int col = currentTetromino[rotation][row].size() - 1; col >= 0; col--)
		{
			if (currentTetromino[rotation][row][col] != BlockColor::NONE) {
				int coordinateX = tetrominoCoordinateX + col;
				int coordinateY = tetrominoCoordinateY + row;
				if (coordinateX == 0 || grid[coordinateY][coordinateX - 1] != BlockColor::NONE)
				{
					canGoLeft = false;
					break;
				}
			}
		}
	}
	if (canGoLeft) tetrominoCoordinateX--;
}

void moveTetrominoRight(vector<vector<BlockColor>> grid, vector<vector<vector<BlockColor>>> currentTetromino, int rotation, int& tetrominoCoordinateX, int tetrominoCoordinateY)
{
	bool canGoRight = true;
	for (int row = 0; row < currentTetromino[rotation].size(); row++)
	{
		for (int col = currentTetromino[rotation][row].size() - 1; col >= 0; col--)
		{
			if (currentTetromino[rotation][row][col] != BlockColor::NONE) {
				int coordinateX = tetrominoCoordinateX + col;
				int coordinateY = tetrominoCoordinateY + row;
				if (coordinateX == board_width - 1 || grid[coordinateY][coordinateX + 1] != BlockColor::NONE)
				{
					canGoRight = false;
					break;
				}
			}
		}
	}
	if (canGoRight) tetrominoCoordinateX++;
}

void applyGravityOnTetromino(vector<vector<BlockColor>> grid, vector<vector<vector<BlockColor>>> currentTetromino, int rotation, int tetrominoCoordinateX, int& tetrominoCoordinateY, bool& isDone)
{
	bool canFall = true;
	for (int row = currentTetromino[rotation].size() - 1; row >= 0; row--)
	{
		for (int col = 0; col < currentTetromino[rotation][row].size(); col++)
		{
			if (currentTetromino[rotation][row][col] != BlockColor::NONE)
			{
				int coordinateX = tetrominoCoordinateX + col;
				int coordinateY = tetrominoCoordinateY + row;
				if (!(coordinateY < grid.size() - 1) || grid[coordinateY + 1][coordinateX] != BlockColor::NONE)
				{
					canFall = false;
					break;
				}
			}
		}
	}
	if (canFall) tetrominoCoordinateY++;
	else isDone = true;
}

void drawCell(RenderWindow& window, int col, int row, BlockColor color)
{
	RectangleShape cell;
	cell.setPosition(Vector2f(convertColToPx(col) + 5, convertRowToPx(row) + 5));
	cell.setSize(Vector2f(block_size - 5, block_size - 5));
	cell.setOutlineThickness(block_outline_size);
	cell.setFillColor(Color::Black);
	cell.setOutlineColor(Color(24, 22, 22));
	switch (color)
	{
	case BlockColor::I:
		cell.setFillColor(Color(255, 165, 0));
		cell.setOutlineColor(Color(171, 118, 2));
		break;
	case BlockColor::J:
		cell.setFillColor(Color(255, 0, 0));
		cell.setOutlineColor(Color(151, 18, 2));
		break;
	case BlockColor::L:
		cell.setFillColor(Color(0, 0, 255));
		cell.setOutlineColor(Color(0, 0, 192));
		break;
	case BlockColor::O:
		cell.setFillColor(Color(129, 0, 129));
		cell.setOutlineColor(Color(100, 0, 100));
		break;
	case BlockColor::S:
		cell.setFillColor(Color(0, 255, 255));
		cell.setOutlineColor(Color(0, 195, 195));
		break;
	case BlockColor::T:
		cell.setFillColor(Color(0, 255, 0));
		cell.setOutlineColor(Color(0, 191, 0));
		break;
	case BlockColor::Z:
		cell.setFillColor(Color(255, 255, 0));
		cell.setOutlineColor(Color(192, 192, 0));
		break;
	}

	window.draw(cell);
}

void drawGrid(RenderWindow& window, vector<vector<BlockColor>>& grid)
{
	for (int row = 0; row < grid.size(); row++)
	{
		for (int col = 0; col < grid[row].size(); col++) {
			drawCell(window, col, row, grid[row][col]);
		}
	}
}

void clearLine(vector<vector<BlockColor>>& grid) 
{
	for (int row = 0; row < grid.size(); row++)
	{
		bool isLineComplete = true;
		for (int col = 0; col < grid[row].size(); col++)
		{
			if (grid[row][col] == BlockColor::NONE) isLineComplete = false;
		}
		if (isLineComplete) {
			grid.erase(grid.begin() + row);
			vector<BlockColor> line(board_width, BlockColor::NONE);
			grid.insert(grid.begin(), line);
		}
	}
}

bool isGameDone(vector<vector<BlockColor>>& grid)
{
	bool isDone = false;
	for (int row = 0; row < grid[4].size(); row++)
	{
		if (grid[4][row] != BlockColor::NONE) 
		{
			isDone = true;
			break;
		}
	}
	return isDone;
}

void drawHoldTetromino(RenderWindow& holdWindow, vector<vector<vector<BlockColor>>> holdTetromino)
{
	if (holdTetromino.empty()) return;
	for (int row = 0; row < holdTetromino[0].size(); row++)
	{
		bool isLineEmpty = true;
		for (int col = 0; col < holdTetromino[0][row].size(); col++)
		{
			if (holdTetromino[0][row][col] != BlockColor::NONE)
			{
				isLineEmpty = false;
				break;
			}
		}
		if (isLineEmpty) holdTetromino[0].erase(holdTetromino[0].begin() + row);
	}
	int biggestSize = 0;
	if (holdTetromino[0][0].size() > holdTetromino[0].size()) biggestSize = holdTetromino[0][0].size();
	else biggestSize = holdTetromino[0].size();
	int blockSize = 130 / biggestSize;
	int totalSizeX = holdTetromino[0][0].size() * blockSize;
	int totalSizeY = holdTetromino[0].size() * blockSize;
	float offsetX = (150 - totalSizeX) / 2 + 4.5f;
	float offsetY = (150 - totalSizeY) / 2 + 4.5f;
	for (int row = 0; row < holdTetromino[0].size(); row++)
	{
		for (int col = 0; col < holdTetromino[0][row].size(); col++) {
			RectangleShape cell;
			cell.setSize(Vector2f(blockSize - 10, blockSize - 10));
			int positionX = blockSize * col;
			int positionY = blockSize * row;
			cell.setPosition(Vector2f(positionX + offsetX, positionY + offsetY));
			cell.setOutlineThickness(5);
			switch (holdTetromino[0][row][col])
			{
			case BlockColor::I:
				cell.setFillColor(Color(255, 165, 0));
				cell.setOutlineColor(Color(171, 118, 2));
				break;
			case BlockColor::J:
				cell.setFillColor(Color(255, 0, 0));
				cell.setOutlineColor(Color(151, 18, 2));
				break;
			case BlockColor::L:
				cell.setFillColor(Color(0, 0, 255));
				cell.setOutlineColor(Color(0, 0, 192));
				break;
			case BlockColor::O:
				cell.setFillColor(Color(129, 0, 129));
				cell.setOutlineColor(Color(100, 0, 100));
				break;
			case BlockColor::S:
				cell.setFillColor(Color(0, 255, 255));
				cell.setOutlineColor(Color(0, 195, 195));
				break;
			case BlockColor::T:
				cell.setFillColor(Color(0, 255, 0));
				cell.setOutlineColor(Color(0, 191, 0));
				break;
			case BlockColor::Z:
				cell.setFillColor(Color(255, 255, 0));
				cell.setOutlineColor(Color(192, 192, 0));
				break;
			default:
				cell.setFillColor(Color::Black);
				cell.setOutlineThickness(0);
				break;
			}
			holdWindow.draw(cell);
		}
	}
}