#include <assert.h>
#include <iostream>
#include "Game.h"
#include "raylibCpp.h"
#include <typeinfo>


Game::Game(int width, int height, int maxFps, std::string title)
	:
	board(settings::boardPosition,
		settings::boardWidthHeight,
		settings::backgroundBuffer,
		settings::cellSize,
		settings::padding),

	nextTetrominoMiniBoard(settings::nextTetrominoPosition,
		settings::nextTetrominoWidthHeight,
		settings::nextTetrominoBackgroundBuffer,
		settings::cellSize,
		settings::padding),

	storedTetrominoMiniBoard(settings::storedTetrominoPosition,
		settings::storedTetrominoWidthHeight,
		settings::storedTetrominoBackgroundBuffer,
		settings::cellSize,
		settings::padding)
{
	storedTetromino = NULL;
	assert(!GetWindowHandle());//If assertion triggers : Window is already open
	canStore = true;
	gameOver = false;
	currentScore = 0;
	bufferCounter = 0;
	tickCounter = 0;
	tickcounterForPlayerMoves = 0;
	tickRate = 30 / (board.GetLevel() + 1);
	SetTargetFPS(maxFps);
	InitWindow(width, height, title.c_str());
	currentTetromino = getNewTetromino(&board, GetRandomValue(0, 6), false);
	nextTetromino = getNewTetromino(&nextTetrominoMiniBoard, GetRandomValue(0, 6), true);
	SetWindowIcon(LoadImage(settings::iconFilename));
}

Game::~Game() noexcept
{
	assert(GetWindowHandle());//If assertion triggers : Window is not already open
	CloseWindow();
}

bool Game::ShouldGameClose() const
{
	return WindowShouldClose();
}

void Game::Tick()
{
	BeginDrawing();
	if (!gameOver) {
		Update();
		Draw();
	}
	else {
		DrawGameOver();
	}
	EndDrawing();
}

bool Game::isValidMove(Vec2<int> movement, Tetromino tetromino) const
{	//assumption of moving in instance of 1, -1, 0
	std::vector<int> positions = tetromino.getPositions();
	for (int i = 0; i < positions.size(); i++) {
		//std::cout << " Position " << i << " is " << positions[i] << " | ";//DEBUGGING
		//if (movement.GetY() > 0 && positions[i] >= board.GetHeight() * board.GetWidth() - 3 * board.GetWidth()) {//possibly un needed when collsion on bottem replaced with locking tetrimino
		//	return false;//Invalid Move
		//}
		int x = (positions[i] + movement.GetX()) % board.GetWidth();//Get the x position of the tetrimino as if it was on row 0 E.g 0 -> board.width
		//std::cout << " X: " << x << " and board pos x is " << tetromino.getBoardPos().GetX() << " | ";//DEBUGGING
		if (movement.GetX() > 0 && x < tetromino.getBoardPos().GetX()) {//Checks that the new postion of a square is not further left of the tetriminoes top left (it has wrapped around the array)
			//std::cout << " Invalid!!!!!!!! ";//DEBUGGING
			return false;//Invalid Move
		} else if (movement.GetX() < 0 && x > tetromino.getBoardPos().GetX() + tetromino.getDimension()) {//Checks that the new postion of a square is not further right of the tetriminoes top 
			//std::cout << " Invalid!!!!!!!! ";//DEBUGGING                                                     left + the length of the tetrmino (it has wrapped around the right edge of the array)
			return false;//Invalid Move
		} if (movement.GetX() == 0 && movement.GetY() == 0) {
			if (x < tetromino.getBoardPos().GetX() || tetromino.getBoardPos().GetX() < 0) {
				//std::cout << " Invalid Rotation!!!!!!!! ";//DEBUGGING
				return false;//Invalid Move
			}
		}
		if (shouldTetCollide(movement, tetromino)) {
			return false;
		}
	}
	return true;//Valid Move
}

