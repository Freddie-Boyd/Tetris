#pragma once
#include <string>
#include <vector>
#include "Board.h"
#include "Tetrominoes.h"
#include "Settings.h"

using namespace std;

class Game {
public:
	Game(int width, int heigth, int maxFps, string title);
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	~Game() noexcept;
	bool ShouldGameClose() const;
	void Tick();

private:
	bool isValidMove(Vec2<int> movement, Tetromino tetromino) const;
	bool isRotationValid(Tetromino tetromino, bool clockwiseOrNot) const;
	bool shouldTetCollide(Vec2<int> movement, Tetromino tetromino) const;//Should check if after the movement it would stop momentum and the next tet should spawn
	void Update();													// Also used to check if movement results in colliding with other tetrominos

	void Draw() const;
	void DrawGameOver() const;
	void WriteScoreAndLevel() const;

	void checkKeyPresses();
	void checkRotations();
	void checkLateralMoves();

	void updateTickCounter();
	void updateTickRate();

	void updateCurrentTetrominoToNext();
	Tetromino* getNewTetromino(Board* board, int type, bool getNewTetromino) const;
	Tetromino getRaisedTetromino() const; //returns a copy of the current tetromino raised slightly to allow rotations to raise the tetromino

	bool gameOver;

	bool canStore;

	int currentScore;

	int tickCounter;
	int tickcounterForPlayerMoves;
	int tickRate;
	int bufferCounter;
	constexpr static int bufferPeriod = settings::bufferPeriod;

	int tickMoveStarted;

	constexpr static Vec2<int> fallVector = {0, 1};

	Board board;
	Board nextTetrominoMiniBoard;
	Board storedTetrominoMiniBoard;

	Tetromino *currentTetromino;
	Tetromino *nextTetromino;
	Tetromino *storedTetromino;
};