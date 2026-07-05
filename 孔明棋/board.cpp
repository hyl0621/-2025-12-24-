#include <iostream>
#include"class.h"
#include"board.h"

Board::Board() : type(STANDARD), step(0), peg_count(0) {
	board.clear();
	start_board.clear();
	while (!history.empty()) history.pop();
}

// reset（将 board 重置为起始布局）
void Board::reset() {
	board = start_board;
	while (!history.empty()) history.pop();
	step = 0;
	get_remaining();
}

// init_board（按当前 Board::type 初始化 board）
void Board::init_board() {
	board.clear();
	while (!history.empty()) history.pop();

	int rows = 7, cols = 7;
	switch (type) {
	case STANDARD:
		for (int i = 0; i < rows; ++i) {
			std::vector<int> row(classic_board[i], classic_board[i] + cols);
			board.push_back(row);
		}
		break;
	case DIAMOND:
		for (int i = 0; i < rows; ++i) {
			std::vector<int> row(diamond_board[i], diamond_board[i] + cols);
			board.push_back(row);
		}
		break;
	case CROSS:
		for (int i = 0; i < rows; ++i) {
			std::vector<int> row(cross_board[i], cross_board[i] + cols);
			board.push_back(row);
		}
		break;
	case SQUARE:
		for (int i = 0; i < rows; ++i) {
			std::vector<int> row(square_board[i], square_board[i] + cols);
			board.push_back(row);
		}
		break;
	default:
		break;
	}

	// 保存起始布局，初始化计数器
	start_board = board;
	step = 0;
	get_remaining();
}



void Board::printBoardState() const {
	std::cout << "棋盘状态 (步数: " << step << ", 棋子数: " << peg_count << "):" << std::endl;
	for (int y = 0; y < 7; y++) {
		for (int x = 0; x < 7; x++) {
			if (x < board.size() && y < board[x].size()) {
				if (board[x][y] == -1) std::cout << "X ";
				else if (board[x][y] == 0) std::cout << ". ";
				else std::cout << "O ";
			}
			else {
				std::cout << "? ";
			}
		}
		std::cout << std::endl;
	}
}
bool Board::check_move_possible(int x1, int y1, int x2, int y2) {
	if (x1 < 0 || x1 >= 7 || y1 < 0 || y1 >= 7 ||
		x2 < 0 || x2 >= 7 || y2 < 0 || y2 >= 7) {
		return false;
	}

	// 使用board而不是classic_board
	if (board[x1][y1] != 1 || board[x2][y2] != 0) {
		return false;
	}

	int dx = x2 - x1;
	int dy = y2 - y1;

	// 检查水平和垂直移动
	int directions[4][2] = { {2,0}, {-2,0}, {0,2}, {0,-2} };
	for (auto& dir : directions) {
		if (dx == dir[0] && dy == dir[1]) {
			int midX = x1 + dir[0] / 2;
			int midY = y1 + dir[1] / 2;
			if (board[midX][midY] == 1) {
				return true;
			}
			return false;
		}
	}

	return false;
}

GameState Board::check_game_state()//检查游戏状态
{
	get_remaining();
	int num = 0;
	//检查游戏是否成功
	if (peg_count == 1)
		return GameState::GAME_WIN;
	else
	{
		for (int i = 0; i < 7; i++) {
			for (int j = 0; j < 7; j++) {
				if (board[i][j] == 1) {
					// 检查四个方向是否可以移动

					if (!(Board::check_move_possible(i, j, i + 2, j)) &&!( Board::check_move_possible(i, j, i - 2, j)) &&
						!(Board::check_move_possible(i, j, i, j + 2)) && !(Board::check_move_possible(i, j, i, j - 2))) {
						num++;
					}
				}
			}
		}
	}
	if(num==peg_count)
				return GameState::GAME_LOSE;
	return GameState::GAME_PLAYING;
}

void Board::get_remaining()
{
	//获取剩余棋子数
	int number = 0;
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (board[i][j] == 1)
				number++;
		}
	}
	peg_count = number;
}

void Board::undo()
{
	if (!history.empty())
	{
		std::vector<std::vector<int>> previousBoard = history.top();
		history.pop();
		board = previousBoard;  // 赋值给成员变量
		step--;
		peg_count++; // 棋子数加一
	}
}





