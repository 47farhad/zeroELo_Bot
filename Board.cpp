#include "Board.h"
#include "PieceUtil.h"
#include <cstring>
#include <cstdlib>
#include <iostream>
using namespace std;

Board::Board() {
    // Initialize all bitboards to 0
    memset(pieceBitboards, 0, sizeof(pieceBitboards));
    memset(occupancyBitboards, 0, sizeof(occupancyBitboards));
    attackedBitboard = 0ULL;
    
    // Initialize game state
    zobristHash = 0ULL;
    sideToMove = white;
    castleRights = 0;
    enpassantSquare = -1;
    halfMoveClocks[0] = 0;  // halfmove clock
    halfMoveClocks[1] = 1;  // fullmove number
}

void Board::initFromFen(char* FEN) {
    // Clear the board first
    memset(pieceBitboards, 0, sizeof(pieceBitboards));
    memset(occupancyBitboards, 0, sizeof(occupancyBitboards));
    
    int square = 0;
    char* currentChar = FEN;
    
    // 1. Parse piece positions
    while (*currentChar != ' ' && square < 64) {
        if (*currentChar >= '1' && *currentChar <= '8') {
            // Skip empty squares
            square += (*currentChar - '0');
        }
        else if (*currentChar == '/') {
            // Move to next rank
            // Do nothing as square is already correctly positioned
        }
        else {
            // Place piece
            int piece, color;
            switch(*currentChar) {
                case 'P': piece = 0; color = white; break;
                case 'N': piece = 1; color = white; break;
                case 'B': piece = 2; color = white; break;
                case 'R': piece = 3; color = white; break;
                case 'Q': piece = 4; color = white; break;
                case 'K': piece = 5; color = white; break;
                case 'p': piece = 0; color = black; break;
                case 'n': piece = 1; color = black; break;
                case 'b': piece = 2; color = black; break;
                case 'r': piece = 3; color = black; break;
                case 'q': piece = 4; color = black; break;
                case 'k': piece = 5; color = black; break;
                default: currentChar++; continue;
            }
            set_bit(pieceBitboards[color][piece], square);
            square++;
        }
        currentChar++;
    }
    
    currentChar++;
    
    // 2. Parse side to move
    sideToMove = (*currentChar == 'w') ? white : black;
    currentChar += 2; // Skip to castling rights
    
    // 3. Parse castling rights
    castleRights = 0;
    if (*currentChar == '-') {
        currentChar += 2;
    } else {
        while (*currentChar != ' ') {
            switch(*currentChar) {
                case 'K': castleRights |= 1; break;     // White kingside
                case 'Q': castleRights |= 2; break;     // White queenside
                case 'k': castleRights |= 4; break;     // Black kingside
                case 'q': castleRights |= 8; break;     // Black queenside
            }
            currentChar++;
        }
        currentChar++; 
    }
    
    // 4. Parse en passant square
    if (*currentChar == '-') {
        enpassantSquare = -1;
        currentChar += 2;
    } else {
        int file = currentChar[0] - 'a';
        int rank = 8 - (currentChar[1] - '0');
        enpassantSquare = rank * 8 + file;
        currentChar += 3;
    }
    
    // 5. Parse halfmove clock
    halfMoveClocks[0] = atoi(currentChar);
    while (*currentChar != ' ') currentChar++;
    currentChar++;
    
    // 6. Parse fullmove number
    halfMoveClocks[1] = atoi(currentChar);
    
    // Update occupancy bitboards
    for (int piece = 0; piece < 6; piece++) {
        occupancyBitboards[white] |= pieceBitboards[white][piece];
        occupancyBitboards[black] |= pieceBitboards[black][piece];
    }
    occupancyBitboards[2] = occupancyBitboards[white] | occupancyBitboards[black];
}

void Board::printBoard() const {
    cout << "\n  Chess Board:\n\n";
    
    // Loop through ranks (8 to 1)
    for (int rank = 0; rank < 8; rank++) {
        cout << 8 - rank << "  ";  // Print rank number
        
        // Loop through files (a to h)
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            char piece = '.';  // Default empty square
            
            // Check each piece type and color
            for (int color = 0; color < 2; color++) {
                for (int pieceType = 0; pieceType < 6; pieceType++) {
                    if (get_bit(pieceBitboards[color][pieceType], square)) {
                        // Convert piece type and color to character
                        switch(pieceType) {
                            case 0: piece = (color == white) ? 'P' : 'p'; break;
                            case 1: piece = (color == white) ? 'N' : 'n'; break;
                            case 2: piece = (color == white) ? 'B' : 'b'; break;
                            case 3: piece = (color == white) ? 'R' : 'r'; break;
                            case 4: piece = (color == white) ? 'Q' : 'q'; break;
                            case 5: piece = (color == white) ? 'K' : 'k'; break;
                        }
                    }
                }
            }
            cout << " " << piece;
        }
        cout << endl;
    }
    
    // Print file letters
    cout << "\n    a b c d e f g h\n\n";
    
  
}