bool Game::isRotationValid(Tetromino tetromino, bool clockwiseOrNot) const
{
	//std::cout << " ROTATION ";//DEBUGGING
	Tetromino copy = tetromino; //Copy current tetrmino and rotate it
	if (clockwiseOrNot) {
		copy.rotateClockwise();
	}
	else {
		copy.rotateCounterClockwise();
	}
	if (!isValidMove({0,  0}, copy)) {// then check it is not in an invalid place therefore the rotation would be invalid
		return false;//Invalid Rotation
	}
	return true;//Valid Rotation
}

bool Game::shouldTetCollide(Vec2<int> movement, Tetromino tetromino) const
{
	std::vector<int> positions = tetromino.getPositions();
	for (int i = 0; i < positions.size(); i++) {
		Vec2<int> pos = { positions[i] % board.GetWidth(), positions[i] / board.GetWidth() };
		pos += movement;
		if (pos.GetY() >= board.GetHeight()) {
			return true;
		}
		if (board.CellExists(pos)) {
			return true;
		}
	}
	return false;
}

void Game::Update()
{
	updateTickCounter();

	if ( tickCounter >= tickRate) {
		if (!shouldTetCollide(fallVector, *currentTetromino)) {
			(*currentTetromino).Move(fallVector);
		}
		else {
			bufferCounter++;
			if (bufferCounter >= bufferPeriod) {//Gives players a number of ticks after colliding to move the Tetromino
				bufferCounter = 0;

				board.AddFallenTetromino((*currentTetromino).getPositions(), (*currentTetromino).getColor());

				updateCurrentTetrominoToNext();

				if (shouldTetCollide({0, 0}, *currentTetromino) != 0) {
					gameOver = true;
				}
				
				currentScore = board.CheckLineClear();
			}
		}
	}
	if (currentTetromino != NULL) {
		checkKeyPresses();
	}
	updateTickRate();//Forces the tick rate to be dependant on the level and resets the tick counter (slows the game down after player stops speeding it up with S Key)
}



void Game::Draw() const
{
	ClearBackground(BLACK);
	board.Draw(RAYWHITE);
	nextTetrominoMiniBoard.Draw(RAYWHITE);
	storedTetrominoMiniBoard.Draw(RAYWHITE);
	WriteScoreAndLevel();
	(*currentTetromino).Draw();
	(*nextTetromino).Draw();
	if (storedTetromino != NULL) {
		(*storedTetromino).Draw();
	}
}

void Game::DrawGameOver() const
{
	Draw();
	
	//raycpp::DrawText("GAME OVER", settings::gameOverPosition - Vec2<int>{6, 1}, settings::gameOverTextSize + 2, BLACK);
	raycpp::DrawText("GAME OVER", settings::gameOverPosition, settings::gameOverTextSize, BEIGE);
	//raycpp::DrawText(TextFormat("Final Score = %08i", currentScore), settings::finalScorePosition - Vec2<int>{4, 1}, settings::finalScoreTextSize + 1, BEIGE);
	raycpp::DrawText(TextFormat("Final Score = %08i", currentScore), settings::finalScorePosition, settings::finalScoreTextSize, BEIGE);
}

void Game::WriteScoreAndLevel() const
{
	raycpp::DrawText(TextFormat("Level: %02i", board.GetLevel()), settings::levelPosition, settings::levelTextSize, RAYWHITE);
	raycpp::DrawText(TextFormat("Score: %08i", currentScore), settings::scorePosition, settings::scoreTextSize, RAYWHITE);
	raycpp::DrawText("Next:", settings::nextTetrominoText, settings::nextTetTextSize, RAYWHITE);
	raycpp::DrawText("Stored:", settings::storedTetrominoText, settings::storedTetTextSize, RAYWHITE);
}

void Game::checkKeyPresses()
{
	checkRotations();
	checkLateralMoves();
}