bool Board::make_move(int x1, int y1, int x2, int y2)
{
	Move move;
	move.from.x = x1;
	move.from.y = y1;
	move.to.x = x2;
	move.to.y = y2;
	move.gap.x = (x1 + x2) / 2;
	move.gap.y = (y1 + y2) / 2;
	if (check_move_possible(x1,y1,x2,y2))
	{
		// 保存当前棋盘状态以便撤销
		history.push(board);
		// 执行移动
		board[x1][y1] = 0; // 源位置设为空
		board[move.gap.x][move.gap.y] = 0; // 中间位置设为空
		board[x2][y2] = 1; // 目标位置设为有棋子
		peg_count--; // 棋子数减一
		step++;
		get_remaining();
		return true;
	}
	else
	{
		return false;
	}
}

std::vector<Move> Board::get_valid_moves()
{
	std::vector<Move> valid_moves;
	for (int x = 0; x < 7; x++) {
		for (int y = 0; y < 7; y++) {
			if (board[x][y] == 1) { // 如果当前位置有棋子
				// 检查四个方向的移动
				int directions[4][2] = { {2,0}, {-2,0}, {0,2}, {0,-2} };
				for (auto& dir : directions) {
					int newX = x + dir[0];
					int newY = y + dir[1];
					if (check_move_possible(x, y, newX, newY)) {
						Move move;
						move.from = { x, y };
						move.to = { newX, newY };
						move.gap = { (x + newX) / 2, (y + newY) / 2 };
						valid_moves.push_back(move);
					}
				}
			}
		}
	}
	return valid_moves;
}


Move Board::get_best_move() {
	std::vector<Move> valid_moves = get_valid_moves();

	if (valid_moves.empty()) {
		return { {-1, -1}, {-1, -1}, {-1, -1} };
	}

	// ：优先移动到中心位置
	int bestScore = -1;
	Move bestMove = valid_moves[0];

	for (const Move& move : valid_moves) {
		int score = 0;

		// 移动到中心位置加分
		if (move.to.x == 3 && move.to.y == 3) {
			score += 10;
		}
		else if(move.to.x >=2 && move.to.x <=4 && move.to.y >=2 && move.to.y <=4)
		{
			score +=5;
		}
		score -= calculate_clustering_score(move);
		score += evaluate_future_mobility(move);
		score += peg_connectivity(move);
		if (score > bestScore) {
			bestScore = score;
			bestMove = move;
		}
	}

	return bestMove;
}


int Board::calculate_clustering_score(const Move& move) {
	
	int clustering_score = 0;
	make_move(move.from.x, move.from.y, move.to.x, move.to.y);//模拟移动
	for (int x = 0; x < 7; x++) {
		for (int y = 0; y < 7; y++) {
			if (board[x][y] == 1 ) 
			{
				int distance = abs(move.to.x - x) + abs(move.to.y - y);
				clustering_score += distance;
			}
		}
	}
	clustering_score = clustering_score / peg_count;
	undo();
	return clustering_score;
}

int Board::evaluate_future_mobility(const Move& move) {
	int mobility_score = 0;

	// 模拟执行移动
	make_move(move.from.x, move.from.y, move.to.x, move.to.y);
	// 计算移动后有多少合法移动
	std::vector<Move> future_moves = get_valid_moves();
	mobility_score = future_moves.size() * 2;  // 移动可能性越多越好

	// 恢复棋盘状态
	undo();

	return mobility_score;
}


int Board::peg_connectivity(const Move& move)
{
	int connectivity_score = 0;
	make_move(move.from.x, move.from.y, move.to.x, move.to.y);//模拟移动
	for(int x = 0; x < 7; x++) {
		for (int y = 0; y < 7; y++) {
			if (board[x][y] == 1) 
			{
				// 检查上下左右四个方向
				int directions[4][2] = { {1,0}, {-1,0}, {0,1}, {0,-1} };
				for (auto& dir : directions) {
					int newX = x + dir[0];
					int newY = y + dir[1];
					if (newX >= 0 && newX < 7 && newY >= 0 && newY < 7) {
						if (board[newX][newY] == 1) {
							connectivity_score += 1; // 每有一个相邻的棋子加分
						}
					}
				}
			}
		}
	}
	undo();

	return connectivity_score;
}