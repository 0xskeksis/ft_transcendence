pragma solidity ^0.8.0;

contract TournamentStorage {
    
    struct MatchResult {
        uint256 gameId;
        uint256 player1Id;
        uint256 player2Id;
        uint256 score1;
        uint256 score2;
        uint256 timestamp;
    }

    MatchResult[] public matches;

    address public owner;

    event ScoreSaved(uint256 indexed gameId, uint256 winnerId);

    constructor() {
        owner = msg.sender; 
    }

    function saveScore(uint256 _gameId, uint256 _p1Id, uint256 _p2Id, uint256 _s1, uint256 _s2) external {
        require(msg.sender == owner, "Only owner can save scores");
        
        matches.push(MatchResult({
            gameId: _gameId,
            player1Id: _p1Id,
            player2Id: _p2Id,
            score1: _s1,
            score2: _s2,
            timestamp: block.timestamp
        }));

        uint256 winnerId = _s1 > _s2 ? _p1Id : _p2Id;
        
        emit ScoreSaved(_gameId, winnerId);
    }

    function getMatchCount() external view returns (uint256) {
        return matches.length;
    }
}