void Game::checkRotations()
{
	if (IsKeyPressed(KEY_E)) {
		if (isRotationValid((*currentTetromino), true)) {
			(*currentTetromino).rotateClockwise();
		}
		else if (isRotationValid(getRaisedTetromino(), true)) {
			(*currentTetromino).Move(fallVector * -1 * ((*currentTetromino).getDimension() / 2));
			(*currentTetromino).rotateClockwise();
		}
	}
	else if (IsKeyPressed(KEY_Q)) {
		if (isRotationValid((*currentTetromino), false)) {
			(*currentTetromino).rotateCounterClockwise();
		}
		else if (isRotationValid(getRaisedTetromino(), false)) {
			(*currentTetromino).Move(fallVector * -1 * ((*currentTetromino).getDimension() / 2));
			(*currentTetromino).rotateCounterClockwise();
		}
	}
}

void Game::checkLateralMoves()
{
	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D)) {
		if (tickMoveStarted == -1) {
			tickMoveStarted = tickcounterForPlayerMoves % max(1, (GetFPS() / settings::moveRatePerTick));
		}
		if (tickcounterForPlayerMoves % max(1, (GetFPS() / settings::moveRatePerTick)) == tickMoveStarted) {
			if (IsKeyDown(KEY_A)) {
				if (isValidMove({ -1, 0 }, (*currentTetromino))) {
					(*currentTetromino).Move({ -1, 0 });
				}
			}
			else if (isValidMove({ 1, 0 }, (*currentTetromino))) {
				(*currentTetromino).Move({ 1, 0 });
			}
		}
	}
	else {
		tickMoveStarted = -1;
	}

	if (IsKeyDown(KEY_S)) {
		tickRate = 2;
	}

	if (IsKeyPressed(KEY_W) && canStore) {
		canStore = false;
		if (storedTetromino != NULL) {
			int tempTetromino = (*storedTetromino).GetTypeNumber();
			storedTetromino = getNewTetromino(&storedTetrominoMiniBoard, (*currentTetromino).GetTypeNumber(), true);
			currentTetromino = getNewTetromino(&board, tempTetromino, false);
		}
		else {
			storedTetromino = getNewTetromino(&storedTetrominoMiniBoard, (*currentTetromino).GetTypeNumber(), true);
			updateCurrentTetrominoToNext();
		}
		
	}
}

void Game::updateTickCounter()
{
	tickCounter++;
	tickcounterForPlayerMoves++;
}

void Game::updateTickRate()
{
	if (tickCounter >= tickRate) {
		tickCounter = 0;
		tickRate = 50 / (board.GetLevel() + 1);
	}
	if (tickcounterForPlayerMoves >= GetFPS()) {
		tickcounterForPlayerMoves = 0;
	}
}

void Game::updateCurrentTetrominoToNext()
{
	currentTetromino = getNewTetromino(&board, (*nextTetromino).GetTypeNumber(), false);
	nextTetromino = getNewTetromino(&nextTetrominoMiniBoard, GetRandomValue(0, 6), true);
	canStore = true;
}

Tetromino* Game::getNewTetromino(Board* board, int type, bool isInMiddle) const
{
	switch (type) {
	case 0:
		return new Straight((*board), isInMiddle);
	case 1:
		return new Square((*board), isInMiddle);
	case 2:
		return new Tee((*board), isInMiddle);
	case 3:
		return new Jay((*board), isInMiddle);
	case 4:
		return new El((*board), isInMiddle);
	case 5:
		return new SkewZ((*board), isInMiddle);
	case 6:
		return new SkewS((*board), isInMiddle);
	default:
		assert(false);
		break;
	}
}

Tetromino Game::getRaisedTetromino() const
{
	Tetromino tempTetromino = (*currentTetromino);
	tempTetromino.Move(fallVector * -1 * (tempTetromino.getDimension() / 2));
	return tempTetromino;
}
